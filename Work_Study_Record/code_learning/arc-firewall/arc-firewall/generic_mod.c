/*
 * This module handles generic rules.
 */

#include "firewall.h"
#include "mod_common.h"

#define GENERIC_MODULE_RULES (&(generic_mod.mod_rules))
#define GENERIC_MODULE_CHAINS (&(generic_mod.mod_chains))
#define MINMRU 128
#define DEFMRU 1500

static void rules_in_filter_tb();
static void rules_in_nat_tb();
static void rules_in_mangle_tb();

module_t generic_mod = {
	.module_name = "generic_mod",
	.module_desc = "handle generic rules",
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
	.mangle_tb_routine = rules_in_mangle_tb
};

static int netif_get_mtu(char *ifname)
{
	struct ifreq ifr;
	int mtu = DEFMRU;
	int sock_fd;

	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock_fd < 0)
	{
		return mtu;
	}
	memset(&ifr, '\0', sizeof (ifr));
	strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));
	if(ioctl(sock_fd, SIOCGIFMTU, (caddr_t)&ifr) >= 0)
	{
		mtu = ifr.ifr_mtu;
	}
	close(sock_fd);

	return mtu;
}

static void rules_in_filter_tb()
{
	char buffer[128];
	int mtu;
	int mru;
	int i = 0;
	char lan_ap_addr[32] = "", lan_ap_netmask[32] = "";

	/* Shell Xie 2013-12-09, multiple wan support */
	char var[CCFG_NAME_LEN]="";
	char prefix[16]="";
	char mape_firewall[256] = "";

	/* system mode is bridge or wb. */
	if(mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "bridge") || (mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") && mapi_ccfg_match_str(tid, "ARC_WAN_0_BridgeEnable", "1")) || mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "wb"))
	{
		if(mapi_ccfg_match_str(tid, "ARC_LAN_0_IP4_BridgeProto", "static"))
		{
			mapi_ccfg_get_str(tid, "ARC_LAN_0_IP4_BridgeAddr", lan_ap_addr, sizeof(lan_ap_addr));
			mapi_ccfg_get_str(tid, "ARC_LAN_0_IP4_BridgeNetmask", lan_ap_netmask, sizeof(lan_ap_netmask));
		}
		else
		{
			mapi_ccfg_get_str(tid, "ARC_LAN_0_IP4_BridgeDHCPAddr", lan_ap_addr, sizeof(lan_ap_addr));
			mapi_ccfg_get_str(tid, "ARC_LAN_0_IP4_BridgeDHCPNetmask", lan_ap_netmask, sizeof(lan_ap_netmask));
		}
		append_rule(GENERIC_MODULE_RULES,  FILTER_TABLE, "-A FORWARD -s %s/%s -j DROP\n", lan_ap_addr, lan_ap_netmask);

		return;
	}

	snprintf(prefix, sizeof(prefix), "ARC_WAN_%d_", dft_wan_idx);
	// RIP function
	if(mapi_ccfg_match_str(tid, "ARC_WAN_0_RIP_Enable", "1") && mapi_ccfg_match_str(tid, "ARC_WAN_0_NAT_Enable", "0"))
	{
		/* RIP destination address could be broadcast and multicast */
		append_rule(GENERIC_MODULE_RULES,  FILTER_TABLE, "-A INPUT -i %s -p udp -s %s/%d --sport %d --dport %d -j ACCEPT\n",
				wanface, wanaddr, wancidr, RIP_PORT, RIP_PORT);
	}

	/* Ticket #7696 support MAP-E by seal */
	mapi_tmp_get(tid, "TMP_WAN_MAPE_FIREWALL", mape_firewall, sizeof(mape_firewall));

	/* Clamp TCP MSS to PMTU of WAN interface */
	if ((mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") ||
			mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "router")) &&
			mapi_ccfg_match_str(tid, "ARC_WAN_0_IP4_Proto", "pppoe"))
	{
		for(i=0; i<PPPOE_MAX_COUNT; i++)
		{
			char glbcfg[64];
			snprintf(glbcfg, sizeof(glbcfg), "ARC_WAN_%d_PPPOE_Enable", i);
			if(mapi_ccfg_match_str(tid, glbcfg, "1"))
			{
				char multi_wan_ifname[32];
				snprintf(glbcfg, sizeof(glbcfg), "ARC_WAN_%d_Iface", i);
				mapi_ccfg_get_str(tid, glbcfg, multi_wan_ifname, sizeof(multi_wan_ifname));
				if(strcmp(multi_wan_ifname, ""))
				{
					mtu = netif_get_mtu(multi_wan_ifname);
					if(mtu < DEFMRU && mtu > MINMRU)
					{
						append_rule(GENERIC_MODULE_RULES,  FILTER_TABLE, "-A FORWARD -o %s -p tcp --tcp-flags SYN,RST SYN -m tcpmss "
								"--mss %d: -j TCPMSS --set-mss %d\n", multi_wan_ifname, mtu-39, mtu-40);
					}
				}
			}
		}
		mru = atoi(mapi_ccfg_get_str(tid, strcat_r(prefix, "PPPOE_MRU", var, CCFG_NAME_LEN), buffer, sizeof(buffer)));
		if(mru < DEFMRU && mru > MINMRU)
		{
			append_rule(GENERIC_MODULE_RULES,  FILTER_TABLE, "-A FORWARD -o %s -p tcp --tcp-flags SYN,RST SYN -m tcpmss "
					"--mss %d: -j TCPMSS --set-mss %d\n", lanface, mru-39, mru-40);
		}
	}
	/* Ticket #7696 support MAP-E by seal */
	else if(mapi_ccfg_match_str(tid, "ARC_WAN_0_IP4_Proto", "v6plus") && strlen(mape_firewall) > 0)
	{
		append_rule(GENERIC_MODULE_RULES,  FILTER_TABLE, "-A FORWARD -p tcp -m tcp --tcp-flags SYN,RST SYN -m tcpmss "
				"--mss 1429: -j TCPMSS --set-mss 1428\n");
	}
	else
	{
		mtu = atoi(mapi_ccfg_get_str(tid, strcat_r(prefix, "GET_MTU", var, CCFG_NAME_LEN), buffer, sizeof(buffer)));
		// [bug fixed]: firewall is not work on LTE wan
		if(mtu < DEFMRU && mtu > MINMRU)
		{
			append_rule(GENERIC_MODULE_RULES,  FILTER_TABLE, "-A FORWARD -p tcp --tcp-flags SYN,RST SYN -m tcpmss "
					"--mss %d: -j TCPMSS --set-mss %d\n", mtu-39, mtu-40);
		}
	}

	if(!natenable)
	{
		append_rule(GENERIC_MODULE_RULES, FILTER_TABLE, "-A INPUT -p tcp -m tcp --dport %d -j ACCEPT\n", web_lanport);
	}
}

