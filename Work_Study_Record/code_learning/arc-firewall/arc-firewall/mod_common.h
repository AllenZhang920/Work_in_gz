#ifndef _MOD_COMMON_H
#define _MOD_COMMON_H

extern int tid;
extern char wanface[IFNAMSIZ];
extern char wanaddr[];
extern char lanaddr[];
extern char unnumbered_lanaddr[];
extern int wancidr;
extern int lancidr;
extern int unnumbered_lancidr;
extern char lanface[IFNAMSIZ];
extern int firewallenable;
extern int natenable;
extern int dmzenable;
extern int triggerenable;
extern int remotemanage;
extern int upnpenable;
extern int unnumbered_enable;
extern int web_lanport;
extern int dft_wan_idx;

#endif
