#include "firewall.h"

#define IPTABLES_SAVE_FILE  "/tmp/.ipt"
// 1. The variable is stored in temp gblcfg.
// 2. It is written to module name by firewall agent, and indicates arc_firewalld that 
//    rules of the module need to update.
// 3. As soon as arc_firewalld updates the iptables rules, it is written to "" which indicates
//    the firewall agent that arc_firewalld has update the iptables rules.
#define VAR_FIREWALL_MODULE_NAME "VAR_FIREWALL_MODULE_NAME"

// place module object here
extern module_t generic_mod;
extern module_t forwarding_mod;
extern module_t access_control_mod;
extern module_t remote_management_mod;
extern module_t state_packet_inspect_pre_mod;
extern module_t state_packet_inspect_post_mod;
extern module_t upnp_mod;
extern module_t log_mod;
extern module_t guest_mod;
extern module_t vpn_pre_mod;
extern module_t vpn_post_mod;
extern module_t simple_mod;
extern module_t ipfilter_mod;
extern module_t state_packet_inspect_mid_mod;
extern module_t ifilter_mod;

static time_t current_alarm = -1;

module_t* mod_seq_in_filter_tb[] =
{
	&vpn_pre_mod,
	&state_packet_inspect_pre_mod,
	&generic_mod,
	&ipfilter_mod,
	&access_control_mod,
	&state_packet_inspect_mid_mod,
	&simple_mod,
	&forwarding_mod,
	&upnp_mod,
	&remote_management_mod,
	&ifilter_mod,
	&guest_mod,
	&state_packet_inspect_post_mod,
	&vpn_post_mod,
	&log_mod,
	NULL
};


module_t* mod_seq_in_nat_tb[] =
{
	&generic_mod,
	&vpn_pre_mod,
	&upnp_mod,
	&guest_mod,
	&ifilter_mod,
	&remote_management_mod,
	&forwarding_mod,
	NULL
};

module_t* mod_seq_in_mangle_tb[] =
{
	NULL
};

module_t* mod_seq_in_conntrack_tb[] =
{
	&forwarding_mod,
	&remote_management_mod,
	&upnp_mod,
	NULL
};

int tid = -1;
int is_filter_changed = 0;
int is_nat_changed = 0;
int is_mangle_changed = 0;
char wanface[IFNAMSIZ];
char lanface[IFNAMSIZ];
char wanaddr[]="xxx.xxx.xxx.xxx";
char lanaddr[]="xxx.xxx.xxx.xxx";
char unnumbered_lanaddr[]="xxx.xxx.xxx.xxx";
int lancidr = 0;
int wancidr = 0;
int unnumbered_lancidr = 0;
int web_lanport = HTTP_PORT;
int firewallenable = 0;
int natenable = 0;
int dmzenable = 0;
int remotemanage = 0;
int upnpenable = 0;
int unnumbered_enable = 0;
int dft_wan_idx = 0;

timer_t timerid;

char *pid_file = NULL;

void append_chain(module_chains_p mod_chains, const char * table_name, const char * chain_name)
{
	double_list_p root = NULL;
	iptable_chain_p tmp;
	size_t len;

	if(mod_chains == NULL || table_name == NULL || chain_name == NULL)
	{
		return;
	}
	if(chain_name[0] == '\0')
	{
		return;
	}
	if (strcmp(table_name, FILTER_TABLE) == 0)
	{
		root = &(mod_chains->filter_chains);
	}
	else if (strcmp(table_name, NAT_TABLE) == 0)
	{
		root = &(mod_chains->nat_chains);
	}
	else if (strcmp(table_name, MANGLE_TABLE) == 0)
	{
		root = &(mod_chains->mangle_chains);
	}
	else
	{
	    cprintf("failed to append_chain due to unknown table name!\n");
	    return;
	}

	/* Initialize double list entry */
	if(root->next == NULL)
	{
		root->next = root;
		root->prev = root;
	}

	len = strlen(chain_name) + 1;
	if(len > MAX_CHAIN_NAME)
	{
	    cprintf("The length of chain name [%s] exceed %d bytes!\n", chain_name, MAX_CHAIN_NAME - 1);
	    return;
	}

	tmp = (iptable_chain_p)malloc(sizeof(iptable_chain_t));
	if (tmp == NULL)
	{
	    cprintf("failed to allocate memory!\n");
	    return;
	}

	memcpy(tmp->chain, chain_name, len);
	tmp->list_entry.next = root;
	tmp->list_entry.prev = root->prev;
	root->prev->next = &(tmp->list_entry);
	root->prev = &(tmp->list_entry);

    dprintf("Add chain %s to table %s\n", chain_name, table_name);
}


void append_rule(module_rules_t* mod_rules, char* table_name, const char *fmt,...)
{
	va_list args;
	iptable_rule_t** head = NULL;
	iptable_rule_t** tail = NULL;

	if (strcmp(table_name, FILTER_TABLE) == 0)
	{
		head = &(mod_rules->filter_rules_head);
		tail = &(mod_rules->filter_rules_tail);
	}
	else if (strcmp(table_name, NAT_TABLE) == 0)
	{
		head = &(mod_rules->nat_rules_head);
		tail = &(mod_rules->nat_rules_tail);
	}
	else if (strcmp(table_name, MANGLE_TABLE) == 0)
	{
		head = &(mod_rules->mangle_rules_head);
		tail = &(mod_rules->mangle_rules_tail);
	}
	else
	{
		cprintf("failed to append_rule due to unexpected table name!\n");
		return;
	}

	iptable_rule_t* temp_rule = (iptable_rule_t*)malloc(sizeof(iptable_rule_t));
	if (temp_rule == NULL)
	{
		cprintf("failed to allocate memory!\n");
		return;
	}

	temp_rule->next = NULL;
	va_start(args, fmt);
	vsnprintf(temp_rule->rule, sizeof(temp_rule->rule), fmt, args);
	va_end(args);

	dprintf("%s : %s", table_name, temp_rule->rule);

	if (*head == NULL)
	{
		*head = temp_rule;
		*tail = temp_rule;
	}
	else
	{
		(*tail)->next = temp_rule;
		*tail = temp_rule;
	}
}

