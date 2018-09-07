#ifndef _FIREWALL_H
#define _FIREWALL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <dirent.h>
#include <ctype.h>
#include <utils.h>
#include <time.h>
#include <signal.h>
#include <syslog.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include "mid_mapi_midcore.h"
#include "argtool.h"
#include "msg_def.h"


#define uint unsigned int

#define FILTER_TABLE  "filter"
#define NAT_TABLE     "nat"
#define MANGLE_TABLE  "mangle"

#define IGMP_PROTO    "2"

#define PPTP_PORT   1723  /* TCP */
#define L2TP_PORT   1701  /* UDP */
#define ISAKMP_PORT   500 /* UDP */

/* Known port */
#define DNS_PORT                53      /* UDP */
#define TFTP_PORT               69      /* UDP */
#define ISAKMP_PORT             500     /* UDP */
#define RIP_PORT                520     /* UDP */
#define L2TP_PORT               1701    /* UDP */
#define HTTP_PORT               80      /* TCP */
#define IDENT_PORT              113     /* TCP */
#define HTTPS_PORT              443     /* TCP */
#define PPTP_PORT               1723    /* TCP */
#define PROXY_PORT              "80,3128,8080,8000"     /* TCP */

#define IP_MULTICAST            "224.0.0.0/4"
#ifdef FLOOD_PROTECT
#define FLOOD_RATE      200
#define LOG_FLOOD_RATE  "10/min"
#define TARG_PASS       "limaccept" 
#define TARG_RST        "logreject"
#define TARG_TRIG       "logtrig"
#else
#define TARG_PASS       "ACCEPT"
#define TARG_RST        "REJECT --reject-with tcp-reset"
#define TARG_TRIG       "ACCEPT"
#endif

#define BUFFER_SIZE_LINE  1024
#define BUFFER_SIZE_MAC   20
#define BUFFER_SIZE_IP    16
#define BUFFER_SIZE_PROTO   8
#define BUFFER_SIZE_PORTS   64

#define LENGTH_STR_IP_MIN   7   //Format: 1.1.1.1
#define LENGTH_STR_IP_MAX   15  //Format: 192.168.106.254

#define STR_TCP       "tcp"
#define STR_UDP       "udp"

#if 0
#define dprintf(fmt, args...) cprintf("%s: " fmt, __func__, ## args)
#else
#define dprintf(fmt, args...)
#endif

#define MAX_CHAIN_NAME	128

#define PPPOE_MAX_COUNT         5

typedef struct double_list
{
	struct double_list * prev;
	struct double_list * next;
}double_list_t, * double_list_p;

typedef struct iptable_chain
{
	double_list_t list_entry; //Must be first member
	char chain[MAX_CHAIN_NAME];
}iptable_chain_t, * iptable_chain_p;

/* struct module_chains - chains name info of module
 * @filter_chains: name info of filter chains
 * @nat_chains: name info of nat chains
 * @mangle_chains: name info of mangle chains
 */
typedef struct module_chains
{
	double_list_t filter_chains;
	double_list_t nat_chains;
	double_list_t mangle_chains;
}module_chains_t, * module_chains_p;


typedef struct iptable_rule
{
	char rule[256];
	struct iptable_rule* next;
}iptable_rule_t;


/* struct module_rules - rules info of module
 * @filter_rules_head: head of rule chain of filter
 * @filter_rules_tail: tail of rule chain of filter, used when appending rule
 * @nat_rules_head: head of rule chain of nat
 * @nat_rules_tail: tail of rule chain of nat, used when appending rule
 * @mangle_rules_head: head of rule chain of mangle
 * @mangle_rules_tail: tail of rule chain of mangle, used when appending rule
 */
typedef struct module_rules
{
	iptable_rule_t* filter_rules_head;
	iptable_rule_t* filter_rules_tail;
	iptable_rule_t* nat_rules_head;
	iptable_rule_t* nat_rules_tail;
	iptable_rule_t* mangle_rules_head;
	iptable_rule_t* mangle_rules_tail;
}module_rules_t;


typedef struct conntrack_info
{
	uint32_t orig_src;
	uint32_t orig_dst;
	uint32_t reply_src;
	uint32_t reply_dst;
	uint16_t orig_port_src;
	uint16_t orig_port_dst;
	uint16_t orig_port_dst_range;
	uint16_t reply_port_src;
	uint16_t reply_port_src_range;
	uint16_t reply_port_dst;
	uint8_t proto;
}conntrack_info_t;

typedef struct conntrack_rules
{
	conntrack_info_t rule_data;
	struct conntrack_rules * next;
}conntrack_rules_t;

typedef struct module_conntrack
{
	conntrack_rules_t * conntrack_rules_head;
	conntrack_rules_t * conntrack_rules_tail;
}module_conntrack_t;

/* struct module - module info
 * @module_name: name of module
 * @module_desc: description of module
 * @filter_tb_routine: point to routine which is used to operate rules in filter table
 * @filter_tb_routine: point to routine which is used to operate rules in nat table
 * @filter_tb_routine: point to routine which is used to operate rules in mangle_tb_routine table
 */
typedef struct module
{
	char                    module_name[32];
	char                    module_desc[256];
	module_rules_t          mod_rules;
	module_chains_t			mod_chains;
	module_conntrack_t		mod_conntrack;
	void					(*conntrack_tb_routine)(void);
	void                    (*filter_tb_routine)(void);
	void                    (*nat_tb_routine)(void);
	void                    (*mangle_tb_routine)(void);
}module_t;

void append_rule(module_rules_t* mod_rules, char* table_name, const char *fmt,...);
void append_chain(module_chains_p mod_chains, const char * table_name, const char * chain_name);
void add_conntrack_rule(module_conntrack_t * mod_conntrack, conntrack_info_t new_rule);
void compare_and_clean_conntrack(module_conntrack_t * old_conntrack, module_conntrack_t * new_conntrack);
void update_timer(time_t interval);
int netmask_to_cidr(char *netmask);

#endif
