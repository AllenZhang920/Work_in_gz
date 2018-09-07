/*
 * This module handles ipfilter function 
 */

#include "firewall.h"
#include "mod_common.h"

#define IPFILTER_MODULE_RULES (&(ipfilter_mod.mod_rules))
#define IPFILTER_MODULE_CHAINS (&(ipfilter_mod.mod_chains))

static void ipfilter_rules_in_filter_tb();

module_t ipfilter_mod = {
	.module_name = "ipfilter_mod",
	.module_desc = "handle ip filter",
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
	.filter_tb_routine = &ipfilter_rules_in_filter_tb,
	.nat_tb_routine = NULL,
	.mangle_tb_routine = NULL
};

/*
 * Generate iptables' rules for virtual server in table filter.
 */
static void ipfilter_rules_in_filter_tb()
{
	char buf[128];
	char glbcfg[128];
	int num;
	int i;

	/* When system mode is bridge or wb, do nothing. */
	if(mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "bridge") || (mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") && mapi_ccfg_match_str(tid, "ARC_WAN_0_BridgeEnable", "1")) || mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "wb"))
		return;

	num = atoi(mapi_ccfg_get_str(tid, "ARC_FIREWALL_IPFILTER_Num", buf, sizeof(buf)));

	append_chain(IPFILTER_MODULE_CHAINS, FILTER_TABLE, "IPFILTER-CHAIN-FORWARD");
	append_rule(IPFILTER_MODULE_RULES, FILTER_TABLE, "-A FORWARD -j IPFILTER-CHAIN-FORWARD\n");
	for (i=0; i<num; i++)
	{
		char proto[16];
		int dest_port_start;
		int dest_port_end;
		char source_ip[20];
		char dest_ip[20];
		char action[16];
		char dir[16];
		char source_face[IFNAMSIZ];
		char dest_face[IFNAMSIZ];
		char suffix[4];

		snprintf(glbcfg, sizeof(glbcfg), "ARC_FIREWALL_IPFILTER_RULE_%d_Action", i);
		mapi_ccfg_get_str(tid, glbcfg, action, sizeof(action));
		if(strcmp(action, "DROP") && strcmp(action, "REJECT") && strcmp(action, "ACCEPT"))
		{
			cprintf("unkown action [%s] for ipfilter rules [%d]\n", action, i);
			continue;
		}

		snprintf(glbcfg, sizeof(glbcfg), "ARC_FIREWALL_IPFILTER_RULE_%d_SourceIP", i);
		mapi_ccfg_get_str(tid, glbcfg, source_ip, sizeof(source_ip));
		if(strcmp(source_ip, "") == 0)
			strcpy(source_ip, "0.0.0.0/0"); //blank means all

		snprintf(glbcfg, sizeof(glbcfg), "ARC_FIREWALL_IPFILTER_RULE_%d_DestIP", i);
		mapi_ccfg_get_str(tid, glbcfg, dest_ip, sizeof(dest_ip));
		if(strcmp(dest_ip, "") == 0)
			strcpy(dest_ip, "0.0.0.0/0"); //blank means all

		snprintf(glbcfg, sizeof(glbcfg), "ARC_FIREWALL_IPFILTER_RULE_%d_Dir", i);
		mapi_ccfg_get_str(tid, glbcfg, dir, sizeof(dir));
		if(strcmp(dir, "WAN2LAN") == 0)
		{
			//change into cidr notation if network address assigned 
			if(is_network_addr(source_ip, wancidr) == 1)
			{
				snprintf(suffix ,sizeof(suffix), "/%d", wancidr);
				strcat(source_ip, suffix);
			}
			if(is_network_addr(dest_ip, lancidr) == 1)
			{
				snprintf(suffix ,sizeof(suffix), "/%d", lancidr);
				strcat(dest_ip, suffix);
			}

			strcpy(source_face, wanface);
			if ((mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") ||
				mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "router")) &&
				mapi_ccfg_match_str(tid, "ARC_WAN_0_IP4_Proto", "pppoe"))
			{
				snprintf(source_face, sizeof(source_face), "ppp+");
			}
			strcpy(dest_face, lanface);
		}
		else if(strcmp(dir, "LAN2WAN")==0)
		{
			//change into cidr notation if network address assigned 
			if(is_network_addr(source_ip, lancidr) == 1)
			{
				snprintf(suffix ,sizeof(suffix), "/%d", lancidr);
				strcat(source_ip, suffix);
			}
			if(is_network_addr(dest_ip, wancidr) == 1)
			{
				snprintf(suffix ,sizeof(suffix), "/%d", wancidr);
				strcat(dest_ip, suffix);
			}

			strcpy(source_face, lanface);
			strcpy(dest_face, wanface);
			if ((mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") ||
				mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "router")) &&
				mapi_ccfg_match_str(tid, "ARC_WAN_0_IP4_Proto", "pppoe"))
			{
				snprintf(dest_face, sizeof(dest_face), "ppp+");
			}
		}
		else
		{
			cprintf("unkown dir [%s] for ipfilter rules [%d]\n", dir, i);
			continue;
		}
		

		snprintf(glbcfg, sizeof(glbcfg), "ARC_FIREWALL_IPFILTER_RULE_%d_Proto", i);
		mapi_ccfg_get_str(tid, glbcfg, proto, sizeof(proto));

		snprintf(glbcfg, sizeof(glbcfg), "ARC_FIREWALL_IPFILTER_RULE_%d_DestPortStart", i);
		dest_port_start = atoi(mapi_ccfg_get_str(tid, glbcfg, buf, sizeof(buf)));

		snprintf(glbcfg, sizeof(glbcfg), "ARC_FIREWALL_IPFILTER_RULE_%d_DestPortEnd", i);
		dest_port_end = atoi(mapi_ccfg_get_str(tid, glbcfg, buf, sizeof(buf)));
	
	
		if ((strcmp(proto, "tcp") == 0) || (strcmp(proto, "udp") == 0))
		{
			if(mapi_ccfg_match_str(tid, "ARC_SYS_LOG_IPFilter_Enable", "1"))
			{
				append_rule(IPFILTER_MODULE_RULES, FILTER_TABLE, "-A IPFILTER-CHAIN-FORWARD -s %s -d %s -p %s --dport %d:%d -i %s -o %s -j ULOG --ulog-prefix \"%s :\"\n",
				source_ip, dest_ip, proto, dest_port_start, dest_port_end, source_face, dest_face, action);
			}
			append_rule(IPFILTER_MODULE_RULES, FILTER_TABLE, "-A IPFILTER-CHAIN-FORWARD -s %s -d %s -p %s --dport %d:%d -i %s -o %s -j %s\n",
				source_ip, dest_ip, proto, dest_port_start, dest_port_end, source_face, dest_face, action);
		}
		else
		{
			if(mapi_ccfg_match_str(tid, "ARC_SYS_LOG_IPFilter_Enable", "1"))
			{
				append_rule(IPFILTER_MODULE_RULES, FILTER_TABLE, "-A IPFILTER-CHAIN-FORWARD -s %s -d %s -p %s -i %s -o %s -j ULOG --ulog-prefix \"%s :\"\n",
				source_ip, dest_ip, proto, source_face, dest_face, action);
			}
			append_rule(IPFILTER_MODULE_RULES, FILTER_TABLE, "-A IPFILTER-CHAIN-FORWARD -s %s -d %s -p %s -i %s -o %s -j %s\n",
				source_ip, dest_ip, proto, source_face, dest_face, action);
		}

	}
}