void add_conntrack_rule(module_conntrack_t* mod_conntrack, conntrack_info_t new_rule)
{
	conntrack_rules_t ** head = &(mod_conntrack->conntrack_rules_head);
	conntrack_rules_t ** tail = &(mod_conntrack->conntrack_rules_tail);
	conntrack_rules_t * temp_rule = NULL;

	temp_rule = (conntrack_rules_t *)malloc(sizeof(conntrack_rules_t));
	if (temp_rule == NULL)
	{
		cprintf("failed to allocate memory!\n");
		return;
	}

	temp_rule->next = NULL;
	temp_rule->rule_data = new_rule;

	if (*head == NULL)
	{
		*head = temp_rule;
		*tail = temp_rule;
	}
	else
	{
		(*tail)->next = temp_rule;
		*tail = temp_rule;
	}
}

static int same_conntrack_rule(conntrack_info_t old_rule, conntrack_info_t new_rule)
{
	if (old_rule.orig_src != new_rule.orig_src)
		return 0;
	if (old_rule.orig_dst != new_rule.orig_dst)
		return 0;
	if (old_rule.reply_src != new_rule.reply_src)
		return 0;
	if (old_rule.reply_dst != new_rule.reply_dst)
		return 0;
	if (old_rule.orig_port_src != new_rule.orig_port_src)
		return 0;
	if (old_rule.orig_port_dst != new_rule.orig_port_dst)
		return 0;
	if (old_rule.reply_port_src != new_rule.reply_port_src)
		return 0;
	if (old_rule.reply_port_dst != new_rule.reply_port_dst)
		return 0;
	if (old_rule.orig_port_dst_range != new_rule.orig_port_dst_range)
		return 0;
	if (old_rule.reply_port_src_range != new_rule.reply_port_src_range)
		return 0;
	if (old_rule.proto != new_rule.proto)
		return 0;

	return 1;
}

static void delete_conntrack(conntrack_info_t data)
{
	char cmd[256] = "";
	char tmp[256] = "";

	snprintf(cmd, sizeof(cmd), "conntrack -D");

	/* 0 matches all */
	if (data.orig_src != 0)
	{
		char *ip = NULL;
		ip = inet_ntoa(*(struct in_addr *)&data.orig_src);
		snprintf(tmp, sizeof(tmp), "%s --orig-src %s", cmd, ip);
		strncpy(cmd, tmp, sizeof(cmd));
	}
	if (data.orig_dst != 0)
	{
		char *ip = NULL;
		ip = inet_ntoa(*(struct in_addr *)&data.orig_dst);
		snprintf(tmp, sizeof(tmp), "%s --orig-dst %s", cmd, ip);
		strncpy(cmd, tmp, sizeof(cmd));
	}
	if (data.reply_src != 0)
	{
		char *ip = NULL;
		ip = inet_ntoa(*(struct in_addr *)&data.reply_src);
		snprintf(tmp, sizeof(tmp), "%s --reply-src %s", cmd, ip);
		strncpy(cmd, tmp, sizeof(cmd));
	}
	if (data.reply_dst != 0)
	{
		char *ip = NULL;
		ip = inet_ntoa(*(struct in_addr *)&data.reply_dst);
		snprintf(tmp, sizeof(tmp), "%s --reply-dst %s", cmd, ip);
		strncpy(cmd, tmp, sizeof(cmd));
	}
	if (data.proto != 0)
	{
		snprintf(tmp, sizeof(tmp), "%s --proto %d", cmd, data.proto);
		strncpy(cmd, tmp, sizeof(cmd));
	}
	if (data.orig_port_src != 0)
	{
		snprintf(tmp, sizeof(tmp), "%s --orig-port-src %d", cmd, data.orig_port_src);
		strncpy(cmd, tmp, sizeof(cmd));
	}
	if (data.orig_port_dst != 0)
	{
		/* Add port range support for port forwarding function */
		uint16_t port_end = data.orig_port_dst + data.orig_port_dst_range;
		snprintf(tmp, sizeof(tmp), "%s --orig-port-dst %d:%d", cmd, data.orig_port_dst, port_end);
		strncpy(cmd, tmp, sizeof(cmd));
	}
	if (data.reply_port_src != 0)
	{
		/* Add port range support for port forwarding function */
		uint16_t port_end = data.reply_port_src + data.reply_port_src_range;
		snprintf(tmp, sizeof(tmp), "%s --reply-port-src %d:%d", cmd, data.reply_port_src, port_end);
		strncpy(cmd, tmp, sizeof(cmd));
	}
	if (data.reply_port_dst != 0)
	{
		snprintf(tmp, sizeof(tmp), "%s --reply-port-dst %d", cmd, data.reply_port_dst);
		strncpy(cmd, tmp, sizeof(cmd));
	}

	if(strcmp(cmd, "conntrack -D"))
		system(cmd);

}

