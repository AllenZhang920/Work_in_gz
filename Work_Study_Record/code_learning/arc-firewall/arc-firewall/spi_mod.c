/*
 * This module handles SPI rules.
 */

#include "firewall.h"
#include "mod_common.h"

#define SPI_PRI_MODULE_RULES (&(state_packet_inspect_pre_mod.mod_rules))
#define SPI_PRI_MODULE_CHAINS (&(state_packet_inspect_pre_mod.mod_chains))
#define SPI_POST_MODULE_RULES (&(state_packet_inspect_post_mod.mod_rules))
#define SPI_POST_MODULE_CHAINS (&(state_packet_inspect_post_mod.mod_chains))
#define SPI_MID_MODULE_RULES (&(state_packet_inspect_mid_mod.mod_rules))
#define SPI_MID_MODULE_CHAINS (&(state_packet_inspect_mid_mod.mod_chains))

static void pre_rules_in_filter_tb();
static void post_rules_in_filter_tb();
static void mid_rules_in_filter_tb();

module_t state_packet_inspect_pre_mod = {
	.module_name = "state_packet_inspect_pre_mod",
	.module_desc = "handle spi pre-rules",
	.mod_chains = {
		.filter_chains = {NULL,NULL},
		.nat_chains = {NULL,NULL},
		.mangle_chains = {NULL,NULL}
		},
	.mod_rules = {
		.filter_rules_head = NULL,
		.filter_rules_tail = NULL,
		.nat_rules_head = NULL,
		.nat_rules_tail = NULL,
		.mangle_rules_head = NULL,
		.mangle_rules_tail = NULL},
	.mod_conntrack = {
		.conntrack_rules_head = NULL,
		.conntrack_rules_tail = NULL,
	},
	.conntrack_tb_routine = NULL,
	.filter_tb_routine = pre_rules_in_filter_tb,
	.nat_tb_routine = NULL,
	.mangle_tb_routine = NULL
};

module_t state_packet_inspect_post_mod = {
	.module_name = "state_packet_inspect_post_mod",
	.module_desc = "handle spi post-rules",
	.mod_chains = {
		.filter_chains = {NULL,NULL},
		.nat_chains = {NULL,NULL},
		.mangle_chains = {NULL,NULL}
		},
	.mod_rules = {
		.filter_rules_head = NULL,
		.filter_rules_tail = NULL,
		.nat_rules_head = NULL,
		.nat_rules_tail = NULL,
		.mangle_rules_head = NULL,
		.mangle_rules_tail = NULL},
	.filter_tb_routine = post_rules_in_filter_tb,
	.nat_tb_routine = NULL,
	.mangle_tb_routine = NULL
};

module_t state_packet_inspect_mid_mod = {
	.module_name = "state_packet_inspect_mid_mod",
	.module_desc = "handle spi mid-rules",
	.mod_chains = {
		.filter_chains = {NULL,NULL},
		.nat_chains = {NULL,NULL},
		.mangle_chains = {NULL,NULL}
		},
	.mod_rules = {
		.filter_rules_head = NULL,
		.filter_rules_tail = NULL,
		.nat_rules_head = NULL,
		.nat_rules_tail = NULL,
		.mangle_rules_head = NULL,
		.mangle_rules_tail = NULL},
	.filter_tb_routine = mid_rules_in_filter_tb,
	.nat_tb_routine = NULL,
	.mangle_tb_routine = NULL
};

static void pre_rules_in_filter_tb()
{
	/* When system mode is bridge or wb, do nothing. */
	if(mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "bridge") || (mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") && mapi_ccfg_match_str(tid, "ARC_WAN_0_BridgeEnable", "1")) || mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "wb"))
		return;

	if (firewallenable)
	{
		/* Filter known SPI state */
		if ((mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") ||
			mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "router")) &&
			mapi_ccfg_match_str(tid, "ARC_WAN_0_IP4_Proto", "pppoe"))
		{
			append_rule(SPI_PRI_MODULE_RULES, FILTER_TABLE, "-A INPUT -m state --state INVALID -i ppp+ -j LOG_DROP\n");
		}
		else
			append_rule(SPI_PRI_MODULE_RULES, FILTER_TABLE, "-A INPUT -m state --state INVALID -i %s -j LOG_DROP\n", wanface);

		append_rule(SPI_PRI_MODULE_RULES, FILTER_TABLE, "-A INPUT -m state --state RELATED,ESTABLISHED -j ACCEPT\n");

		//append_rule(SPI_PRI_MODULE_RULES, FILTER_TABLE, "-A FORWARD -i %s -o %s -j ACCEPT\n", lanface, lanface);
		append_rule(SPI_PRI_MODULE_RULES, FILTER_TABLE, "-A FORWARD -m state --state INVALID -j LOG_DROP\n");

		//append_rule(SPI_PRI_MODULE_RULES, FILTER_TABLE, "-A FORWARD -m state --state RELATED,ESTABLISHED -j ACCEPT\n");
	}
}

