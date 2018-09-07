/*
 * This module handles virtual server/port trigger/DMZ function.
 */

#include "firewall.h"
#include "mod_common.h"

#define FORWARDING_MODULE_RULES (&(forwarding_mod.mod_rules))
#define FORWARDING_MODULE_CHAINS (&(forwarding_mod.mod_chains))

static void port_forwarding_rules_in_conntrack_tb();
static void forwarding_rules_in_filter_tb();
static void forwarding_rules_in_nat_tb();


module_t forwarding_mod = {
	.module_name = "forwarding_mod",
	.module_desc = "handle port forwarding",
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
	.conntrack_tb_routine = &port_forwarding_rules_in_conntrack_tb,
	.filter_tb_routine = &forwarding_rules_in_filter_tb,
	.nat_tb_routine = &forwarding_rules_in_nat_tb,
	.mangle_tb_routine = NULL
};

/*
 * Generate iptables' rules for virtual server in table filter.
 */
static void create_port_forward_in_filter_tb()
{
	char buf[128];
	char glbcfg[128];
	int max_num;
	int i;

	max_num = atoi(mapi_ccfg_get_str(tid, "ARC_NAT_PORTMAP_MaxNum", buf, sizeof(buf)));

	append_chain(FORWARDING_MODULE_CHAINS, FILTER_TABLE, "NAT-GROUP-FORWARD-CHAIN");
	append_rule(FORWARDING_MODULE_RULES, FILTER_TABLE, "-A FORWARD -j NAT-GROUP-FORWARD-CHAIN\n");

	for (i=0; i<max_num; i++)
	{
		int enable;
		char ip[16];
		char proto[16];
		int private_port_start;
		int private_port_end;
		int public_port_start;
		int public_port_end;
		int privateip[4];
		int netmask[4];
		int lanip[4];
		char p_ip[16];
		char mask[16];
		int j;
		char public_ip[16];
		int public_ip_type;

		snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_Enable", i);
		enable = atoi(mapi_ccfg_get_str(tid, glbcfg, buf, sizeof(buf)));

		if (!enable)
			continue;

		snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_PublicIPType", i);
		public_ip_type = atoi(mapi_ccfg_get_str(tid, glbcfg, buf, sizeof(buf)));
		if(public_ip_type >=2 && public_ip_type <=6)	//PPPoE IP address
		{
			//public type 2~6 correspond to PPPoE0~4
			snprintf(glbcfg, sizeof(glbcfg), "ARC_WAN_%d_PPP_GET_IP4_Addr", (public_ip_type -2));
			mapi_ccfg_get_str(tid, glbcfg, public_ip, sizeof(public_ip));

			//skip this rule, if PPPoE doesn't get address
			if((strcmp(public_ip, "0.0.0.0") == 0) || strlen(public_ip) == 0)
			{
				continue;
			}

		}

		snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_PrivateIP", i);
		mapi_ccfg_get_str(tid, glbcfg, p_ip, sizeof(p_ip));

		snprintf(glbcfg, sizeof(glbcfg), "ARC_LAN_0_IP4_Netmask");
		mapi_ccfg_get_str(tid, glbcfg, mask, sizeof(mask));

		sscanf(p_ip, "%d.%d.%d.%d", &privateip[0], &privateip[1], &privateip[2], &privateip[3]);
		sscanf(mask, "%d.%d.%d.%d", &netmask[0], &netmask[1], &netmask[2], &netmask[3]);
		sscanf(lanaddr, "%d.%d.%d.%d", &lanip[0], &lanip[1], &lanip[2], &lanip[3]);

		for (j=0; j<4; j++)
		{
			if (netmask[j] == 255)
				privateip[j] = lanip[j];
			else
				break;
		}
		snprintf(ip, sizeof(ip), "%d.%d.%d.%d", privateip[0], privateip[1], privateip[2], privateip[3]);

		snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_Proto", i);
		mapi_ccfg_get_str(tid, glbcfg, proto, sizeof(proto));

		snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_PublicPortStart", i);
		public_port_start = atoi(mapi_ccfg_get_str(tid, glbcfg, buf, sizeof(buf)));

		snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_PublicPortEnd", i);
		public_port_end = atoi(mapi_ccfg_get_str(tid, glbcfg, buf, sizeof(buf)));

		snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_PrivatePortStart", i);
		private_port_start = atoi(mapi_ccfg_get_str(tid, glbcfg, buf, sizeof(buf)));
		if(private_port_start == 0)
		{
			private_port_start = public_port_start; // leave blank means the same as public port
		}

		snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_PrivatePortEnd", i);
		private_port_end = atoi(mapi_ccfg_get_str(tid, glbcfg, buf, sizeof(buf)));
		if(private_port_end == 0)
		{
			private_port_end = public_port_end;	// leave blank means the same as public port
		}

		if ((strcmp(proto, "tcp") == 0) || (strcmp(proto, "udp") == 0))
		{

			append_rule(FORWARDING_MODULE_RULES, FILTER_TABLE, "-A NAT-GROUP-FORWARD-CHAIN -p %s -d %s --dport %d:%d -j ACCEPT\n", 
				proto, ip, private_port_start, private_port_end);
		}
		else
		{
			append_rule(FORWARDING_MODULE_RULES, FILTER_TABLE, "-A NAT-GROUP-FORWARD-CHAIN -p %s -d %s -j ACCEPT\n",
				proto, ip);
		}
	}
}