void cleanup_conntrack_all(void)
{
	openlog("NAT", 0, LOG_USER);
	syslog(LOG_NOTICE, "cleanup /proc/net/netfilter/nf_conntrack_flush");
	closelog();
	system("cat /proc/net/netfilter/nf_conntrack_flush");
}

static void free_conntrack_list(module_conntrack_t * mod_conntrack)
{
	conntrack_rules_t * pre = NULL;
	conntrack_rules_t * iter = NULL;

	pre = mod_conntrack->conntrack_rules_head;
	iter = mod_conntrack->conntrack_rules_head;
	while (iter != NULL)
	{
		pre = iter;
		iter = iter->next;
		free(pre);
	}
	mod_conntrack->conntrack_rules_head = NULL;
	mod_conntrack->conntrack_rules_tail = NULL;
}

void compare_and_clean_conntrack(module_conntrack_t * old_conntrack, module_conntrack_t * new_conntrack)
{
	conntrack_rules_t * pt_old = old_conntrack->conntrack_rules_head;
	conntrack_rules_t * pt_new = new_conntrack->conntrack_rules_head;
	int has_same_rule;

	while (pt_old != NULL)
	{
		has_same_rule = 0;

		while(pt_new != NULL)
		{
			if (same_conntrack_rule(pt_old->rule_data, pt_new->rule_data) == 1)
			{
				has_same_rule = 1;
				break;
			}
			pt_new = pt_new->next;
		}

		if (has_same_rule == 0)
			delete_conntrack(pt_old->rule_data);

		pt_old = pt_old->next;
		pt_new = new_conntrack->conntrack_rules_head;
	}

	free_conntrack_list(old_conntrack);

	old_conntrack->conntrack_rules_head = new_conntrack->conntrack_rules_head;
	old_conntrack->conntrack_rules_tail = new_conntrack->conntrack_rules_tail;
	new_conntrack->conntrack_rules_head = NULL;
	new_conntrack->conntrack_rules_tail = NULL;
}

void cleanup_conntrack_by_mac(char* mac)
{
   char ip_list[256]="";
   char ip[20], *next;
   char cmd[256];

   get_ip_list_by_mac(mac, ip_list, sizeof(ip_list));
   
   foreach(ip, ip_list, next){

	   //clean up ip conntrack by matching --orig-src
	   snprintf(cmd ,sizeof(cmd), "conntrack -D -s %s", ip);
	   //cprintf("cmd is [%s]\n", cmd);
	   system(cmd);

	   //clean up ip conntrack by matching --reply-src
	   snprintf(cmd ,sizeof(cmd), "conntrack -D -r %s", ip);
	   //cprintf("cmd is [%s]\n", cmd);
	   system(cmd);
   }
}

int netmask_to_cidr(char *netmask)
{
	int a, flag, ret;
	int b[4];
	int i, j;

	sscanf(netmask, "%d.%d.%d.%d", &b[0], &b[1], &b[2], &b[3]);

	flag = 0;
	ret = 0;
	for(i = 0; i < 4; i++)
	{
		a = (1 << 7);
		for(j = 0; j < 8; j++)
		{
			if(0 == flag)
			{
				if((a & b[i]) == 0)
				{
					flag = 1;
				}
				else
				{
					ret ++;
					a = (a >> 1);
				}
			}
			else
			{
				if((a & b[i]) != 0)
				{
					return -1;
				}
			}
		}
	}
	return ret;
}

/*
 * get general parameters from glbcfg
 */
