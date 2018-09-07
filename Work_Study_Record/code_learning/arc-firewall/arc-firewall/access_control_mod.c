/*
 * This module handles access control, url blocking.
 */

#include "firewall.h"
#include "mod_common.h"

#define AC_MODULE_RULES (&(access_control_mod.mod_rules))
#define AC_MODULE_CHAINS (&(access_control_mod.mod_chains))

static void rules_in_conntrack_tb();
static void rules_in_filter_tb();
extern void cleanup_conntrack_by_mac(char* mac);

module_t access_control_mod = {
	.module_name = "access_control_mod",
	.module_desc = "handle access control function",
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
	.nat_tb_routine = NULL,
	.mangle_tb_routine = NULL
};

static void rules_in_conntrack_tb()
{
	char glbcfg[128], mac[32], hour_mask[32];
	int rule_num, i;
	time_t current_time;
	struct tm *tm_cur;

	/* When system mode is bridge or wb, do nothing. */
	if(mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "bridge") || (mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") && mapi_ccfg_match_str(tid, "ARC_WAN_0_BridgeEnable", "1")) || mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "wb"))
		return;

	if(mapi_ccfg_match_str(tid, "ARC_FIREWALL_AC_Enable", "1"))
	{
		/* get current time */
		current_time = time(NULL);
		tm_cur = localtime(&current_time);

		//cprintf("curent local time hour [%d], min [%d], sec [%d]\n", tm_cur->tm_hour, tm_cur->tm_min, tm_cur->tm_sec);

		rule_num = atoi(mapi_ccfg_get_str(tid, "ARC_FIREWALL_AC_RULE_Num", glbcfg, sizeof(glbcfg)));

		for(i = 0; i < rule_num; i++)
		{
			snprintf(glbcfg, sizeof(glbcfg), "ARC_FIREWALL_AC_RULE_%d_MAC", i);
			mapi_ccfg_get_str(tid, glbcfg, mac, sizeof(mac));

			snprintf(glbcfg, sizeof(glbcfg), "ARC_FIREWALL_AC_RULE_%d_DAY_%d_HourMask", i, tm_cur->tm_wday);
			mapi_ccfg_get_str(tid, glbcfg, hour_mask, sizeof(hour_mask));

			//cprintf("get var [%s] value [%s], hour[%d],  bit value[%c]\n", glbcfg, hour_mask, tm_cur->tm_hour, hour_mask[tm_cur->tm_hour]);

			/* no internet access in this hour */
			if(hour_mask[tm_cur->tm_hour] == '0')
			{
				//cprintf("cleanup_conntrack_by_mac [%s]\n", mac);
				cleanup_conntrack_by_mac(mac);
			}
		}
	}

	return;
}

static void rules_in_filter_tb()
{
	char buf[128] = "", glbcfg[128] = "", source_mac[32] = "", lan1_ifname[10] = "", hour_mask[32] = "";
	int rule_num;
	int i;
	
	time_t current_time;
	struct tm *tm_cur;
	time_t interval;

	/* When system mode is bridge or wb, do nothing. */
	if(mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "bridge") || (mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") && mapi_ccfg_match_str(tid, "ARC_WAN_0_BridgeEnable", "1")) || mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "wb"))
		return;

	if(mapi_ccfg_match_str(tid, "ARC_FIREWALL_AC_Enable", "1"))
	{
		append_chain(AC_MODULE_CHAINS, FILTER_TABLE, "AC-INPUT-CHAIN");
		append_chain(AC_MODULE_CHAINS, FILTER_TABLE, "AC-FORWARD-CHAIN");
		append_rule(AC_MODULE_RULES, FILTER_TABLE, "-A FORWARD -j AC-FORWARD-CHAIN\n");
		append_rule(AC_MODULE_RULES, FILTER_TABLE, "-A AC-FORWARD-CHAIN -i %s -o %s -j RETURN\n", lanface, lanface);

		if(mapi_ccfg_match_str(tid, "ARC_LAN_1_Enable", "1"))
		{
			mapi_ccfg_get_str(tid, "ARC_LAN_1_Ifname", lan1_ifname, sizeof(lan1_ifname));
			append_rule(AC_MODULE_RULES, FILTER_TABLE, "-A AC-FORWARD-CHAIN -i %s -j RETURN\n", lan1_ifname);
		}

		/* get current time */
		current_time = time(NULL);
		tm_cur = localtime(&current_time);

		//cprintf("curent local time hour [%d], min [%d], sec [%d]\n", tm_cur->tm_hour, tm_cur->tm_min, tm_cur->tm_sec);

		rule_num = atoi(mapi_ccfg_get_str(tid, "ARC_FIREWALL_AC_RULE_Num", buf, sizeof(buf)));
		
		for(i = 0; i< rule_num; i++)
		{	
			snprintf(glbcfg, sizeof(glbcfg), "ARC_FIREWALL_AC_RULE_%d_MAC", i);
			mapi_ccfg_get_str(tid, glbcfg, source_mac, sizeof(source_mac));

			snprintf(glbcfg, sizeof(glbcfg), "ARC_FIREWALL_AC_RULE_%d_DAY_%d_HourMask", i, tm_cur->tm_wday);
			mapi_ccfg_get_str(tid, glbcfg, hour_mask, sizeof(hour_mask));

			//cprintf("get var [%s] value [%s], hour[%d],  bit value[%c]\n", glbcfg, hour_mask, tm_cur->tm_hour, hour_mask[tm_cur->tm_hour]);

			/* no internet access in this hour */
			if(hour_mask[tm_cur->tm_hour] == '0')
			{
				append_rule(AC_MODULE_RULES, FILTER_TABLE, "-A AC-FORWARD-CHAIN -m mac --mac-source %s -j DROP\n", source_mac);
				if(mapi_ccfg_match_str(tid, "ARC_IFILTER_PRIVATE_UseName", "1"))
				{
					append_rule(AC_MODULE_RULES, FILTER_TABLE, "-A AC-INPUT-CHAIN -m mac --mac-source %s -p tcp -m tcp --dport 8080 -j DROP\n", source_mac);
				}
			}
		}

		/* update timer every hour, on the hour */
		interval = 3600;
		interval -= (tm_cur->tm_sec + tm_cur->tm_min*60);

		//cprintf("update_timer %ld seconds later\n", interval);
		
		update_timer(interval);

	}

}
