/*
 * This module handles UPNP rules.
 */

#include "firewall.h"
#include "mod_common.h"

#define IFILTER_MODULE_RULES (&(ifilter_mod.mod_rules))
#define IFILTER_MODULE_CHAINS (&(ifilter_mod.mod_chains))

static void rules_in_nat_tb();
static void rules_in_filter_tb();

module_t ifilter_mod = {
	.module_name = "ifilter_mod",
	.module_desc = "buffalo i-filter mode",
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
	.nat_tb_routine = rules_in_nat_tb,
	.mangle_tb_routine = NULL
};

//#include <arca_debug.h>

static void rules_in_nat_tb()
{
	char glbcfg[256];
	char value[256];
	int i, reg_mac_count;
	char lan_ip_str[32]="";
	char lan_mask_str[32]="";
    struct in_addr	lan_ipaddr, lan_netmask, lan_netaddr;
	
	//ARCA_LOG((LOG_DEBUG, "Enter %s\n", __FUNCTION__));

	//do nothing if ifilter not enabled
	if(!mapi_ccfg_match_str(tid, "ARC_IFILTER_PRIVATE_UseName", "1"))
		return;

	//append rules for LAN port
	append_chain(IFILTER_MODULE_CHAINS, NAT_TABLE, "I-FILTER-REDIRECT");

	append_rule(IFILTER_MODULE_RULES, NAT_TABLE, "-A PREROUTING -i %s -p tcp --dport 80 -j I-FILTER-REDIRECT\n", lanface);

	bzero(value, sizeof(value));
	reg_mac_count = atoi(mapi_ccfg_get_str(tid ,"ARC_IFILTER_PRIVATE_RegisterMacAddrCnt", value ,sizeof(value)));
	for(i = 0; i < reg_mac_count; i++) {
		snprintf(glbcfg, sizeof(glbcfg), "ARC_IFILTER_PRIVATE_RegisterMacAddr_%d", i);
		bzero(value ,sizeof(value));
		mapi_ccfg_get_str(tid, glbcfg, value, sizeof(value));
		if(strcmp(value, "") !=0 ){
			append_rule(IFILTER_MODULE_RULES, NAT_TABLE, "-A I-FILTER-REDIRECT -p tcp -m mac --mac-source %s -j ACCEPT\n", value);
		}
	}

	mapi_ccfg_get_str(tid, "ARC_LAN_0_IP4_Addr",lan_ip_str , sizeof(lan_ip_str));
	mapi_ccfg_get_str(tid, "ARC_LAN_0_IP4_Netmask", lan_mask_str, sizeof(lan_mask_str));
    inet_aton(lan_ip_str, &lan_ipaddr);
    inet_aton(lan_mask_str, &lan_netmask);
    lan_netaddr.s_addr = lan_ipaddr.s_addr & lan_netmask.s_addr;
	append_rule(IFILTER_MODULE_RULES, NAT_TABLE, "-A I-FILTER-REDIRECT -p tcp -d %s/%d -j ACCEPT\n",
			inet_ntoa(lan_netaddr), netmask_to_cidr(lan_mask_str) );

	append_rule(IFILTER_MODULE_RULES, NAT_TABLE, "-A I-FILTER-REDIRECT -p tcp -d 10.0.0.0/8 -j ACCEPT\n");
	append_rule(IFILTER_MODULE_RULES, NAT_TABLE, "-A I-FILTER-REDIRECT -p tcp -d 172.16.0.0/12 -j ACCEPT\n");
	append_rule(IFILTER_MODULE_RULES, NAT_TABLE, "-A I-FILTER-REDIRECT -p tcp -d 192.168.0.0/16 -j ACCEPT\n");

	append_rule(IFILTER_MODULE_RULES, NAT_TABLE, "-A I-FILTER-REDIRECT -p tcp --dport 80 -j REDIRECT --to-ports 8080\n");


	//append rules for guest network
	if(!mapi_ccfg_match_str(tid, "ARC_LAN_1_Enable", "1"))
		return;

	append_chain(IFILTER_MODULE_CHAINS, NAT_TABLE, "I-FILTER-REDIRECT-GUESTPORT");

	mapi_ccfg_get_str(tid, "ARC_LAN_1_Ifname", value, sizeof(value));
	append_rule(IFILTER_MODULE_RULES, NAT_TABLE, "-A PREROUTING -i %s -p tcp --dport 80 -j I-FILTER-REDIRECT-GUESTPORT\n", value);

	bzero(value, sizeof(value));
	reg_mac_count = atoi(mapi_ccfg_get_str(tid ,"ARC_IFILTER_PRIVATE_RegisterMacAddrCnt", value ,sizeof(value)));
	for(i = 0; i < reg_mac_count; i++) {
		snprintf(glbcfg, sizeof(glbcfg), "ARC_IFILTER_PRIVATE_RegisterMacAddr_%d", i);
		bzero(value ,sizeof(value));
		mapi_ccfg_get_str(tid, glbcfg, value, sizeof(value));
		if(strcmp(value, "") !=0 ){
			append_rule(IFILTER_MODULE_RULES, NAT_TABLE, "-A I-FILTER-REDIRECT-GUESTPORT -p tcp -m mac --mac-source %s -j ACCEPT\n", value);
		}
	}

	if(mapi_ccfg_match_str(tid, "ARC_LAN_1_DHCP4S_Type", "auto")){
		mapi_ccfg_get_str(tid, "ARC_LAN_1_IP4_DefaultAddr",lan_ip_str , sizeof(lan_ip_str));
		mapi_ccfg_get_str(tid, "ARC_LAN_1_IP4_DefaultNetmask", lan_mask_str, sizeof(lan_mask_str));
	}
	else{
		mapi_ccfg_get_str(tid, "ARC_LAN_1_IP4_Addr",lan_ip_str , sizeof(lan_ip_str));
		mapi_ccfg_get_str(tid, "ARC_LAN_1_IP4_Netmask", lan_mask_str, sizeof(lan_mask_str));
	}
    inet_aton(lan_ip_str, &lan_ipaddr);
    inet_aton(lan_mask_str, &lan_netmask);
    lan_netaddr.s_addr = lan_ipaddr.s_addr & lan_netmask.s_addr;
	append_rule(IFILTER_MODULE_RULES, NAT_TABLE, "-A I-FILTER-REDIRECT-GUESTPORT -p tcp -d %s/%d -j ACCEPT\n",
			inet_ntoa(lan_netaddr), netmask_to_cidr(lan_mask_str) );

	append_rule(IFILTER_MODULE_RULES, NAT_TABLE, "-A I-FILTER-REDIRECT-GUESTPORT -p tcp -d 10.0.0.0/8 -j ACCEPT\n");
	append_rule(IFILTER_MODULE_RULES, NAT_TABLE, "-A I-FILTER-REDIRECT-GUESTPORT -p tcp -d 172.16.0.0/12 -j ACCEPT\n");
	append_rule(IFILTER_MODULE_RULES, NAT_TABLE, "-A I-FILTER-REDIRECT-GUESTPORT -p tcp -d 192.168.0.0/16 -j ACCEPT\n");
	append_rule(IFILTER_MODULE_RULES, NAT_TABLE, "-A I-FILTER-REDIRECT-GUESTPORT -p tcp --dport 80 -j REDIRECT --to-ports 8085\n");

	return;
}


static void rules_in_filter_tb()
{
	char lan1_ifname[16]="";
	
	if(!mapi_ccfg_match_str(tid, "ARC_LAN_1_Enable", "1"))
		return;

	mapi_ccfg_get_str(tid, "ARC_LAN_1_Ifname", lan1_ifname, sizeof(lan1_ifname));

	append_rule(IFILTER_MODULE_RULES, FILTER_TABLE, "-A INPUT -i %s -p tcp -m tcp --dport 8085 -j ACCEPT\n", lan1_ifname);

	return;
}

