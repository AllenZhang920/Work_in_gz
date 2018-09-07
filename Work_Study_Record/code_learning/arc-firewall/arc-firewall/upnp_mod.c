/*
 * This module handles UPNP rules.
 */

#include "firewall.h"
#include "mod_common.h"

#define UPNP_MODULE_RULES (&(upnp_mod.mod_rules))
#define UPNP_MODULE_CHAINS (&(upnp_mod.mod_chains))

static void rules_in_conntrack_tb();
static void rules_in_filter_tb();
static void rules_in_nat_tb();

module_t upnp_mod = {
	.module_name = "upnp_mod",
	.module_desc = "handle upnp rules",
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

static int get_field_from_string(char * src, char * dst, const char keyword[], size_t size)
{
	char * cp = NULL;
	char * ep = NULL;

	cp = strstr(src, keyword);

	if (cp == NULL)
		return 1;

	cp = cp + strlen(keyword);

	if (cp != NULL)
		cp ++;
	else
		return 1;

	ep = strchr(cp, ' ');

	if ((size > ep-cp) && (ep != NULL))
	{
		strncpy(dst, cp, ep-cp);
		dst[ep-cp] = '\0';
	}
	else
	{
		strncpy(dst, cp, size);
		dst[size-1] = '\0';
	}

	return 0;
}

static void rules_in_conntrack_tb(void)
{
	char buffer[128];
	char buffer1[128];
	int i;
	char var[32];
	module_conntrack_t new_conntrack_list = {
		.conntrack_rules_head = NULL,
		.conntrack_rules_tail = NULL,
	};

	/* When system mode is bridge or wb, do nothing. */
	if(mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "bridge") || (mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") && mapi_ccfg_match_str(tid, "ARC_WAN_0_BridgeEnable", "1")) || mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "wb"))
		return;

	if (upnpenable)
	{
		if (mapi_tmp_get(tid, "upnp_rule_mask", buffer, sizeof(buffer)) == NULL)
		{
			dprintf("failed to get upnp_rule_mask from temp gblcfg");
			return;
		}

		for (i = 0; i < 100; i++)
		{
			if (buffer[i] == '1')
			{
				char reply_src[32];
				char reply_port_src[16];
				char orig_port_dst[16];
				char proto[8];
				conntrack_info_t conntrack_data;

				sprintf(var, "upnp_fw_rule_%d", i);
				//-I upnp_forward -p TCP -d 192.168.2.100 --dport 2003 -j ACCEPT
				if (mapi_tmp_get(tid, var, buffer1, sizeof(buffer1)) != NULL)
				{
					get_field_from_string(buffer1, proto, "-p", sizeof(proto));
					get_field_from_string(buffer1, reply_src, "-d", sizeof(reply_src));
					get_field_from_string(buffer1, reply_port_src, "--dport", sizeof(reply_port_src));
				}

				sprintf(var, "upnp_nat_rule_%d", i);
				//-I upnp_nat -i eth1 -p TCP --dport 2004 -j DNAT --to 192.168.2.100:2003
				if (mapi_tmp_get(tid, var, buffer1, sizeof(buffer1)) != NULL)
				{
					get_field_from_string(buffer1, orig_port_dst, "--dport", sizeof(orig_port_dst));
				}

				conntrack_data.orig_port_src = 0;
				conntrack_data.orig_port_dst = atoi(orig_port_dst);
				conntrack_data.orig_src = 0;
				conntrack_data.orig_dst = 0;
				conntrack_data.reply_src = inet_addr(reply_src);
				conntrack_data.reply_dst = 0;
				conntrack_data.reply_port_src = atoi(reply_port_src);
				conntrack_data.reply_port_dst = 0;
				conntrack_data.orig_port_dst_range = 0;
				conntrack_data.reply_port_src_range = 0;

				if (strcmp(proto, "TCP") == 0)
				{
					conntrack_data.proto = 6;
				}
				else if (strcmp(proto, "UDP") == 0)
				{
					conntrack_data.proto = 17;
				}
				else
				{
					conntrack_data.proto = 0;
				}
 
				add_conntrack_rule(&new_conntrack_list, conntrack_data);
			}
		}
	}

	compare_and_clean_conntrack(&(upnp_mod.mod_conntrack), &new_conntrack_list);
}

static void rules_in_filter_tb()
{
	char buffer[128];
	char buffer1[128];
	int i = 0;
	char var[32];

	/* When system mode is bridge or wb, do nothing. */
	if(mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "bridge") || (mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") && mapi_ccfg_match_str(tid, "ARC_WAN_0_BridgeEnable", "1")) || mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "wb"))
		return;

	if (upnpenable)
	{
		append_chain(UPNP_MODULE_CHAINS, FILTER_TABLE, "upnp_forward");
		
		append_rule(UPNP_MODULE_RULES, FILTER_TABLE, "-A FORWARD -j upnp_forward\n");

		// 1. upnp_rule_mask is written by upnp.
		// 2. its format: 1100111000... (length:100, because upnp maintain 100 rules at most).
		// 3. upnp_rule_mask[i] = '1' means variables upnp_fw_rule_i and upnp_nat_rule_i in temp 
		//    glbcfg store iptables rules. '0' means variables upnp_fw_rule_i and upnp_nat_rule_i
		//    does not store rules.
		if (mapi_tmp_get(tid, "upnp_rule_mask", buffer, 128) == NULL)
		{
			dprintf("failed to get upnp_rule_mask from temp gblcfg");
			return;
		}

		dprintf("upnp_rule_mask = %s\n", buffer);

		for (i = 0;i < 100; i++)
		{
			if (buffer[i] == '1')
			{
				sprintf(var, "upnp_fw_rule_%d", i);
				if (mapi_tmp_get(tid, var, buffer1, 128) != NULL)
				{
					append_rule(UPNP_MODULE_RULES, FILTER_TABLE, buffer1);
				}
				else
				{
					dprintf("failed to get %s from temp glbcfg.\n", var);
				}
			}
		}
	}
}

static void rules_in_nat_tb()
{
	char buffer[128];
	char buffer1[128];
	int i = 0;
	char var[32];

	/* When system mode is bridge or wb, do nothing. */
	if(mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "bridge") || (mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") && mapi_ccfg_match_str(tid, "ARC_WAN_0_BridgeEnable", "1")) || mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "wb"))
		return;

	if (upnpenable)
	{
        append_chain(UPNP_MODULE_CHAINS, NAT_TABLE, "upnp_nat");
        
		append_rule(UPNP_MODULE_RULES, NAT_TABLE, "-A PREROUTING -j upnp_nat\n");

		// 1. upnp_rule_mask is written by upnp.
		// 2. its format: 1100111000... (length:100, because upnp maintain 100 rules at most).
		// 3. upnp_rule_mask[i] = '1' means variables upnp_fw_rule_i and upnp_nat_rule_i in temp 
		//    glbcfg store iptables rules. '0' means variables upnp_fw_rule_i and upnp_nat_rule_i
		//    does not store rules.
		if (mapi_tmp_get(tid, "upnp_rule_mask", buffer, 128) == NULL)
		{
			dprintf("failed to get upnp_rule_mask from temp gblcfg");
			return;
		}

		dprintf("upnp_rule_mask = %s\n", buffer);

		for (i = 0;i < 100; i++)
		{
			if (buffer[i] == '1')
			{
				sprintf(var, "upnp_nat_rule_%d", i);
				if (mapi_tmp_get(tid, var, buffer1, 128) != NULL)
				{
					append_rule(UPNP_MODULE_RULES, NAT_TABLE, buffer1);
				}
				else
				{
					dprintf("failed to get %s from temp glbcfg.\n", var);
				}
			}
		}
	}
}