static void port_forwarding_rules_in_conntrack_tb(void)
{
	char buf[128];
	char glbcfg[128];
	int max_num;
	int i;

	/* When system mode is bridge or wb, do nothing. */
	if(mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "bridge") || (mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") && mapi_ccfg_match_str(tid, "ARC_WAN_0_BridgeEnable", "1")) || mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "wb"))
		return;

	module_conntrack_t new_conntrack_list = {
		.conntrack_rules_head = NULL,
		.conntrack_rules_tail = NULL,
	};

	max_num = atoi(mapi_ccfg_get_str(tid, "ARC_NAT_PORTMAP_MaxNum", buf, sizeof(buf)));

	if (!natenable)
		return;

	for (i=0; i<max_num; i++)
	{
		int enable;
		char ip[16];
		char proto[16];
		int private_port_start;
		int private_port_end;
		int public_port_start;
		int public_port_end;
		int privateip[4];
		int netmask[4];
		int lanip[4];
		char p_ip[16];
		char mask[16];
		int j;

		conntrack_info_t conntrack_data;

		snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_Enable", i);
		enable = atoi(mapi_ccfg_get_str(tid, glbcfg, buf, sizeof(buf)));

		if (!enable)
			continue;

		snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_PrivateIP", i);
		mapi_ccfg_get_str(tid, glbcfg, p_ip, sizeof(p_ip));

		snprintf(glbcfg, sizeof(glbcfg), "ARC_LAN_0_IP4_Netmask");
		mapi_ccfg_get_str(tid, glbcfg, mask, sizeof(mask));

		sscanf(p_ip, "%d.%d.%d.%d", &privateip[0], &privateip[1], &privateip[2], &privateip[3]);
		sscanf(mask, "%d.%d.%d.%d", &netmask[0], &netmask[1], &netmask[2], &netmask[3]);
		sscanf(lanaddr, "%d.%d.%d.%d", &lanip[0], &lanip[1], &lanip[2], &lanip[3]);

		for (j=0; j<4; j++)
		{
			if (netmask[j] == 255)
				privateip[j] = lanip[j];
			else
				break;
		}
		snprintf(ip, sizeof(ip), "%d.%d.%d.%d", privateip[0], privateip[1], privateip[2], privateip[3]);

		snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_Proto", i);
		mapi_ccfg_get_str(tid, glbcfg, proto, sizeof(proto));

		snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_PublicPortStart", i);
		public_port_start = atoi(mapi_ccfg_get_str(tid, glbcfg, buf, sizeof(buf)));

		snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_PublicPortEnd", i);
		public_port_end = atoi(mapi_ccfg_get_str(tid, glbcfg, buf, sizeof(buf)));

		snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_PrivatePortStart", i);
		private_port_start = atoi(mapi_ccfg_get_str(tid, glbcfg, buf, sizeof(buf)));
		if(private_port_start == 0)
		{
			private_port_start = public_port_start; // leave blank means the same as public port
		}

		snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_PrivatePortEnd", i);
		private_port_end = atoi(mapi_ccfg_get_str(tid, glbcfg, buf, sizeof(buf)));
		if(private_port_end == 0)
		{
			private_port_end = public_port_end;	// leave blank means the same as public port
		}

		conntrack_data.orig_port_src = 0;
		conntrack_data.orig_src = 0;
		conntrack_data.orig_dst = 0;
		conntrack_data.reply_src = inet_addr(ip);
		conntrack_data.reply_dst = 0;
		conntrack_data.reply_port_dst = 0;

		conntrack_data.orig_port_dst = public_port_start;
		conntrack_data.orig_port_dst_range = public_port_end - public_port_start;
		conntrack_data.reply_port_src = private_port_start;
		conntrack_data.reply_port_src_range = private_port_end - private_port_start;

		if (!strcmp(proto, "tcp"))
		{
			conntrack_data.proto = 6;
			add_conntrack_rule(&new_conntrack_list, conntrack_data);
		}
		else if (!strcmp(proto, "udp"))
		{
			conntrack_data.proto = 17;
			add_conntrack_rule(&new_conntrack_list, conntrack_data);
		}
		else if (!strcmp(proto, "both"))
		{
			conntrack_data.proto = 6;
			add_conntrack_rule(&new_conntrack_list, conntrack_data);
			conntrack_data.proto = 17;
			add_conntrack_rule(&new_conntrack_list, conntrack_data);
		}
		else
		{
			conntrack_data.proto = 0;
			add_conntrack_rule(&new_conntrack_list, conntrack_data);
		}
	}

	/* DMZ */
	if (dmzenable)
	{
		char dmz_dst_ip[16];

		mapi_ccfg_get_str(tid, "ARC_FIREWALL_DMZ_0_HostIP", dmz_dst_ip, sizeof(dmz_dst_ip));
		conntrack_info_t conntrack_data;

		conntrack_data.orig_port_src = 0;
		conntrack_data.orig_port_dst = 0;
		conntrack_data.orig_src = 0;
		conntrack_data.orig_dst = 0;
		conntrack_data.reply_src = inet_addr(dmz_dst_ip);
		conntrack_data.reply_dst = 0;
		conntrack_data.reply_port_src = 0;
		conntrack_data.reply_port_dst = 0;
		conntrack_data.proto = 0;
		conntrack_data.orig_port_dst_range = 0;
		conntrack_data.reply_port_src_range = 0;

		add_conntrack_rule(&new_conntrack_list, conntrack_data);
	}

	compare_and_clean_conntrack(&(forwarding_mod.mod_conntrack), &new_conntrack_list);
}

/*
 * Generate iptables' rules for virtual server in table nat.
 */
static void create_port_forward_in_nat_tb()
{
	char buf[128];
	char glbcfg[128];
	int max_num;
	int i;

	struct groups{
		char name[64];
	};
	struct groups *all_groups;
	int group_count =0;
	
	max_num = atoi(mapi_ccfg_get_str(tid, "ARC_NAT_PORTMAP_MaxNum", buf, sizeof(buf)));

	if(max_num ==0)
		return;
	all_groups = malloc(sizeof(struct groups)*max_num);
	if(all_groups == NULL)
	{
		cprintf("out of memory to store groups\n");
		return;
	}
	bzero(all_groups, sizeof(struct groups)*max_num);

	for (i=0; i<max_num; i++)
	{
		int enable;
		char ip[16];
		char proto[16];
		int private_port_start;
		int private_port_end;
		int public_port_start;
		int public_port_end;
		int privateip[4];
		int netmask[4];
		int lanip[4];
		char p_ip[16];
		char mask[16];
		int j;
		char group_name[16];
		char chain_name[64];
		char public_ip[16];
		int public_ip_type;

		snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_Enable", i);
		enable = atoi(mapi_ccfg_get_str(tid, glbcfg, buf, sizeof(buf)));

		if (!enable)
			continue;

		/*Add chain & entry if the group is new */
		snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_GroupName", i);
		mapi_ccfg_get_str(tid, glbcfg, group_name, sizeof(group_name));
		snprintf(chain_name, sizeof(chain_name), "NAT-GROUP-%s", group_name);
		for(j=0; j<group_count; j++)
		{
			if(strcmp(all_groups[j].name, group_name) == 0)
				break;
		}
		if(j>=group_count)
		{
			//new group found, record it then append chain
			strncpy(all_groups[group_count].name, group_name, sizeof(all_groups[group_count].name));
			group_count++;
			append_chain(FORWARDING_MODULE_CHAINS, NAT_TABLE, chain_name);
			append_rule(FORWARDING_MODULE_RULES, NAT_TABLE, "-A PREROUTING -j %s\n", chain_name);
		}

		/* get Public IP */
		snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_PublicIPType", i);
		public_ip_type = atoi(mapi_ccfg_get_str(tid, glbcfg, buf, sizeof(buf)));
		if(public_ip_type == 1)	//default WAN IP address
		{
			strncpy(public_ip, wanaddr, sizeof(public_ip));
		}
		else if(public_ip_type == 0)  //Manually set WAN IP address
		{
			snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_PublicIP", i);
			mapi_ccfg_get_str(tid, glbcfg, public_ip, sizeof(public_ip));
		}else if(public_ip_type >=2 && public_ip_type <=6)	//PPPoE IP address
		{
			//public type 2~6 correspond to PPPoE0~4
			snprintf(glbcfg, sizeof(glbcfg), "ARC_WAN_%d_PPP_GET_IP4_Addr", (public_ip_type -2));
			mapi_ccfg_get_str(tid, glbcfg, public_ip, sizeof(public_ip));

			//skip this rule, if PPPoE doesn't get address
			if((strcmp(public_ip, "0.0.0.0") == 0) || strlen(public_ip) == 0)
			{
				continue;
			}

		}

		snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_PrivateIP", i);
		mapi_ccfg_get_str(tid, glbcfg, p_ip, sizeof(p_ip));

		snprintf(glbcfg, sizeof(glbcfg), "ARC_LAN_0_IP4_Netmask");
		mapi_ccfg_get_str(tid, glbcfg, mask, sizeof(mask));

		sscanf(p_ip, "%d.%d.%d.%d", &privateip[0], &privateip[1], &privateip[2], &privateip[3]);
		sscanf(mask, "%d.%d.%d.%d", &netmask[0], &netmask[1], &netmask[2], &netmask[3]);
		sscanf(lanaddr, "%d.%d.%d.%d", &lanip[0], &lanip[1], &lanip[2], &lanip[3]);

		/* allow 0.0.0.1 and 233.255.255.254 set to lanip for ticket#7147
		for (j=0; j<4; j++)
		{
			if (netmask[j] == 255)
				privateip[j] = lanip[j];
			else
				break;
		}
		*/
		snprintf(ip, sizeof(ip), "%d.%d.%d.%d", privateip[0], privateip[1], privateip[2], privateip[3]);


		snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_Proto", i);
		mapi_ccfg_get_str(tid, glbcfg, proto, sizeof(proto));


		snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_PublicPortStart", i);
		public_port_start = atoi(mapi_ccfg_get_str(tid, glbcfg, buf, sizeof(buf)));

		snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_PublicPortEnd", i);
		public_port_end = atoi(mapi_ccfg_get_str(tid, glbcfg, buf, sizeof(buf)));
		
		snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_PrivatePortStart", i);
		private_port_start = atoi(mapi_ccfg_get_str(tid, glbcfg, buf, sizeof(buf)));
		if(private_port_start == 0)
		{
			private_port_start = public_port_start; // leave blank means the same as public port
		}

		snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_PrivatePortEnd", i);
		private_port_end = atoi(mapi_ccfg_get_str(tid, glbcfg, buf, sizeof(buf)));
		if(private_port_end == 0)
		{
			private_port_end = public_port_end;
		}

		if ((strcmp(proto, "tcp") == 0) || (strcmp(proto, "udp") == 0))
		{
			append_rule(FORWARDING_MODULE_RULES, NAT_TABLE, "-A %s -p %s -d %s --dport %d:%d -j DNAT --to-destination %s:%d-%d\n", 
				chain_name, proto, public_ip, public_port_start, public_port_end, ip, private_port_start, private_port_end);
		}
		else
		{
			append_rule(FORWARDING_MODULE_RULES, NAT_TABLE, "-A %s -p %s -d %s -j DNAT --to-destination %s\n",
				chain_name, proto, public_ip, ip);
				
		}
	}

	free(all_groups);
}

/* Can add IP address of WAN port manaully when add port forwarding rules */
#define MAX_FORWARDING_RULES 32
static void add_wan_ip_port_forwarding()
{
	char glbcfg[128];
	char public_ip[16];
	char wan_addrs[MAX_FORWARDING_RULES][16];
	int count = 0;
	int i, j;
	int ret = 0;
	char cmd[256];
	char mask[16];

	sprintf(glbcfg, "ARC_WAN_%d_IP4_Proto", dft_wan_idx);

	if(mapi_ccfg_match_str(tid, glbcfg, "pptp") || mapi_ccfg_match_str(tid, glbcfg, "l2tp") || mapi_ccfg_match_str(tid, glbcfg, "pppoe"))
		sprintf(glbcfg, "ARC_WAN_%d_PPP_GET_IP4_Netmask", dft_wan_idx);
	else if(mapi_ccfg_match_str(tid, glbcfg, "dhcp"))
		sprintf(glbcfg, "ARC_WAN_%d_DHCP_IP4_Netmask", dft_wan_idx);
	else
		sprintf(glbcfg, "ARC_WAN_%d_IP4_Netmask", dft_wan_idx);

	mapi_ccfg_get_str(tid, glbcfg , mask, sizeof(mask));
	memset(glbcfg, 0, sizeof(glbcfg));

	/* Ticket #7959 support MAP-E by seal */
	sprintf(glbcfg, "ARC_WAN_%d_IP4_Proto", dft_wan_idx);
	if(mapi_ccfg_match_str(tid, glbcfg, "v6plus"))
		strncpy(mask, "255.255.255.255", sizeof(mask));

	bzero(wan_addrs, sizeof(wan_addrs));

	for(i=0; i<MAX_FORWARDING_RULES; i++){

		//ignore not enabled rules
		snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_Enable", i);
		if(mapi_ccfg_match_str(tid, glbcfg, "0"))
			continue;

		//wan ip address is set manually
		snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_PublicIPType", i);
		if(mapi_ccfg_match_str(tid, glbcfg, "0")){
			snprintf(glbcfg, sizeof(glbcfg), "ARC_NAT_PORTMAP_%d_PublicIP", i);
			mapi_ccfg_get_str(tid, glbcfg, public_ip, sizeof(public_ip));
		}
		else
			continue;

		//same ip address of wanaddr
		if(strcmp(public_ip, wanaddr) == 0){
			//ARCA_LOG((LOG_DEBUG, "%d public ip same with wan_addr [%s], ignore\n", i, public_ip));
			continue;
		}

		//check if the public_ip is duplicated
		for(j=0; j<count; j++){
			if(strcmp(public_ip, wan_addrs[j]) == 0){
				//ARCA_LOG((LOG_DEBUG, "public_ip [%s] duplicated, ignore it\n", public_ip));
				break;;
			}
		}
		//record the new public_ip
		if(j >=count){
			strncpy(wan_addrs[count], public_ip, MAX_FORWARDING_RULES);
			//ARCA_LOG((LOG_DEBUG, "add ip [%s] in index[%d]\n", public_ip, count));
			count++;
		}
	}

	//Add new IP addresses to wan port
	for(i=0; i<count; i++){
		snprintf(cmd ,sizeof(cmd), "ifconfig %s:%02d %s netmask %s", wanface, i, wan_addrs[i], mask);
		ret = system(cmd);
		//ARCA_LOG((LOG_DEBUG, "command is [%s], ret=%d\n", cmd, ret));
	}

	//Clear unused IP addresses
	for(i=count; i<MAX_FORWARDING_RULES; i++){
		snprintf(cmd, sizeof(cmd), "%s:%02d", wanface, i);
		snprintf(cmd, sizeof(cmd), "ifconfig %s:%02d down", wanface, i);
		ret = system(cmd);
		//ARCA_LOG((LOG_DEBUG, "command is [%s], ret=%d\n", cmd, ret));
		//no need to try anymore when error ocurrs.
		if(ret != 0)
			break;
	}

	return;
}

/*
 * Generate iptables' rules for DMZ settings in table filter.
 */
void create_dmz_rule_in_filter_tb(void)
{
	char dmz_dst_ip[20];

	mapi_ccfg_get_str(tid, "ARC_FIREWALL_DMZ_0_HostIP", dmz_dst_ip, sizeof(dmz_dst_ip));

	append_chain(FORWARDING_MODULE_CHAINS, FILTER_TABLE, "NAT-GROUP-DMZ-CHAIN");

	append_rule(FORWARDING_MODULE_RULES, FILTER_TABLE, "-A FORWARD -j NAT-GROUP-DMZ-CHAIN \n");
	append_rule(FORWARDING_MODULE_RULES, FILTER_TABLE, "-A NAT-GROUP-DMZ-CHAIN -d %s -p udp --dport 520 -j DROP\n", dmz_dst_ip);
	append_rule(FORWARDING_MODULE_RULES, FILTER_TABLE, "-A NAT-GROUP-DMZ-CHAIN -d %s -p all	-j ACCEPT\n", dmz_dst_ip);
}

/*
 * Generate iptables' rules for DMZ settings in nat filter.
 */
void create_dmz_rule_in_nat_tb(void)
{
	int i;
	char glbcfg[128];
	char wanip[20];
	char dmz_dst_ip[20];

	mapi_ccfg_get_str(tid, "ARC_FIREWALL_DMZ_0_HostIP", dmz_dst_ip, sizeof(dmz_dst_ip));

	append_chain(FORWARDING_MODULE_CHAINS, NAT_TABLE, "NAT-GROUP-DMZ-CHAIN");

	append_rule(FORWARDING_MODULE_RULES, NAT_TABLE, "-A PREROUTING -j NAT-GROUP-DMZ-CHAIN \n");
	if ((mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") ||
		mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "router")) &&
		mapi_ccfg_match_str(tid, "ARC_WAN_0_IP4_Proto", "pppoe"))
	{
		for(i =0; i < PPPOE_MAX_COUNT; i++ )
		{
			snprintf(glbcfg, sizeof(glbcfg), "ARC_WAN_%d_PPPOE_Enable", i);
			if (mapi_ccfg_match_str(tid, glbcfg, "1"))
			{
				snprintf(glbcfg, sizeof(glbcfg), "ARC_WAN_%d_PPP_GET_IP4_Addr", i);
				mapi_ccfg_get_str(tid, glbcfg, wanip, sizeof(wanip));
				if(strcmp(wanip, "") && strcmp(wanip, "0.0.0.0"))
				{
					append_rule(FORWARDING_MODULE_RULES, NAT_TABLE, "-A NAT-GROUP-DMZ-CHAIN -p all -d %s -j DNAT --to %s\n", wanip, dmz_dst_ip);
				}
			}
		}
	}
	else
		append_rule(FORWARDING_MODULE_RULES, NAT_TABLE, "-A NAT-GROUP-DMZ-CHAIN -p all -d %s -j DNAT --to %s\n",wanaddr, dmz_dst_ip);
}

