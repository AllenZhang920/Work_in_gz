/*
 * This module handles log and drop rules.  */

#include "firewall.h"
#include "mod_common.h"

#define VPN_PRE_MODULE_CHAINS (&(vpn_pre_mod.mod_chains))
#define VPN_PRE_MODULE_RULES (&(vpn_pre_mod.mod_rules))
#define VPN_POST_MODULE_CHAINS (&(vpn_post_mod.mod_chains))
#define VPN_POST_MODULE_RULES (&(vpn_post_mod.mod_rules))

static void pre_rules_in_filter_tb(void);
static void post_rules_in_filter_tb(void);
static void pre_rules_in_nat_tb(void);

module_t vpn_pre_mod = {
	.module_name = "vpn_pre_mod",
	.module_desc = "handle vpn server rules",
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
	.nat_tb_routine = pre_rules_in_nat_tb,
	.mangle_tb_routine = NULL
};

module_t vpn_post_mod = {
	.module_name = "vpn_post_mod",
	.module_desc = "handle vpn server rules",
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
	.filter_tb_routine = post_rules_in_filter_tb,
	.nat_tb_routine = NULL,
	.mangle_tb_routine = NULL
};

static void pre_rules_in_filter_tb(void)
{
	/* When system mode is bridge or wb, do nothing. */
	if(mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "bridge") || (mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") && mapi_ccfg_match_str(tid, "ARC_WAN_0_BridgeEnable", "1")) || mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "wb"))
		return;

	if (mapi_ccfg_match_str(tid, "ARC_VPNSERVER_PPTP_Enable", "1"))
	{
		append_rule(VPN_PRE_MODULE_RULES, FILTER_TABLE, "-A INPUT -p tcp --dport 1723 -i %s -m state --state NEW -j ACCEPT\n", wanface); 
		append_rule(VPN_PRE_MODULE_RULES, FILTER_TABLE, "-A INPUT -p 47 -i %s -m state --state NEW -j ACCEPT\n", wanface);
	}
}

static void post_rules_in_filter_tb(void)
{
	/* When system mode is bridge or wb, do nothing. */
	if(mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "bridge") || (mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") && mapi_ccfg_match_str(tid, "ARC_WAN_0_BridgeEnable", "1")) || mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "wb"))
		return;

	if (mapi_ccfg_match_str(tid, "ARC_VPNSERVER_PPTP_Enable", "1"))
	{
		append_rule(VPN_POST_MODULE_RULES, FILTER_TABLE, "-A INPUT -i ppp+ -m state --state NEW -j ACCEPT\n"); 
		append_rule(VPN_POST_MODULE_RULES, FILTER_TABLE, "-A FORWARD -i ppp+ -m state --state NEW -j ACCEPT\n"); 
	}
}

static void pre_rules_in_nat_tb(void)
{
	/* When system mode is bridge or wb, do nothing. */
	if(mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "bridge") || (mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") && mapi_ccfg_match_str(tid, "ARC_WAN_0_BridgeEnable", "1")) || mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "wb"))
		return;

	if (mapi_ccfg_match_str(tid, "ARC_VPNSERVER_PPTP_Enable", "1") && mapi_ccfg_match_str(tid, "ARC_FIREWALL_DMZ_Enable","1"))
	{
		append_rule(VPN_PRE_MODULE_RULES, NAT_TABLE, "-A PREROUTING -d %s -p tcp --dport 1723 -i %s -j DNAT --to-destination %s\n", wanaddr, wanface, lanaddr);
		append_rule(VPN_PRE_MODULE_RULES, NAT_TABLE, "-A PREROUTING -d %s -p 47 -i %s -j DNAT --to-destination %s\n", wanaddr, wanface, lanaddr);
	}
}