static void prepare_parm()
{
	char buffer[128];
	struct stat statbuff;

	/* Shell Xie 2013-12-09, multiple wan support */
	char wan_idx_char[8]="";
	char var[CCFG_NAME_LEN]="";
	char prefix[16]="";
	mapi_ccfg_get_str(tid, "ARC_WAN_DefaultRouteIdx", wan_idx_char, sizeof(wan_idx_char));
	if(strlen(wan_idx_char) == 0 || atoi(wan_idx_char) < 0)
	{
		strcpy(wan_idx_char, "0");
	}
	snprintf(prefix, sizeof(prefix), "ARC_WAN_%s_", wan_idx_char);
	dft_wan_idx = atoi(wan_idx_char);

	mapi_ccfg_get_str(tid, strcat_r(prefix, "Iface", var, CCFG_NAME_LEN), wanface, IFNAMSIZ); //Real iface name is stored in VAR_WANx_IFACE

	strcat_r(prefix, "IP4_Proto", var, CCFG_NAME_LEN);
	if(mapi_ccfg_match_str(tid, var, "pptp") || mapi_ccfg_match_str(tid, var, "l2tp") || mapi_ccfg_match_str(tid, var, "pppoe"))
	{
		mapi_ccfg_get_str(tid, strcat_r(prefix, "PPP_GET_IP4_Addr", var, CCFG_NAME_LEN), wanaddr, sizeof(wanaddr));
		mapi_ccfg_get_str(tid, strcat_r(prefix, "PPP_GET_IP4_Netmask", var, CCFG_NAME_LEN), buffer, sizeof(buffer));
	}
	else if(mapi_ccfg_match_str(tid, var, "dhcp"))
	{
		mapi_ccfg_get_str(tid, strcat_r(prefix, "DHCP_IP4_Addr", var, CCFG_NAME_LEN), wanaddr, sizeof(wanaddr));
		mapi_ccfg_get_str(tid, strcat_r(prefix, "DHCP_IP4_Netmask", var, CCFG_NAME_LEN), buffer, sizeof(buffer));
	}
	/* Ticket #7959 support MAP-E by seal */
	else if(mapi_ccfg_match_str(tid, var, "v6plus"))
	{
		mapi_tmp_get(tid, "TMP_WAN_MAPE_IP4_Addr", wanaddr, sizeof(wanaddr));
		strncpy(buffer, "255.255.255.255", sizeof(buffer));
		if(mapi_ccfg_get_str(tid, "ARC_WAN_MAPE_Ifname", wanface, sizeof(wanface)) != wanface || strlen(wanface) <= 0)
			strcpy(wanface, "map0");
	}
	else
	{
		mapi_ccfg_get_str(tid, strcat_r(prefix, "IP4_Addr", var, CCFG_NAME_LEN), wanaddr, sizeof(wanaddr));
		mapi_ccfg_get_str(tid, strcat_r(prefix, "IP4_Netmask", var, CCFG_NAME_LEN), buffer, sizeof(buffer));
	}
	wancidr = netmask_to_cidr(buffer);

	mapi_ccfg_get_str(tid, "ARC_LAN_0_IP4_Addr", lanaddr, sizeof(lanaddr));
	mapi_ccfg_get_str(tid, "ARC_LAN_0_Ifname", lanface, IFNAMSIZ);

	mapi_ccfg_get_str(tid, "ARC_LAN_0_IP4_Netmask", buffer, sizeof(buffer));
	lancidr = netmask_to_cidr(buffer);

	if(mapi_ccfg_match_str(tid, "ARC_IPUNNUMBERED_Enable", "1"))
	{
		unnumbered_enable = 1;
		mapi_ccfg_get_str(tid, "ARC_IPUNNUMBERED_LAN_Address", unnumbered_lanaddr, sizeof(unnumbered_lanaddr));
		mapi_ccfg_get_str(tid, "ARC_IPUNNUMBERED_LAN_Netmask", buffer, sizeof(buffer));
		unnumbered_lancidr = netmask_to_cidr(buffer);
	}
	else
	{
		unnumbered_enable = 0;
	}

	if(mapi_ccfg_match_str(tid, "ARC_FIREWALL_Enable", "1"))
	{
		firewallenable = 1;
	}
	else
	{
		firewallenable = 0;
	}

	if(mapi_ccfg_match_str(tid, "ARC_WAN_0_NAT_Enable", "1"))
	{
		natenable = 1;
	}
	else
	{
		natenable = 0;
	}

	if(natenable && firewallenable && mapi_ccfg_match_str(tid, "ARC_FIREWALL_DMZ_Enable","1"))
	{
		dmzenable = 1;
	}
	else
	{
		dmzenable = 0;
	}

	if(mapi_ccfg_match_str(tid, "ARC_UPNP_Enable", "1"))
	{
		upnpenable = 1;
	}
	else
	{
		upnpenable = 0;
	}

	/* Remote management */
	if (mapi_ccfg_match_str(tid, "ARC_UI_WEB_REMOTEMN_Enable", "1"))
	{
		remotemanage = 1;
	}
	else
	{
		remotemanage = 0;
	}

	if (mapi_ccfg_match_str(tid, "ARC_UI_WEB_REMOTEMN_Https", "1") && mapi_ccfg_match_str(tid, "ARC_SYS_HTTPS_Enable", "1"))
	{
		//crazy_liang 2013.9.22: Use current http(s) port for remote management port.
		if(mapi_ccfg_match_str(tid, "ARC_SYS_HTTP_UseSpecialPort", "1"))
		{
			web_lanport = atoi(mapi_ccfg_get_str(tid, "ARC_SYS_HTTPS_PORT", buffer, sizeof(buffer)));
			if(web_lanport <= 0 || web_lanport > 65535)
			{
				web_lanport = HTTPS_PORT;
			}
		}
		else
		//end crazy_liang 2013.9.22
		{
		web_lanport = HTTPS_PORT;
		}
	}
	else
	{
		//crazy_liang 2013.9.22: Use current http(s) port for remote management port.
		if(mapi_ccfg_match_str(tid, "ARC_SYS_HTTP_UseSpecialPort", "1"))
		{
			web_lanport = atoi(mapi_ccfg_get_str(tid, "ARC_SYS_HTTP_PORT", buffer, sizeof(buffer)));
			if(web_lanport <= 0 || web_lanport > 65535)
			{
				web_lanport = HTTP_PORT;
			}
		}
		else
		//end crazy_liang 2013.9.22
		{
		web_lanport = HTTP_PORT;
		}
	}

	is_filter_changed = 0;
	is_nat_changed = 0;
	is_mangle_changed = 0;

	if(stat(IPTABLES_SAVE_FILE, &statbuff) == 0)
	{
		unlink(IPTABLES_SAVE_FILE);
	}
}

// clean chains for all modules
static void free_chains(double_list_p root)
{
	double_list_p cur;
	double_list_p next;
	
	if(root != NULL)
	{
		if(root->next != NULL)
		{
			cur = root->next;
			while (cur != root)
			{
				next = cur->next;
				free(cur);
				cur = next;
			}
		}
		root->next = NULL;
		root->prev = NULL;
	}
	
}

