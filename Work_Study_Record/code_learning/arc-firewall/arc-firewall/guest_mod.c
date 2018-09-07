/*
 * This module handles guest wifi related rules.  */

#include "firewall.h"
#include "mod_common.h"

#define GUEST_MODULE_RULES (&(guest_mod.mod_rules))
#define GUEST_MODULE_CHAINS (&(guest_mod.mod_chains))

static void rules_in_filter_tb();
static void rules_in_nat_tb();
static void conntrack_routine();
extern void cleanup_conntrack_by_mac(char* mac);

module_t guest_mod = {
	.module_name = "guest_mod",
	.module_desc = "handle guest wifi related rules",
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
	.conntrack_tb_routine = conntrack_routine,
	.filter_tb_routine = rules_in_filter_tb,
	.nat_tb_routine = rules_in_nat_tb,
	.mangle_tb_routine = NULL
};

void restore_approved_list(int is_filter)
{
	char word[50], *stat, *mac, *next;
	char apv_list[512] = "";
	char cmd[100] = "";
	char lan1_addr[INET_ADDRSTRLEN] = "";
	mapi_tmp_get(tid, "TMP_GUEST_ApprovedList", apv_list, sizeof(apv_list));
	if(mapi_ccfg_match_str(tid, "ARC_LAN_1_DHCP4S_Type", "auto"))
	{
		mapi_ccfg_get_str(tid, "ARC_LAN_1_IP4_DefaultAddr", lan1_addr, sizeof(lan1_addr));
	}
	else
	{
		mapi_ccfg_get_str(tid, "ARC_LAN_1_IP4_Addr", lan1_addr, sizeof(lan1_addr));
	}
	
	foreach(word, apv_list, next)
	{       
		stat = word;
		mac = strsep(&stat, ".");
		//ARCA_LOG((LOG_DEBUG, "approval %s login time %s\n", mac, stat));

		if(!mac || !stat)
		{
			//ARCA_LOG((LOG_DEBUG, "wrong setting.\n"));
			break;
		}
		if(is_filter == 1)
		{
			memset(cmd, 0, sizeof(cmd));
			snprintf(cmd, sizeof(cmd), "-A guestaccept -m mac --mac-source %s -j RETURN\n", mac);
			append_rule(GUEST_MODULE_RULES, FILTER_TABLE, cmd); 

			memset(cmd, 0, sizeof(cmd));
			snprintf(cmd, sizeof(cmd), "-A guestinput -m mac --mac-source %s -d %s -p icmp -j ACCEPT\n", mac, lan1_addr);
			append_rule(GUEST_MODULE_RULES, FILTER_TABLE, cmd); 
		}
		else
		{
			memset(cmd, 0, sizeof(cmd));
			snprintf(cmd, sizeof(cmd), "-A guest2wan -m mac --mac-source %s -j RETURN\n", mac);
			append_rule(GUEST_MODULE_RULES, NAT_TABLE, cmd); 
		}
	}
	return;
}

