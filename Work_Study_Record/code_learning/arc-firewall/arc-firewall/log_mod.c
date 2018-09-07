/*
 * This module handles log and drop rules.  */

#include "firewall.h"
#include "mod_common.h"

#define LOG_MODULE_CHAINS (&(log_mod.mod_chains))
#define LOG_MODULE_RULES (&(log_mod.mod_rules))

static void rules_in_filter_tb();


module_t log_mod = {
	.module_name = "log_mod",
	.module_desc = "handle log and drop rules",
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
	int logenable;                /* boolean value*/
	char log_enable_buf[32];      /* glbcfg: "ARC_SYS_LogEnable" value*/

	/* When system mode is bridge or wb, do nothing. */
	if(mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "bridge") || (mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") && mapi_ccfg_match_str(tid, "ARC_WAN_0_BridgeEnable", "1")) || mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "wb"))
		return;

	memset(log_enable_buf,0,sizeof(log_enable_buf));

	/* add a new chain call LOG_DROP, handle log and drop rules */
	append_chain(LOG_MODULE_CHAINS, FILTER_TABLE, "LOG_DROP");

	/* only the firewall enable, should we consider log the firewall or not */
	if (firewallenable)
	{
		/* get the glbcfg, default is enable to log the firewall, so if not set the vaule, we still regard it as enable */
		mapi_ccfg_get_str(tid, "ARC_SYS_LogEnable", log_enable_buf, sizeof(log_enable_buf));
		if (strcmp(log_enable_buf, "") == 0 || strcmp(log_enable_buf, "1") == 0)
		{
			logenable = 1;
		}
		else
		{
			logenable = 0;
		}

		if (logenable)
		{
			append_rule(LOG_MODULE_RULES, FILTER_TABLE, "-A LOG_DROP -j ULOG --ulog-prefix \"IPTABLES denied:\"\n"); 
		}
	}

	/* drop all the packet on this chain*/
	append_rule(LOG_MODULE_RULES, FILTER_TABLE, "-A LOG_DROP -j DROP\n"); 
	

}