static module_t* find_mod_by_name(const char* mod_name)
{
	int i = 0;
	for (i = 0;i < sizeof(mod_seq_in_filter_tb)/sizeof(module_t*); i++)
	{
		if (mod_seq_in_filter_tb[i] == NULL)
		{
			continue;
		}

		if (strcmp(mod_seq_in_filter_tb[i]->module_name, mod_name) == 0)
		{
			return mod_seq_in_filter_tb[i];
		}
	}

	for (i = 0;i < sizeof(mod_seq_in_nat_tb)/sizeof(module_t*); i++)
	{
		if (mod_seq_in_nat_tb[i] == NULL)
		{
			continue;
		}

		if (strcmp(mod_seq_in_nat_tb[i]->module_name, mod_name) == 0)
		{
			return mod_seq_in_nat_tb[i];
		}
	}

	for (i = 0;i < sizeof(mod_seq_in_mangle_tb)/sizeof(module_t*); i++)
	{
		if (mod_seq_in_mangle_tb[i] == NULL)
		{
			continue;
		}

		if (strcmp(mod_seq_in_mangle_tb[i]->module_name, mod_name) == 0)
		{
			return mod_seq_in_mangle_tb[i];
		}
	}

	return NULL;
}

static void generate_rules_for_conntrack_tb(const char * mod_name)
{

	if (strcmp(mod_name, "ALL") == 0)
	{
#if 0
		int i;
		for (i = 0; i < sizeof(mod_seq_in_conntrack_tb)/sizeof(module_t*); i++)
		{
			if (mod_seq_in_conntrack_tb[i] == NULL)
				continue;

			if (mod_seq_in_conntrack_tb[i]->conntrack_tb_routine != NULL)
				mod_seq_in_conntrack_tb[i]->conntrack_tb_routine();
		}
#else
		cleanup_conntrack_all();
#endif
	}
	else
	{
		module_t* mod = NULL;
		mod = find_mod_by_name(mod_name);
		if (mod == NULL)
		{
			dprintf("failed to find module by name[%s]", mod_name);
			return;
		}

		if (mod->conntrack_tb_routine != NULL)
			mod->conntrack_tb_routine();
	}
}

// generate rules of filter table for all modules
static void generate_rules_for_filter_tb()
{
	int i = 0;
	iptable_rule_t* pre = NULL;
	iptable_rule_t* iter = NULL;

	for (i = 0;i < sizeof(mod_seq_in_filter_tb)/sizeof(module_t*); i++)
	{
		if (mod_seq_in_filter_tb[i] == NULL)
		{
			continue;
		}

		// clean firstly
		free_chains(&(mod_seq_in_filter_tb[i]->mod_chains.filter_chains));
		
		module_rules_t* mod_rules = &(mod_seq_in_filter_tb[i]->mod_rules);
		if (mod_rules->filter_rules_head != NULL)
		{
			pre = mod_rules->filter_rules_head;
			iter = mod_rules->filter_rules_head;
			while (iter != NULL)
			{
				pre = iter;
				iter = iter->next;
				free(pre);
			}
			mod_rules->filter_rules_head = NULL;
			mod_rules->filter_rules_tail = NULL;
		}

		// generate secondly
		if (mod_seq_in_filter_tb[i]->filter_tb_routine != NULL)
		{
			dprintf("module [%s] fill filter table\n", mod_seq_in_filter_tb[i]->module_name);
			mod_seq_in_filter_tb[i]->filter_tb_routine();
		}
	}
}

// generate rules of nat table for all modules
static void generate_rules_for_nat_tb()
{
	int i = 0;
	iptable_rule_t* pre = NULL;
	iptable_rule_t* iter = NULL;

	for (i = 0;i < sizeof(mod_seq_in_nat_tb)/sizeof(module_t*); i++)
	{

		if (mod_seq_in_nat_tb[i] == NULL)
		{
			continue;
		}

		// clean firstly
		free_chains(&(mod_seq_in_nat_tb[i]->mod_chains.nat_chains));
		
		module_rules_t* mod_rules = &(mod_seq_in_nat_tb[i]->mod_rules);
		if (mod_rules->nat_rules_head != NULL)
		{
			pre = mod_rules->nat_rules_head;
			iter = mod_rules->nat_rules_head;
			while (iter != NULL)
			{
				pre = iter;
				iter = iter->next;
				free(pre);
			}
			mod_rules->nat_rules_head = NULL;
			mod_rules->nat_rules_tail = NULL;
		}

		// generate secondly
		if (mod_seq_in_nat_tb[i]->nat_tb_routine != NULL)
		{
			dprintf("module [%s] fill nat table\n", mod_seq_in_nat_tb[i]->module_name);
			mod_seq_in_nat_tb[i]->nat_tb_routine();
		}
	}
}

// generate rules of mangle table for all modules
static void generate_rules_for_mangle_tb()
{
	int i = 0;
	iptable_rule_t* pre = NULL;
	iptable_rule_t* iter = NULL;

	for (i = 0;i < sizeof(mod_seq_in_mangle_tb)/sizeof(module_t*); i++)
	{
		if (mod_seq_in_mangle_tb[i] == NULL)
		{
			continue;
		}

		// clean firstly
		free_chains(&(mod_seq_in_mangle_tb[i]->mod_chains.mangle_chains));
		
		module_rules_t* mod_rules = &(mod_seq_in_mangle_tb[i]->mod_rules);
		if (mod_rules->mangle_rules_head != NULL)
		{
			pre = mod_rules->mangle_rules_head;
			iter = mod_rules->mangle_rules_head;
			while (iter != NULL)
			{
				pre = iter;
				iter = iter->next;
				free(pre);
			}
			mod_rules->mangle_rules_head = NULL;
			mod_rules->mangle_rules_tail = NULL;
		}

		// generate secondly
		if (mod_seq_in_mangle_tb[i]->mangle_tb_routine != NULL)
		{
			dprintf("module [%s] fill mangle table\n", mod_seq_in_mangle_tb[i]->module_name);
			mod_seq_in_mangle_tb[i]->mangle_tb_routine();
		}
	}
}

