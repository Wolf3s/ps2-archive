#ifndef _EE_PS2IP_H
#define _EE_PS2IP_H

#include "tcpip.h"

int ps2ip_init();
int accept(int s, struct sockaddr *addr, int *addrlen);
int bind(int s, struct sockaddr *name, int namelen);
int disconnect(int s);
int connect(int s, struct sockaddr *name, int namelen);
int listen(int s, int backlog);
int recv(int s, void *mem, int len, unsigned int flags);
int recvfrom(int s, void *mem, int len, unsigned int flags, struct sockaddr *from, int *fromlen);
int send(int s, void *dataptr, int size, unsigned int flags);
int sendto(int s, void *dataptr, int size, unsigned int flags, struct sockaddr *to, int tolen);
int socket(int domain, int type, int protocol);
int ps2ip_setconfig(t_ip_info *ip_info);
int ps2ip_getconfig(char *netif_name, t_ip_info *ip_info);
int select(int maxfdp1, struct fd_set *readset, struct fd_set *writeset, struct fd_set *exceptset, struct timeval *timeout);
int ioctlsocket(int s, long cmd, void *argp);

#endif /* _EE_PS2IP_H */