static void rules_in_nat_tb()
{
	char glbcfg[64];
	int i;
	char mape_firewall[256] = "";

	if(mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "bridge") || (mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") && mapi_ccfg_match_str(tid, "ARC_WAN_0_BridgeEnable", "1")) || mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "wb"))
	{ /* Added nat rules about POSTROUTING when system mode is bridge or wb. */
		append_rule(GENERIC_MODULE_RULES,  NAT_TABLE, "-A POSTROUTING -o %s -j MASQUERADE\n", lanface);
		return;
	}

	/* Ticket #7696 support MAP-E by seal */
	mapi_tmp_get(tid, "TMP_WAN_MAPE_FIREWALL", mape_firewall, sizeof(mape_firewall));

	// -A POSTROUTING -o eth1 -j MASQUERADE
	if(natenable)
	{
		if(unnumbered_enable)
			append_rule(GENERIC_MODULE_RULES,  NAT_TABLE, "-A POSTROUTING ! -s %s/%d -o %s -j MASQUERADE\n", unnumbered_lanaddr, unnumbered_lancidr, wanface);
		else
		{
			if(mapi_ccfg_match_str(tid, "ARC_WAN_0_IP4_Proto", "pppoe"))
				append_rule(GENERIC_MODULE_RULES,  NAT_TABLE, "-A POSTROUTING -o %s -j MASQUERADE\n", wanface);
			/* Ticket #7696 support MAP-E by seal */
			else if(mapi_ccfg_match_str(tid, "ARC_WAN_0_IP4_Proto", "v6plus") && strlen(mape_firewall) > 0)
			{
				append_rule(GENERIC_MODULE_RULES,  NAT_TABLE, "-A POSTROUTING -j MASQUERADE %s\n", mape_firewall);
			}
			else
			{
				if((strcmp(wanaddr, "") == 0) || (strcmp(wanaddr, "0.0.0.0") == 0))
				{
					append_rule(GENERIC_MODULE_RULES,  NAT_TABLE, "-A POSTROUTING -o %s -j MASQUERADE\n", wanface);
				}
				else
				{
					append_rule(GENERIC_MODULE_RULES,  NAT_TABLE, "-A POSTROUTING -o %s -j SNAT --to-source %s\n", wanface, wanaddr);
				}
			}
		}

		/* Allow "LAN to LAN" loopback connection, DMZ and port-forwarding could match it. */
		append_rule(GENERIC_MODULE_RULES,  NAT_TABLE, "-A POSTROUTING -o %s -s %s/%d -d %s/%d -j MASQUERADE\n", 
			lanface, lanaddr, lancidr, lanaddr, lancidr);

		if ((mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "auto") ||
			mapi_ccfg_match_str(tid, "ARC_SYS_SwitchMode", "router")) &&
			mapi_ccfg_match_str(tid, "ARC_WAN_0_IP4_Proto", "pppoe"))
		{
			for(i=0; i<PPPOE_MAX_COUNT; i++)
			{
				if(i == dft_wan_idx)
					continue;

				snprintf(glbcfg, sizeof(glbcfg), "ARC_WAN_%d_PPPOE_Enable", i);
				if(mapi_ccfg_match_str(tid, glbcfg, "1"))
				{
					char multi_wan_ifname[32];
					snprintf(glbcfg, sizeof(glbcfg), "ARC_WAN_%d_Iface", i);
					mapi_ccfg_get_str(tid, glbcfg, multi_wan_ifname, sizeof(multi_wan_ifname));
					if(strcmp(multi_wan_ifname, ""))
					{
						append_rule(GENERIC_MODULE_RULES, NAT_TABLE, "-A POSTROUTING -o %s -j MASQUERADE\n", multi_wan_ifname);
					}
				}
			}
		}
	}

	/* ICMP packets are always redirected to INPUT chains */
	// -A PREROUTING -d wanaddr -p icmp -j DNAT --to-destination 192.168.2.1
	// append_rule(GENERIC_MODULE_RULES,  NAT_TABLE, "-A PREROUTING -p icmp -d %s -j DNAT --to-destination %s\n",
	//          wanaddr, mapi_ccfg_get_str(tid, "ARC_LAN_0_IP4_Addr", buffer, 128));
}

static void rules_in_mangle_tb()
{
	/*
	if(mapi_ccfg_match_str(tid, "ARC_WAN_0_NAT_Enable", "1"))
	{
		append_rule(GENERIC_MODULE_RULES,  MANGLE_TABLE, "-A PREROUTING -i %s -d %s/%d -j DROP\n", wanface, lanaddr, lancidr);
	}
	*/
}