static void generate_all_modules_rules_in_memory()
{
	prepare_parm();

	generate_rules_for_nat_tb();
	generate_rules_for_mangle_tb();
	generate_rules_for_filter_tb();
	generate_rules_for_conntrack_tb("ALL");
}

static void clean_chains_in_module(module_t* mod)
{
	if(mod != NULL)
	{
		free_chains(&(mod->mod_chains.filter_chains));
		free_chains(&(mod->mod_chains.nat_chains));
		free_chains(&(mod->mod_chains.mangle_chains));
	}
}

static void clean_rules_in_module(module_t* mod)
{
	iptable_rule_t* pre = NULL;
	iptable_rule_t* iter = NULL;

	module_rules_t* mod_rules = &(mod->mod_rules);

	if (mod_rules->filter_rules_head != NULL)
	{
		pre = mod_rules->filter_rules_head;
		iter = mod_rules->filter_rules_head;
		while (iter != NULL)
		{
			pre = iter;
			iter = iter->next;
			free(pre);
		}
		mod_rules->filter_rules_head = NULL;
		mod_rules->filter_rules_tail = NULL;
	}

	if (mod_rules->nat_rules_head != NULL)
	{
		pre = mod_rules->nat_rules_head;
		iter = mod_rules->nat_rules_head;
		while (iter != NULL)
		{
			pre = iter;
			iter = iter->next;
			free(pre);
		}
		mod_rules->nat_rules_head = NULL;
		mod_rules->nat_rules_tail = NULL;
	}

	if (mod_rules->mangle_rules_head != NULL)
	{
		pre = mod_rules->mangle_rules_head;
		iter = mod_rules->mangle_rules_head;
		while (iter != NULL)
		{
			pre = iter;
			iter = iter->next;
			free(pre);
		}
		mod_rules->mangle_rules_head = NULL;
		mod_rules->mangle_rules_tail = NULL;
	}
}

static void update_rules_by_module(const char* mod_name)
{
	module_t* mod = NULL;
	if (strcmp(mod_name, "ALL") == 0)
	{
		generate_all_modules_rules_in_memory();
		is_filter_changed = 1;
		is_nat_changed = 1;
		is_mangle_changed = 1;
		return;
	}

	mod = find_mod_by_name(mod_name);
	if (mod == NULL)
	{
		dprintf("failed to find module by name[%s]", mod_name);
		return;
	}

    clean_chains_in_module(mod);
	clean_rules_in_module(mod);

	if (mod->filter_tb_routine != NULL)
	{
		mod->filter_tb_routine();
		is_filter_changed = 1;
	}

	if (mod->nat_tb_routine != NULL)
	{
		mod->nat_tb_routine();
		is_nat_changed = 1;
	}

	if (mod->mangle_tb_routine != NULL)
	{
		mod->mangle_tb_routine();
		is_mangle_changed = 1;
	}
}

// write the rules of nat table for all modules to ipt file
static void generate_nat_rules_in_ipt()
{
	module_chains_p chains;
	iptable_chain_p chain;
	double_list_p root = NULL;
	double_list_p tmp = NULL;
    int i;
	FILE *fp = NULL;

	if ((fp = fopen(IPTABLES_SAVE_FILE, "a")) == NULL)
	{
		dprintf("Can't open /tmp/.ipt\n");
		return;
	}
	fprintf(fp,
			"*nat\n"
			":PREROUTING ACCEPT [0:0]\n"
			":POSTROUTING ACCEPT [0:0]\n"
			":OUTPUT ACCEPT [0:0]\n"
		   );

	for (i = 0; i < sizeof(mod_seq_in_nat_tb)/sizeof(module_t*); i++)
	{
	    if (mod_seq_in_nat_tb[i] == NULL)
	    {
	        continue;
	    }

		chains = &(mod_seq_in_nat_tb[i]->mod_chains);
		root = &(chains->nat_chains);
		tmp = root->next;
		if(tmp != NULL)
		{
			while(tmp != root)
			{
				chain = (iptable_chain_p)tmp;
				fprintf(fp, ":%s - [0:0]\n", chain->chain);
				tmp = tmp->next;
			}
		}
	}

    for (i = 0; i < sizeof(mod_seq_in_nat_tb)/sizeof(module_t*); i++)
	{
		if (mod_seq_in_nat_tb[i] == NULL)
		{
			continue;
		}

		module_rules_t mod_rules = mod_seq_in_nat_tb[i]->mod_rules;
		iptable_rule_t* iptable_rule = mod_rules.nat_rules_head;
		while (iptable_rule != NULL)
		{
			fprintf(fp, "%s", iptable_rule->rule);
			iptable_rule = iptable_rule->next;
		}
	}

	fprintf(fp, "COMMIT\n");
	fclose(fp);
}

#if 0
/*arc_qos will call MTK's hardware QoS API to update mangle table, and we don't have its source.
  access control module need to update firewall rules periodically. thus rules of mangle table will be lost.
  since we didn't add any rules in mangle table, so we just ignore mangle table.
  */
