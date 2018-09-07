/*
 * This module handles remote management rules.
 */

#include "firewall.h"
#include "mod_common.h"

#define REMOTE_MNG_MODULE_RULES (&(remote_management_mod.mod_rules))
#define REMOTE_MNG_MODULE_CHAINS (&(remote_management_mod.mod_chains))

static void rules_in_conntrack_tb();
static void rules_in_filter_tb();
static void rules_in_nat_tb();

module_t remote_management_mod = {
	.module_name = "remote_management_mod",
	.module_desc = "handle remote management rules",
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
	.conntrack_tb_routine = rules_in_conntrack_tb,
	.filter_tb_routine = rules_in_filter_tb,
	.nat_tb_routine = rules_in_nat_tb,
	.mangle_tb_routine = NULL
};

static void rules_in_conntrack_tb(void)
{
	module_conntrack_t new_conntrack_list = {
		.conntrack_rules_head = NULL,
		.conntrack_rules_tail = NULL,
	};

	/* When system mode is bridge or wb, do nothing. */
	if(mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "bridge") || (mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") && mapi_ccfg_match_str(tid, "ARC_WAN_0_BridgeEnable", "1")) || mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "wb"))
		return;

	/* Web management */
	if (remotemanage && natenable)
	{
		char port[16];
		conntrack_info_t conntrack_data;

		mapi_ccfg_get_str(tid, "ARC_UI_WEB_REMOTEMN_Port", port, sizeof(port));

		conntrack_data.orig_port_src = 0;
		conntrack_data.orig_port_dst = atoi(port);
		conntrack_data.orig_src = 0;
		conntrack_data.orig_dst = inet_addr(wanaddr);
		conntrack_data.reply_src = inet_addr(lanaddr);
		conntrack_data.reply_dst = 0;
		conntrack_data.reply_port_src = web_lanport;
		conntrack_data.reply_port_dst = 0;
		conntrack_data.proto = 6;
		conntrack_data.orig_port_dst_range = 0;
		conntrack_data.reply_port_src_range = 0;

		add_conntrack_rule(&new_conntrack_list, conntrack_data);
	}

	compare_and_clean_conntrack(&(remote_management_mod.mod_conntrack), &new_conntrack_list);
}

static void rules_in_filter_tb()
{
	/* When system mode is bridge or wb, do nothing. */
	if(mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "bridge") || (mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") && mapi_ccfg_match_str(tid, "ARC_WAN_0_BridgeEnable", "1")) || mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "wb"))
		return;

	// -A INPUT -d 192.168.2.1/32 -p tcp -m tcp --dport 80 -j ACCEPT
	if (remotemanage)
	{
		append_rule(REMOTE_MNG_MODULE_RULES, FILTER_TABLE, "-A INPUT -p tcp -m tcp -d %s --dport %d -j ACCEPT\n",
				lanaddr, web_lanport);
	}
}

static void set_multiple_pppoe_rules()
{
	char glbcfg[128] = "";
	char wanip[20] = "";
	char buffer[128] = "";
	char port[12] = "";
	int ii[4], to = 0;
	int i = 0;

	mapi_ccfg_get_str(tid, "ARC_UI_WEB_REMOTEMN_Port", port, sizeof(port));

	for(i = 0; i < PPPOE_MAX_COUNT; i++)
	{
		snprintf(glbcfg, sizeof(glbcfg), "ARC_WAN_%d_PPPOE_Enable", i);
		if(mapi_ccfg_match_str(tid, glbcfg, "1"))
		{
			snprintf(glbcfg, sizeof(glbcfg), "ARC_WAN_%d_PPP_GET_IP4_Addr", i);
			mapi_ccfg_get_str(tid, glbcfg, wanip, sizeof(wanip));
			if(strcmp(wanip, "") && strcmp(wanip, "0.0.0.0"))
			{
				if(mapi_ccfg_match_str(tid, "ARC_UI_WEB_REMOTEMN_ExposeToIP", ""))
				{
					append_rule(REMOTE_MNG_MODULE_RULES, NAT_TABLE, "-A PREROUTING -p tcp -m tcp -d %s --dport %s "
						"-j DNAT --to-destination %s:%d\n", wanip, port,
						lanaddr, web_lanport);
				}
				else
				{
					sscanf(mapi_ccfg_get_str(tid, "ARC_UI_WEB_REMOTEMN_ExposeToIP",buffer,sizeof(buffer)),"%d.%d.%d.%d-%d",
						&ii[0],&ii[1],&ii[2],&ii[3], &to);
					if(to == 0)
						to = ii[3];
					append_rule(REMOTE_MNG_MODULE_RULES, NAT_TABLE, "-A PREROUTING -m iprange --src-range %d.%d.%d.%d-%d.%d.%d.%d -p tcp "
						"-d %s --dport %s -j DNAT --to-destination %s:%d\n",
						ii[0],ii[1],ii[2],ii[3],ii[0],ii[1],ii[2],to, wanip,
						port, lanaddr, web_lanport);
				}
			} //end of if(strcmp(wanip, "") && strcmp(wanip, "0.0.0.0"))
		}//end of if(mapi_ccfg_match_str(tid, glbcfg, "1"))
	}// end of for(i = 0; i < PPPOE_MAX_COUNT; i++)
}

static void rules_in_nat_tb()
{
	char buffer[128], port[12];
	int ii[4];
	int to = 0;

	/* When system mode is bridge or wb, do nothing. */
	if(mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "bridge") || (mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") && mapi_ccfg_match_str(tid, "ARC_WAN_0_BridgeEnable", "1")) || mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "wb"))
		return;

	// -A PREROUTING -d wanaddr -p tcp -m iprange --src-range 192.168.106.2-192.168.106.254 -m tcp
	// --dport 8080 -j DNAT --to-destination 192.168.2.1:80
	if (remotemanage && natenable)
	{
		if(mapi_ccfg_match_str(tid, "ARC_WAN_0_IP4_Proto", "pppoe"))
		{
			set_multiple_pppoe_rules();
		}
		else
		{
			//crazy_liang 2013.4.12: Fix bad codings.
			mapi_ccfg_get_str(tid, "ARC_UI_WEB_REMOTEMN_Port", port, sizeof(port));
			if(mapi_ccfg_match_str(tid, "ARC_UI_WEB_REMOTEMN_ExposeToIP", ""))
			{
				append_rule(REMOTE_MNG_MODULE_RULES, NAT_TABLE, "-A PREROUTING -p tcp -m tcp -d %s --dport %s "
					"-j DNAT --to-destination %s:%d\n", wanaddr, port,
					lanaddr, web_lanport);
			}
			else
			{
				sscanf(mapi_ccfg_get_str(tid, "ARC_UI_WEB_REMOTEMN_ExposeToIP",buffer,sizeof(buffer)),"%d.%d.%d.%d-%d",
					&ii[0],&ii[1],&ii[2],&ii[3], &to);
				if(to == 0)
					to = ii[3];
				append_rule(REMOTE_MNG_MODULE_RULES, NAT_TABLE, "-A PREROUTING -m iprange --src-range %d.%d.%d.%d-%d.%d.%d.%d -p tcp "
					"-d %s --dport %s -j DNAT --to-destination %s:%d\n",
					ii[0],ii[1],ii[2],ii[3],ii[0],ii[1],ii[2],to, wanaddr,
					port, lanaddr, web_lanport);
			}
		}
	}
}