static void post_rules_in_filter_tb()
{
	/* When system mode is bridge or wb, do nothing. */
	if(mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "bridge") || (mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") && mapi_ccfg_match_str(tid, "ARC_WAN_0_BridgeEnable", "1")) || mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "wb"))
		return;

	if (firewallenable)
	{
		int i;
		char multi_wan_ifname[32] = {0};
		char glbcfg[128] = {0};
		/* 
		 * Henry Lee Aug-7th-2013
		 * After deleting a specific conntrack, this conntrack can be
		 * reestablished by receiving an ACK packet from LAN side. In order to
		 * fix it, we should reject these non conntracked ACKs. This may cause
		 * a problem when the router is not the only one gateway.
		 */
		append_rule(SPI_POST_MODULE_RULES, FILTER_TABLE, "-A FORWARD -i %s "
				"-m state --state NEW -p tcp --tcp-flags ACK ACK -j REJECT "
				"--reject-with tcp-reset\n", lanface);
		/* End by Henry Lee */

		/* Filter known SPI state */
		append_rule(SPI_POST_MODULE_RULES, FILTER_TABLE,"-A FORWARD -i %s -m state --state NEW -j ACCEPT\n", lanface);

		/* When NAT is disabled, just accept packets from WAN to LAN directly. */
		if (natenable == 0)
		{
			if ((mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") ||
				mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "router")) &&
				mapi_ccfg_match_str(tid, "ARC_WAN_0_IP4_Proto", "pppoe"))
			{
				append_rule(SPI_POST_MODULE_RULES, FILTER_TABLE,"-A FORWARD -i ppp+ -o %s -j ACCEPT\n", lanface);
			}
			else
			{
				append_rule(SPI_POST_MODULE_RULES, FILTER_TABLE,"-A FORWARD -i %s -o %s -j ACCEPT\n",wanface, lanface);
			}

		}

		if(mapi_ccfg_match_str(tid, "ARC_FIREWALL_AC_Enable", "1"))
		{
			append_rule(SPI_POST_MODULE_RULES, FILTER_TABLE,"-A INPUT -i %s -j AC-INPUT-CHAIN\n", lanface);
		}
		
		append_rule(SPI_POST_MODULE_RULES, FILTER_TABLE,"-A FORWARD -i lo -m state --state NEW -j ACCEPT\n");
		
		append_rule(SPI_POST_MODULE_RULES, FILTER_TABLE,"-A INPUT -i lo -m state --state NEW -j ACCEPT\n"
				"-A INPUT -i %s -m state --state NEW -j ACCEPT\n", lanface);

		if ((mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") ||
			mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "router")) &&
			mapi_ccfg_match_str(tid, "ARC_WAN_0_IP4_Proto", "pppoe"))
		{
			for (i=0; i<PPPOE_MAX_COUNT; i++)
			{
				snprintf(glbcfg, sizeof(glbcfg), "ARC_WAN_%d_PPPOE_Enable", i);
				if(mapi_ccfg_match_str(tid, glbcfg, "1"))
				{
					snprintf(glbcfg, sizeof(glbcfg), "ARC_WAN_%d_Iface", i);
					mapi_ccfg_get_str(tid, glbcfg, multi_wan_ifname, sizeof(multi_wan_ifname));
					if(strcmp(multi_wan_ifname, ""))
					{
						append_rule(SPI_POST_MODULE_RULES, FILTER_TABLE, "-A FORWARD -i %s -j DROP\n", multi_wan_ifname);
						append_rule(SPI_POST_MODULE_RULES, FILTER_TABLE, "-A INPUT -i %s -j DROP\n", multi_wan_ifname);
					}
				}
			}
		}
	}
}


static void mid_rules_in_filter_tb()
{
	if (firewallenable)
	{
		append_rule(SPI_MID_MODULE_RULES, FILTER_TABLE, "-A FORWARD -m state --state RELATED,ESTABLISHED -j ACCEPT\n");
	}
}