// write the rules of mangle table for all modules to ipt file
static void generate_mangle_rules_in_ipt()
{
	module_chains_p chains;
	iptable_chain_p chain;
	double_list_p root = NULL;
	double_list_p tmp = NULL;
    int i;
	FILE *fp = NULL;

	if ((fp = fopen(IPTABLES_SAVE_FILE, "a")) == NULL)
	{
		dprintf("Can't open /tmp/.ipt\n");
		return;
	}

	fprintf(fp,
			"*mangle\n"
			":PREROUTING ACCEPT [0:0]\n"
            ":INPUT ACCEPT [0:0]\n"
            ":FORWARD ACCEPT [0:0]\n"
            ":OUTPUT ACCEPT [0:0]\n"
            ":POSTROUTING ACCEPT [0:0]\n");

	for (i = 0; i < sizeof(mod_seq_in_mangle_tb)/sizeof(module_t*); i++)
	{
	    if (mod_seq_in_mangle_tb[i] == NULL)
	    {
	        continue;
	    }

		chains = &(mod_seq_in_mangle_tb[i]->mod_chains);
		root = &(chains->mangle_chains);
		tmp = root->next;
		if(tmp != NULL)
		{
			while(tmp != root)
			{
				chain = (iptable_chain_p)tmp;
				fprintf(fp, ":%s - [0:0]\n", chain->chain);
				tmp = tmp->next;
			}
		}
	}

    for (i = 0; i < sizeof(mod_seq_in_mangle_tb)/sizeof(module_t*); i++)
	{
		if (mod_seq_in_mangle_tb[i] == NULL)
		{
			continue;
		}

		module_rules_t mod_rules = mod_seq_in_mangle_tb[i]->mod_rules;
		iptable_rule_t* iptable_rule = mod_rules.mangle_rules_head;
		while (iptable_rule != NULL)
		{
			fprintf(fp, "%s", iptable_rule->rule);
			iptable_rule = iptable_rule->next;
		}
	}

	fprintf(fp, "COMMIT\n");
	fclose(fp);
}
#endif