static void rules_in_filter_tb()
{
	char guest_enable_buf[5] = "", lan0_ifname[10] = "", lan1_ifname[10] = "", auth_enable[5] = "";
	char lan0_addr[INET_ADDRSTRLEN] = "", lan1_addr[INET_ADDRSTRLEN] = "";
	char cmd[150] = "";
	char lan0_br_addr[64] = "";
	int lan0_br_cidr = 0;
	char tmp_lan0_br_addr[64] = "";
	int tmp_lan0_br_cidr = 0;

	/* get the glbcfg, if guest wifi enable, set the rules; or do nothing */
	mapi_ccfg_get_str(tid, "ARC_LAN_1_Enable", guest_enable_buf, sizeof(guest_enable_buf));
	mapi_ccfg_get_str(tid, "ARC_GUEST_Uesr_Auth_Enable", auth_enable, sizeof(auth_enable));
	mapi_ccfg_get_str(tid, "ARC_LAN_1_Ifname", lan1_ifname, sizeof(lan1_ifname));
	mapi_ccfg_get_str(tid, "ARC_LAN_0_Ifname", lan0_ifname, sizeof(lan0_ifname));
	mapi_ccfg_get_str(tid, "ARC_LAN_0_IP4_Addr", lan0_addr, sizeof(lan0_addr));
	if(mapi_ccfg_match_str(tid, "ARC_LAN_1_DHCP4S_Type", "auto"))
	{
		mapi_ccfg_get_str(tid, "ARC_LAN_1_IP4_DefaultAddr", lan1_addr, sizeof(lan1_addr));
	}
	else
	{
		mapi_ccfg_get_str(tid, "ARC_LAN_1_IP4_Addr", lan1_addr, sizeof(lan1_addr));
	}

	if (mapi_ccfg_match_str(tid, "ARC_LAN_0_IP4_BridgeProto", "static"))
	{
		mapi_ccfg_get_str(tid, "ARC_LAN_0_IP4_BridgeAddr", lan0_br_addr, sizeof(lan0_br_addr));
		mapi_ccfg_get_str(tid, "ARC_LAN_0_IP4_BridgeNetmask", cmd, sizeof(cmd));
		lan0_br_cidr = netmask_to_cidr(cmd);

		/* Ticket #7605 by seal 180525 */
		mapi_tmp_get(tid, "TMP_LAN_0_IP4_BridgeDHCPAddr", tmp_lan0_br_addr, sizeof(tmp_lan0_br_addr));
		memset(cmd, 0, sizeof(cmd));
		if (NULL != mapi_tmp_get(tid, "TMP_LAN_0_IP4_BridgeDHCPNetmask", cmd, sizeof(cmd)))
			tmp_lan0_br_cidr = netmask_to_cidr(cmd);
	}
	else
	{
		mapi_ccfg_get_str(tid, "ARC_LAN_0_IP4_BridgeDHCPAddr", lan0_br_addr, sizeof(lan0_br_addr));
		mapi_ccfg_get_str(tid, "ARC_LAN_0_IP4_BridgeDHCPNetmask", cmd, sizeof(cmd));
		lan0_br_cidr = netmask_to_cidr(cmd);
	}

	if (strcmp(guest_enable_buf, "1") == 0)
	{
	        append_chain(GUEST_MODULE_CHAINS, FILTER_TABLE, "guestaccept");

		// can use the router as dns server
		memset(cmd, 0, sizeof(cmd));
		snprintf(cmd, sizeof(cmd), "-A INPUT -i %s -p udp -m udp --dport 53 -j ACCEPT\n", lan1_ifname);
		append_rule(GUEST_MODULE_RULES, FILTER_TABLE, cmd); 

		memset(cmd, 0, sizeof(cmd));
		snprintf(cmd, sizeof(cmd), "-A INPUT -i %s -p udp -m udp --dport 67 -j ACCEPT\n", lan1_ifname);
		append_rule(GUEST_MODULE_RULES, FILTER_TABLE, cmd); 

		//if(strcmp(auth_enable, "1") == 0)
		{
			//port 51000 is also used by i-filter to show block html
			memset(cmd, 0, sizeof(cmd));
			snprintf(cmd, sizeof(cmd), "-A INPUT -i %s -p tcp -m tcp --dport 51000 -j ACCEPT\n", lan1_ifname);
			append_rule(GUEST_MODULE_RULES, FILTER_TABLE, cmd); 

			memset(cmd, 0, sizeof(cmd));
			snprintf(cmd, sizeof(cmd), "-A INPUT -i %s -p tcp -m tcp --dport 51001 -j ACCEPT\n", lan1_ifname);
			append_rule(GUEST_MODULE_RULES, FILTER_TABLE, cmd); 
		}

		append_chain(GUEST_MODULE_CHAINS, FILTER_TABLE, "guestinput");

		memset(cmd, 0, sizeof(cmd));
		snprintf(cmd, sizeof(cmd), "-A INPUT -i %s -j guestinput\n" , lan1_ifname);
		append_rule(GUEST_MODULE_RULES, FILTER_TABLE, cmd); 

		if(strcmp(auth_enable, "1") != 0)
		{
			memset(cmd, 0, sizeof(cmd));
			snprintf(cmd, sizeof(cmd), "-A guestinput -i %s -d %s -p icmp -j ACCEPT\n", lan1_ifname, lan1_addr);
			append_rule(GUEST_MODULE_RULES, FILTER_TABLE, cmd); 
		}

		if(mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "bridge") || (mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") && mapi_ccfg_match_str(tid, "ARC_WAN_0_BridgeEnable", "1")) || mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "wb"))
		{ 
			/* Added filter rules when system mode is bridge. */
			memset(cmd, 0, sizeof(cmd));
			snprintf(cmd, sizeof(cmd), "-A INPUT -i %s -j DROP\n", lan1_ifname);
			append_rule(GUEST_MODULE_RULES, FILTER_TABLE, cmd); 

			memset(cmd, 0, sizeof(cmd));
			snprintf(cmd, sizeof(cmd), "-A INPUT -i %s -d %s -j DROP\n", lan0_ifname, lan1_addr);
			append_rule(GUEST_MODULE_RULES, FILTER_TABLE, cmd); 

			memset(cmd, 0, sizeof(cmd));
			snprintf(cmd, sizeof(cmd), "-A FORWARD -i %s -o %s -d %s/%d -j DROP\n", lan1_ifname, lan0_ifname, lan0_br_addr, lan0_br_cidr);
			append_rule(GUEST_MODULE_RULES, FILTER_TABLE, cmd);

			/* Ticket #7605 by seal 180525 */
			if (mapi_ccfg_match_str(tid, "ARC_LAN_0_IP4_BridgeProto", "static") && strlen(tmp_lan0_br_addr) > 0)
			{
				memset(cmd, 0, sizeof(cmd));
				snprintf(cmd, sizeof(cmd), "-A FORWARD -i %s -o %s -d %s/%d -j DROP\n", lan1_ifname, lan0_ifname, tmp_lan0_br_addr, tmp_lan0_br_cidr);
				append_rule(GUEST_MODULE_RULES, FILTER_TABLE, cmd);
			}
		}
		else
		{
			memset(cmd, 0, sizeof(cmd));
			snprintf(cmd, sizeof(cmd), "-A INPUT -i %s -j LOG_DROP\n", lan1_ifname);
			append_rule(GUEST_MODULE_RULES, FILTER_TABLE, cmd); 

			memset(cmd, 0, sizeof(cmd));
			snprintf(cmd, sizeof(cmd), "-A INPUT -i %s -d %s -j LOG_DROP\n", lan0_ifname, lan1_addr);
			append_rule(GUEST_MODULE_RULES, FILTER_TABLE, cmd); 

			// forbid the guest nework to access the home network
			memset(cmd, 0, sizeof(cmd));
			snprintf(cmd, sizeof(cmd), "-A FORWARD -i %s -o %s -j LOG_DROP\n", lan1_ifname, lan0_ifname); 
			append_rule(GUEST_MODULE_RULES, FILTER_TABLE, cmd);

			memset(cmd, 0, sizeof(cmd));
			snprintf(cmd, sizeof(cmd), "-A FORWARD -i %s -o %s -j LOG_DROP\n", lan0_ifname, lan1_ifname); 
			append_rule(GUEST_MODULE_RULES, FILTER_TABLE, cmd);
		}

		memset(cmd, 0, sizeof(cmd));
		snprintf(cmd, sizeof(cmd), "-A FORWARD -i %s -p udp -m udp --dport 53 -j ACCEPT\n", lan1_ifname);
		append_rule(GUEST_MODULE_RULES, FILTER_TABLE, cmd); 

		memset(cmd, 0, sizeof(cmd));
		snprintf(cmd, sizeof(cmd), "-A FORWARD -i %s -j guestaccept\n" , lan1_ifname);
		append_rule(GUEST_MODULE_RULES, FILTER_TABLE, cmd); 

		memset(cmd, 0, sizeof(cmd));
		snprintf(cmd, sizeof(cmd), "-A FORWARD -i %s -m state --state NEW -j ACCEPT\n", lan1_ifname);
		append_rule(GUEST_MODULE_RULES, FILTER_TABLE, cmd); 
		if(mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "bridge") || (mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") && mapi_ccfg_match_str(tid, "ARC_WAN_0_BridgeEnable", "1")) || mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "wb"))
		{
			append_rule(GUEST_MODULE_RULES, FILTER_TABLE, "-A FORWARD -j DROP\n"); 
		}

		//Enable Guest User Authorization
		if(strcmp(auth_enable, "1") == 0)
		{
			restore_approved_list(1);
			append_rule(GUEST_MODULE_RULES, FILTER_TABLE, "-A guestaccept -j DROP\n");
		}
		else
		{
			append_rule(GUEST_MODULE_RULES, FILTER_TABLE, "-A guestaccept -j RETURN\n");
		}
	}
}

static void rules_in_nat_tb()
{
	char guest_enable_buf[5] = "", lan1_ifname[10] = "", auth_enable[5] = "";      
	char web_port[10] = "", lan1_addr[30] = "", web_https_port[10] = "";
	char cmd[150] = "";

	//ARCA_LOG((LOG_DEBUG, "Enter %s\n", __FUNCTION__));
	/* get the glbcfg, if guest wifi enable, set the rules; or do nothing */
	mapi_ccfg_get_str(tid, "ARC_LAN_1_Enable", guest_enable_buf, sizeof(guest_enable_buf));
	mapi_ccfg_get_str(tid, "ARC_GUEST_Uesr_Auth_Enable", auth_enable, sizeof(auth_enable));
	mapi_ccfg_get_str(tid, "ARC_GUEST_WebPort", web_port, sizeof(web_port));
	mapi_ccfg_get_str(tid, "ARC_GUEST_WebPort_Https", web_https_port, sizeof(web_https_port));
	mapi_ccfg_get_str(tid, "ARC_LAN_1_Ifname", lan1_ifname, sizeof(lan1_ifname));
	/*ten_zheng, 2014-05-16, add for choosing different IP according to guest mode is auto or manual.*/
	if(mapi_ccfg_match_str(tid, "ARC_LAN_1_DHCP4S_Type", "auto"))
	{
		mapi_ccfg_get_str(tid, "ARC_LAN_1_IP4_DefaultAddr", lan1_addr, sizeof(lan1_addr));
	}
	else
	{
		mapi_ccfg_get_str(tid, "ARC_LAN_1_IP4_Addr", lan1_addr, sizeof(lan1_addr));
	}

	//ARCA_LOG((LOG_DEBUG, "Enter %s\n", __FUNCTION__));

	if (strcmp(guest_enable_buf, "1") == 0)
	{
		append_chain(GUEST_MODULE_CHAINS, NAT_TABLE, "guest2wan");

		memset(cmd, 0, sizeof(cmd));
		snprintf(cmd, sizeof(cmd), "-A PREROUTING -i %s -j guest2wan\n", lan1_ifname);
		append_rule(GUEST_MODULE_RULES, NAT_TABLE, cmd);

		if(mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "bridge") || (mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") && mapi_ccfg_match_str(tid, "ARC_WAN_0_BridgeEnable", "1")) || mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "wb"))
		{
			/* When system is bridge or wb, do nothing. */
		}
		else
		{
			memset(cmd, 0, sizeof(cmd));
			snprintf(cmd, sizeof(cmd), "-A PREROUTING -i %s -p udp -m udp --dport 53 -j ACCEPT\n", lan1_ifname);
			append_rule(GUEST_MODULE_RULES, NAT_TABLE, cmd);
		}
		
		if(strcmp(auth_enable, "1") == 0)
		{
			restore_approved_list(0);
			memset(cmd, 0, sizeof(cmd));
			snprintf(cmd, sizeof(cmd), "-A guest2wan -p tcp --dport 80 -j DNAT --to-destination %s:%s\n", lan1_addr, web_port);
			append_rule(GUEST_MODULE_RULES, NAT_TABLE, cmd);

			memset(cmd, 0, sizeof(cmd));
			snprintf(cmd, sizeof(cmd), "-A guest2wan -p tcp --dport 443 -j DNAT --to-destination %s:%s\n", lan1_addr, web_https_port);
			append_rule(GUEST_MODULE_RULES, NAT_TABLE, cmd);
		}
		else 
			append_rule(GUEST_MODULE_RULES, NAT_TABLE, "-A guest2wan -j RETURN\n");
	}
	else
	{
		mapi_tmp_set(tid, "TMP_GUEST_ApprovedList", "");
	}
}

void conntrack_routine()
{
	char buf[32];
	char ccfg_name[64];
	int max_user = 0;
	int i;
	char mac_list[500] = "";
	char mac[20], *next;

	//ARCA_LOG((LOG_DEBUG, "clean up ip contrack...\n"));

	bzero(buf, sizeof(buf));
	max_user = atoi(mapi_ccfg_get_str(tid, "ARC_GUEST_MaxUser", buf, sizeof(buf)));

	for(i=1; i<= max_user; i++)
	{
		snprintf(ccfg_name, sizeof(ccfg_name), "ARC_GUEST_%d_Enable", i);
		bzero(buf, sizeof(buf));
		mapi_ccfg_get_str(tid, ccfg_name, buf, sizeof(buf));
		//ARCA_LOG((LOG_DEBUG, "ccfg_name [%s], value [%s]\n", ccfg_name, buf));

		//this account is turned off.  clean up  MACs and related IP conntrack
		if(strcmp(buf, "0") == 0)
		{
			bzero(mac_list ,sizeof(mac_list));
			snprintf(ccfg_name, sizeof(ccfg_name), "TMP_GUEST_%d_Mac", i);
			mapi_tmp_get(tid, ccfg_name, mac_list, sizeof(mac_list));
			//ARCA_LOG((LOG_DEBUG, "ccfg_name [%s], value [%s]\n", ccfg_name, mac_list));
			if( strcmp(mac_list, "") )
			{
				//clean up related ipconntrack
				foreach(mac, mac_list, next)
				{
					//ARCA_LOG((LOG_DEBUG, "clean ipconntrack of mac [%s]\n" ,mac));
					cleanup_conntrack_by_mac(mac);
				}

				//clean up MACs
				mapi_tmp_set(tid, ccfg_name ,"");
			}
		}
	}
	
	return;
}