void create_pptp_pssthru_rule_in_filter_tb()
{

	if(!mapi_ccfg_match_str(tid, "ARC_SYS_PPTPPassthru", "1"))
	{

		append_chain(FORWARDING_MODULE_CHAINS, FILTER_TABLE, "PPTP_PTHROUGH_DROP");
		append_rule(FORWARDING_MODULE_RULES, FILTER_TABLE, "-A FORWARD -j PPTP_PTHROUGH_DROP\n");

		append_rule(FORWARDING_MODULE_RULES, FILTER_TABLE, "-A PPTP_PTHROUGH_DROP -o %s -p tcp -m tcp --dport %d -j DROP\n", wanface, PPTP_PORT);
	}
}

static void forwarding_rules_in_filter_tb()
{
	/* When system mode is bridge or wb, do nothing. */
	if(mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "bridge") || (mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") && mapi_ccfg_match_str(tid, "ARC_WAN_0_BridgeEnable", "1")) || mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "wb"))
		return;

	if (!natenable)
		return;

	// -A FORWARD -d 192.168.2.148/32 -p tcp -m tcp --dport 21 -j ACCEPT
	create_port_forward_in_filter_tb();

	if (dmzenable)
	{
		// -A FORWARD -d 192.168.2.148/32 -o br0 -j ACCEPT
		create_dmz_rule_in_filter_tb();
	}

	create_pptp_pssthru_rule_in_filter_tb();
}

static void forwarding_rules_in_nat_tb()
{
	/* When system mode is bridge or wb, do nothing. */
	if(mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "bridge") || (mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") && mapi_ccfg_match_str(tid, "ARC_WAN_0_BridgeEnable", "1")) || mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "wb"))
		return;

	if (!natenable)
		return;

	//FIXME: Should not configure WAN port in this deamon, but actually I can find any better place to do.
	add_wan_ip_port_forwarding();

	// -A PREROUTING -d [wanadd] -p tcp -m tcp --dport 21 -j DNAT --to-destination 192.168.2.148:21
	create_port_forward_in_nat_tb();

	if (dmzenable)
	{
		// -A PREROUTING -d 192.168.106.30/32 -j DNAT --to-destination 192.168.2.148
		create_dmz_rule_in_nat_tb();
	}
}