// write the rules of filter table for all modules to ipt file
static void generate_filter_rules_in_ipt()
{
	module_chains_p chains;
	iptable_chain_p chain;
	double_list_p root = NULL;
	double_list_p tmp = NULL;
	int i;
	FILE *fp = NULL;

	if ((fp = fopen(IPTABLES_SAVE_FILE, "a")) == NULL)
	{
		dprintf("Can't open /tmp/.ipt\n");
		return;
	}
	fprintf(fp, "*filter\n"
			":INPUT ACCEPT [0:0]\n"
			":FORWARD ACCEPT [0:0]\n"
			":OUTPUT ACCEPT [0:0]\n"
		   );

	for (i = 0; i < sizeof(mod_seq_in_filter_tb)/sizeof(module_t*); i++)
	{
	    if (mod_seq_in_filter_tb[i] == NULL)
	    {
	        continue;
	    }

		chains = &(mod_seq_in_filter_tb[i]->mod_chains);
		root = &(chains->filter_chains);
		tmp = root->next;
		if(tmp != NULL)
		{
			while(tmp != root)
			{
				chain = (iptable_chain_p)tmp;
				fprintf(fp, ":%s - [0:0]\n", chain->chain);
				tmp = tmp->next;
			}
		}
	}

	for (i = 0; i < sizeof(mod_seq_in_filter_tb)/sizeof(module_t*); i++)
	{
		if (mod_seq_in_filter_tb[i] == NULL)
		{
			continue;
		}

		module_rules_t mod_rules = mod_seq_in_filter_tb[i]->mod_rules;
		iptable_rule_t* iptable_rule = mod_rules.filter_rules_head;
		while (iptable_rule != NULL)
		{
			fprintf(fp, "%s", iptable_rule->rule);
			iptable_rule = iptable_rule->next;
		}
	}

	if (mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "router") || (mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") && mapi_ccfg_match_str(tid, "ARC_WAN_0_BridgeEnable", "0")))
	{
		if(firewallenable)
		{
			if(mapi_ccfg_match_str(tid, "ARC_SYS_LOG_NAT_Enable", "1"))
			{
				fprintf(fp, "-A FORWARD -j ULOG --ulog-prefix \"FIREWALL-NAT :\"\n");
				fprintf(fp, "-A INPUT ! -i %s -j ULOG --ulog-prefix \"FIREWALL-NAT :\"\n", lanface);
			}
			fprintf(fp, "-A FORWARD -j LOG_DROP\n");
			fprintf(fp, "-A INPUT ! -i %s -j LOG_DROP\n", lanface);
		}
	}
	else
	{
		fprintf(fp, "-A INPUT -i %s -p udp -m udp --dport 68 -j DROP\n", lanface);
	}

	fprintf(fp, "COMMIT\n");
	fclose(fp);
}

static void update_iptable_rules()
{
	char mod[64] = {0};
	char* mod_name = mod;
	char* iter = NULL;
	if (mapi_tmp_get(tid, VAR_FIREWALL_MODULE_NAME, mod, sizeof(mod)) == NULL)
	{
		dprintf("%s is NOT found in temp glbcfg!\n", VAR_FIREWALL_MODULE_NAME);
		return;
	}

	dprintf("The module name is %s.\n", mod);

	/* Reset the alarm */
	current_alarm = -1;

	prepare_parm();

	// The module name may consist of mutliple modules
	while(*mod_name != '\0')
	{
		iter = strstr(mod_name, ";");
		if (iter != NULL)
		{
			*iter = '\0';
		}
		dprintf("mod_name = %s\n", mod_name);
		update_rules_by_module(mod_name);
		if (iter == NULL)
		{
			break;
		}
		else
		{
			mod_name = iter + 1;
		}
	}

	if (is_nat_changed)
	{
		generate_nat_rules_in_ipt();
	}
#if 0 
	/*arc_qos will call MTK's hardware QoS API to update mangle table, and we don't have its source.
      access control module need to update firewall rules periodically. thus rules of mangle table will be lost.
      since we didn't add any rules in mangle table, so we just ignore mangle table.
 	  */
	if (is_mangle_changed)
	{
		generate_mangle_rules_in_ipt();
	}
#endif
	if (is_filter_changed)
	{
		generate_filter_rules_in_ipt();
	}

	if (is_nat_changed || is_mangle_changed || is_filter_changed)
	{
		system("iptables-restore /tmp/.ipt");
	}

	mapi_tmp_get(tid, VAR_FIREWALL_MODULE_NAME, mod, sizeof(mod));
	mod_name = mod;
	// The module name may consist of mutliple modules
	while(*mod_name != '\0')
	{
		iter = strstr(mod_name, ";");
		if (iter != NULL)
		{
			*iter = '\0';
		}
		dprintf("mod_name = %s\n", mod_name);
		generate_rules_for_conntrack_tb(mod_name);
		if (iter == NULL)
		{
			break;
		}
		else
		{
			mod_name = iter + 1;
		}
	}

	if (mapi_tmp_set(tid, VAR_FIREWALL_MODULE_NAME, "") == MID_FAIL)
	{
		dprintf("failed to set %s in temp gblcfg.\n", VAR_FIREWALL_MODULE_NAME);
	}
}

// handle USR1 signal
static void sigusr1_handler(int sig)
{
	dprintf("sigusr1 is received.\n");

	update_iptable_rules();
}

static void sigterm_handler(int sig)
{
	if (tid != MID_FAIL)
		mapi_end_transc(tid);

	if (pid_file != NULL)
		unlink(pid_file);

	timer_delete(timerid);

	exit(0);
}

static void sigalrm_handler(int sig)
{
	mapi_tmp_set(tid, VAR_FIREWALL_MODULE_NAME, "access_control_mod");

	update_iptable_rules();
}

void update_timer(time_t interval)
{
	/* TODO
	 * In order to support multiple modules, we need to add a mechanism to
	 * accept only the nearest alarm, so that the further alarm won't override
	 * the nearer one. However using a global variable here may cause potential
	 * problems.
	 */
	struct itimerspec its;

	if ((current_alarm >= 0) && (interval > current_alarm))
		return;

	/* Postpone 1 second, to avoid the 0 value or a potential 0 value. */
	interval++;

	current_alarm = interval;

	its.it_interval.tv_sec = 0;
	its.it_interval.tv_nsec = 0;
	its.it_value.tv_sec = interval;
	its.it_value.tv_nsec = 0;

	timer_settime(timerid, 0, &its, NULL);
}

static void firwall_init()
{
	signal(SIGUSR1, sigusr1_handler);
	signal(SIGTERM, sigterm_handler);
	signal(SIGALRM, sigalrm_handler);

	timer_create(CLOCK_REALTIME, NULL, &timerid);

	system("echo 1 > /proc/sys/net/ipv4/ip_forward");

	// generate rules in memory per module
	generate_all_modules_rules_in_memory();
	generate_nat_rules_in_ipt();
#if 0
	/*arc_qos will call MTK's hardware QoS API to update mangle table, and we don't have its source.
      access control module need to update firewall rules periodically. thus rules of mangle table will be lost.
      since we didn't add any rules in mangle table, so we just ignore mangle table.
 	  */
	generate_mangle_rules_in_ipt();
#endif
	generate_filter_rules_in_ipt();

	system("iptables-restore /tmp/.ipt");

	if (mapi_tmp_set(tid, VAR_FIREWALL_MODULE_NAME, "") == MID_FAIL)
	{
		dprintf("failed to set %s in temp gblcfg.\n", VAR_FIREWALL_MODULE_NAME);
	}
}

static void usage(void)
{
	printf("arc_firewalld [-pf pidfile] [-d | --daemonize]\n");
}

int main(int argc, char **argv)
{
	int i;
	int run_daemon = 0;

	for (i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--daemonize"))
		{
			run_daemon = 1;
		}
		else if (!strcmp(argv[i], "-pf"))
		{
			if (++i == argc)
			{
				usage();
				exit(1);
			}
			pid_file = argv[i];
		}
		else
		{
			usage();
			exit(1);
		}
	}

	if (run_daemon)
	{
		pid_t pid;

		pid = fork();
		if (pid < 0)
		{
			dprintf("Can't fork daemon\n");
			exit(1);
		}
		else if (pid > 0)
		{
			exit(0);
		}
	}

	if (pid_file != NULL)
	{
		FILE *fp = NULL;

		if ((fp = fopen(pid_file, "w")) != NULL)
		{
			fprintf(fp, "%ld\n", (long)getpid());
			fclose(fp);
		}
		else
		{
			dprintf("Can't open pid file %s\n", pid_file);
		}
	}

	if ((tid = mapi_start_transc()) == MID_FAIL )
	{
		cprintf("[%s] Failed to connect to midcore\n", __FUNCTION__);
		return -1;
	}

	firwall_init();

	while(1)
	{
		pause();
	}

	timer_delete(timerid);
	mapi_end_transc(tid);
	return 0;
}
