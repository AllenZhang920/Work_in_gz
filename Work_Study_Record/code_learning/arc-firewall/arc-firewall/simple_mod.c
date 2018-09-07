/* instrusion detection module handles anti-dos rules */

#include "firewall.h"
#include "mod_common.h"

#define SIMPLE_MODULE_RULES (&(simple_mod.mod_rules))
#define SIMPLE_MODULE_CHAINS (&(simple_mod.mod_chains))


static void rules_in_filter_tb();

module_t simple_mod = {
	.module_name = "simple_mod",
	.module_desc = "handle basic simple rules",
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
	.filter_tb_routine = rules_in_filter_tb,
	.nat_tb_routine = NULL,
	.mangle_tb_routine = NULL
};



static void rules_in_filter_tb()
{
	int i;
	char glbcfg[128];
	char wan_face[IFNAMSIZ];

	/* When system mode is bridge or wb, do nothing. */
	if(mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "bridge") || (mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") && mapi_ccfg_match_str(tid, "ARC_WAN_0_BridgeEnable", "1")) || mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "wb"))
		return;

	/**  NBT & Microsoft-DS routing */
	if(mapi_ccfg_match_str(tid, "ARC_FIREWALL_NBTDrop", "1"))
	{
		append_chain(SIMPLE_MODULE_CHAINS, FILTER_TABLE, "SIMPLE-NBT-ROUTING-DROP");
	
		if ((mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") ||
			mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "router")) &&
			mapi_ccfg_match_str(tid, "ARC_WAN_0_IP4_Proto", "pppoe"))	
		{
			for(i =0; i < PPPOE_MAX_COUNT; i++ )
			{
				snprintf(glbcfg, sizeof(glbcfg), "ARC_FIREWALL_NBTDrop_PPPoE%d", i);
				if (mapi_ccfg_match_str(tid, glbcfg, "1"))
				{
					snprintf(glbcfg, sizeof(glbcfg), "ARC_WAN_%d_Iface", i);
					mapi_ccfg_get_str(tid, glbcfg, wan_face, sizeof(wan_face));
					if(strcmp(wan_face, ""))
					{
						append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A FORWARD -p udp --dport 445 -i %s -j SIMPLE-NBT-ROUTING-DROP\n", wan_face);
						append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A FORWARD -p udp --dport 445 -o %s -j SIMPLE-NBT-ROUTING-DROP\n", wan_face);
						append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A FORWARD -p udp --dport 137:139 -i %s -j SIMPLE-NBT-ROUTING-DROP\n", wan_face);
						append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A FORWARD -p udp --dport 137:139 -o %s -j SIMPLE-NBT-ROUTING-DROP\n", wan_face);
						append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A FORWARD -p udp --sport 137:139 -i %s -j SIMPLE-NBT-ROUTING-DROP\n", wan_face);
						append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A FORWARD -p udp --sport 137:139 -o %s -j SIMPLE-NBT-ROUTING-DROP\n", wan_face);
						append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A FORWARD -p tcp --dport 445 -i %s -j SIMPLE-NBT-ROUTING-DROP\n", wan_face);
						append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A FORWARD -p tcp --dport 445 -o %s -j SIMPLE-NBT-ROUTING-DROP\n", wan_face);
						append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A FORWARD -p tcp --dport 137:139 -i %s -j SIMPLE-NBT-ROUTING-DROP\n", wan_face);
						append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A FORWARD -p tcp --dport 137:139 -o %s -j SIMPLE-NBT-ROUTING-DROP\n", wan_face);
						append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A FORWARD -p tcp --sport 137:139 -i %s -j SIMPLE-NBT-ROUTING-DROP\n", wan_face);
						append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A FORWARD -p tcp --sport 137:139 -o %s -j SIMPLE-NBT-ROUTING-DROP\n", wan_face);
					}
				}
			}
		}
		else
		{
			append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A FORWARD -p udp --dport 445 -j SIMPLE-NBT-ROUTING-DROP\n");
			append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A FORWARD -p udp --dport 137:139 -j SIMPLE-NBT-ROUTING-DROP\n");
			append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A FORWARD -p udp --sport 137:139 -j SIMPLE-NBT-ROUTING-DROP\n");
			append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A FORWARD -p tcp --dport 445 -j SIMPLE-NBT-ROUTING-DROP\n");
			append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A FORWARD -p tcp --dport 137:139 -j SIMPLE-NBT-ROUTING-DROP\n");
			append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A FORWARD -p tcp --sport 137:139 -j SIMPLE-NBT-ROUTING-DROP\n");
		}
		if(mapi_ccfg_match_str(tid, "ARC_SYS_LOG_Firewall_Enable", "1"))
		{
			append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A SIMPLE-NBT-ROUTING-DROP -j ULOG --ulog-prefix \"FIREWALL-FILTER-DROP:\"\n");
		}
		append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A SIMPLE-NBT-ROUTING-DROP -j DROP\n");
	}

	/* IDENT request */
	if(mapi_ccfg_match_str(tid, "ARC_FIREWALL_IdentRejct", "1"))
	{
		append_chain(SIMPLE_MODULE_CHAINS, FILTER_TABLE, "SIMPLE-IDENT-REJECT");

		if ((mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") ||
			mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "router")) &&
			mapi_ccfg_match_str(tid, "ARC_WAN_0_IP4_Proto", "pppoe"))
		{
			append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A INPUT -p tcp --dport 113 -i ppp+ -j SIMPLE-IDENT-REJECT\n");
		}
		else
			append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A INPUT -p tcp --dport 113 -i %s -j SIMPLE-IDENT-REJECT\n", wanface);
		if(mapi_ccfg_match_str(tid, "ARC_SYS_LOG_Firewall_Enable", "1"))
		{
			append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A SIMPLE-IDENT-REJECT -p tcp -j ULOG --ulog-prefix \"FIREWALL-FILTER-REJECT :\"\n");
		}
		append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A SIMPLE-IDENT-REJECT -p tcp -j REJECT --reject-with tcp-reset\n");
 	}

	/* ICMP */
	append_chain(SIMPLE_MODULE_CHAINS, FILTER_TABLE, "SIMPLE-ICMP");
	if ((mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") ||
		mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "router")) &&
		mapi_ccfg_match_str(tid, "ARC_WAN_0_IP4_Proto", "pppoe"))
	{
		if (mapi_ccfg_match_str(tid, "ARC_FIREWALL_BlockWANPing", "0"))
		{
			append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A INPUT -i ppp+ -p icmp --icmp-type 8/0 -j SIMPLE-ICMP\n");
		}
		else
		{
			append_chain(SIMPLE_MODULE_CHAINS, FILTER_TABLE, "SIMPLE-ICMP-PPP-ACCEPT");
			append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A SIMPLE-ICMP-PPP-ACCEPT -j ACCEPT\n");
			for(i =0; i < PPPOE_MAX_COUNT; i++ )
			{
				snprintf(glbcfg, sizeof(glbcfg), "ARC_WAN_%d_PPPOE_Enable", i);
				if(mapi_ccfg_match_str(tid, glbcfg, "1"))
				{
					snprintf(glbcfg, sizeof(glbcfg), "ARC_FIREWALL_BlockWANPing_PPPoE%d", i);
					if (mapi_ccfg_match_str(tid, glbcfg, "1"))
					{
						snprintf(glbcfg, sizeof(glbcfg), "ARC_WAN_%d_Iface", i);
						mapi_ccfg_get_str(tid, glbcfg, wan_face, sizeof(wan_face));
						if(strcmp(wan_face, ""))
						{
							append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A INPUT -i %s -p icmp --icmp-type 8/0 -j SIMPLE-ICMP\n", wan_face);
						}
					}
					else if (mapi_ccfg_match_str(tid, glbcfg, "0"))
					{
						snprintf(glbcfg, sizeof(glbcfg), "ARC_WAN_%d_Iface", i);
						mapi_ccfg_get_str(tid, glbcfg, wan_face, sizeof(wan_face));
						if(strncmp(wan_face, "ppp", 3) == 0)
							append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A INPUT -i %s -p icmp --icmp-type 8/0 -j SIMPLE-ICMP-PPP-ACCEPT\n", wan_face);
					}
				}
			}
		}
	}
	else
		append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A INPUT -i %s -p icmp --icmp-type 8/0 -j SIMPLE-ICMP\n", wanface);
	if (mapi_ccfg_match_str(tid, "ARC_FIREWALL_BlockWANPing", "1"))
	{
		if(mapi_ccfg_match_str(tid, "ARC_SYS_LOG_Firewall_Enable", "1"))
		{
			append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A SIMPLE-ICMP -j ULOG --ulog-prefix \"FIREWALL-FILTER-DROP :\"\n");
		}
		append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A SIMPLE-ICMP -j DROP\n");
	}
	else
	{
		/* Accept only ICMP echo */
		append_rule(SIMPLE_MODULE_RULES, FILTER_TABLE, "-A SIMPLE-ICMP -j ACCEPT\n");
	}
}



