/*
 * ps2ip.h - Imports and definitions for ps2ip.
 *
 * Copyright (c) 2003 Marcus R. Brown <mrbrown@0xd6.org>
 *
 * See the file LICENSE included with this distribution for licensing terms.
 */

/* Some portions of this header fall under the following copyright.  The license
   is compatible with that of ps2sdk.  */

/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#ifndef PS2IP_H
#define PS2IP_H


struct ip_addr {
  u32 addr;
};

#define IP4_ADDR(ipaddr, a,b,c,d) (ipaddr)->addr = htonl(((u32)(a & 0xff) << 24) | ((u32)(b & 0xff) << 16) | \
                                                         ((u32)(c & 0xff) << 8) | (u32)(d & 0xff))

#define ip_addr_set(dest, src) (dest)->addr = ((struct ip_addr *)src)->addr
#define ip_addr_maskcmp(addr1, addr2, mask) (((addr1)->addr & \
                                              (mask)->addr) == \
                                             ((addr2)->addr & \
                                              (mask)->addr))
#define ip_addr_cmp(addr1, addr2) ((addr1)->addr == (addr2)->addr)

#define ip_addr_isany(addr1) ((addr1) == NULL || (addr1)->addr == 0)

#define ip_addr_isbroadcast(addr1, mask) (((((addr1)->addr) & ~((mask)->addr)) == \
					 (0xffffffff & ~((mask)->addr))) || \
                                         ((addr1)->addr == 0xffffffff) || \
                                         ((addr1)->addr == 0x00000000))

#define ip_addr_ismulticast(addr1) (((addr1)->addr & ntohl(0xf0000000)) == ntohl(0xe0000000))
				   
#define ip4_addr1(ipaddr) ((u8)(ntohl((ipaddr)->addr) >> 24) & 0xff)
#define ip4_addr2(ipaddr) ((u8)(ntohl((ipaddr)->addr) >> 16) & 0xff)
#define ip4_addr3(ipaddr) ((u8)(ntohl((ipaddr)->addr) >> 8) & 0xff)
#define ip4_addr4(ipaddr) ((u8)(ntohl((ipaddr)->addr)) & 0xff)


struct in_addr {
  u32 s_addr;
};

struct sockaddr_in {
  u8 sin_len;
  u8 sin_family;
  u16 sin_port;
  struct in_addr sin_addr;
  char sin_zero[8];
};

struct sockaddr {
  u8 sa_len;
  u8 sa_family;
  char sa_data[14];
};

#ifndef socklen_t
#  define socklen_t int
#endif

#define	SOCK_STREAM     1
#define	SOCK_DGRAM      2
#define	SOCK_RAW        3

/*
 * Option flags per-socket.
 */
#define	SO_DEBUG				0x0001		/* turn on debugging info recording */
#define	SO_ACCEPTCONN		0x0002		/* socket has had listen() */
#define	SO_REUSEADDR		0x0004		/* allow local address reuse */
#define	SO_KEEPALIVE		0x0008		/* keep connections alive */
#define	SO_DONTROUTE		0x0010		/* just use interface addresses */
#define	SO_BROADCAST		0x0020		/* permit sending of broadcast msgs */
#define	SO_USELOOPBACK		0x0040		/* bypass hardware when possible */
#define	SO_LINGER			0x0080		/* linger on close if data present */
#define	SO_OOBINLINE		0x0100		/* leave received OOB data in line */

#define	SO_DONTLINGER   (int)(~SO_LINGER)

/*
 * Additional options, not kept in so_options.
 */
#define	SO_SNDBUF		0x1001		/* send buffer size */
#define	SO_RCVBUF		0x1002		/* receive buffer size */
#define	SO_SNDLOWAT		0x1003		/* send low-water mark */
#define	SO_RCVLOWAT		0x1004		/* receive low-water mark */
#define	SO_SNDTIMEO		0x1005		/* send timeout */
#define	SO_RCVTIMEO		0x1006		/* receive timeout */
#define	SO_ERROR			0x1007		/* get error status and clear */
#define	SO_TYPE			0x1008		/* get socket type */

/*
 * Level number for (get/set)sockopt() to apply to socket itself.
 */
#define	SOL_SOCKET	0xfff		/* options for socket level */

#define	AF_UNSPEC	0
#define	AF_INET		2
#define	PF_INET		AF_INET

#define	IPPROTO_TCP		6
#define	IPPROTO_UDP		17

#define	INADDR_ANY			0
#define	INADDR_BROADCAST	0xffffffff

/* Flags we can use with send and recv. */
#define	MSG_DONTWAIT	0x40            /* Nonblocking i/o for this operation only */

#ifndef FD_SET
#	undef  FD_SETSIZE
#	define FD_SETSIZE    16
#	define FD_SET(n, p)  ((p)->fd_bits[(n)/8] |=  (1 << ((n) & 7)))
#	define FD_CLR(n, p)  ((p)->fd_bits[(n)/8] &= ~(1 << ((n) & 7)))
#	define FD_ISSET(n,p) ((p)->fd_bits[(n)/8] &   (1 << ((n) & 7)))
#	define FD_ZERO(p)    memset((void*)(p),0,sizeof(*(p)))

  typedef struct fd_set {
          unsigned char fd_bits [(FD_SETSIZE+7)/8];
  } fd_set;

  struct timeval {
	  long    tv_sec;         /* seconds */
	  long    tv_usec;        /* and microseconds */
  };

#endif

int       lwip_accept(int s, struct sockaddr *addr, socklen_t *addrlen);
#define I_lwip_accept DECLARE_IMPORT(4, lwip_accept)
int       lwip_bind(int s, struct sockaddr *name, socklen_t namelen);
#define I_lwip_bind DECLARE_IMPORT(5, lwip_bind)
int       lwip_close(int s);
#define I_lwip_close DECLARE_IMPORT(6, lwip_close)
int       lwip_connect(int s, struct sockaddr *name, socklen_t namelen);
#define I_lwip_connect DECLARE_IMPORT(7, lwip_connect)
int       lwip_listen(int s, int backlog);
#define I_lwip_listen DECLARE_IMPORT(8, lwip_listen)
int       lwip_recv(int s, void *mem, int len, unsigned int flags);
#define I_lwip_recv DECLARE_IMPORT(9, lwip_recv)
int       lwip_recvfrom(int s, void *mem, int len, unsigned int flags,
                        struct sockaddr *from, socklen_t *fromlen);
#define I_lwip_recvfrom DECLARE_IMPORT(10, lwip_recvfrom)
int       lwip_send(int s, void *dataptr, int size, unsigned int flags);
#define I_lwip_send DECLARE_IMPORT(11, lwip_send)
int       lwip_sendto(int s, void *dataptr, int size, unsigned int flags,
                      struct sockaddr *to, socklen_t tolen);
#define I_lwip_sendto DECLARE_IMPORT(12, lwip_sendto)
int       lwip_socket(int domain, int type, int protocol);
#define I_lwip_socket DECLARE_IMPORT(13, lwip_socket)
int       lwip_select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,
                      struct timeval *timeout);
#define I_lwip_select DECLARE_IMPORT(14, lwip_select)
int       lwip_ioctl(int s, long cmd, void *argp);
#define I_lwip_ioctl DECLARE_IMPORT(15, lwip_ioctl)
int       lwip_getpeername (int s, struct sockaddr *name, socklen_t *namelen);
#define I_lwip_getpeername DECLARE_IMPORT(16, lwip_getpeername)
int       lwip_getsockname (int s, struct sockaddr *name, socklen_t *namelen);
#define I_lwip_getsockname DECLARE_IMPORT(17, lwip_getsockname)
int       lwip_getsockopt (int s, int level, int optname, void *optval, socklen_t *optlen);
#define I_lwip_getsockopt DECLARE_IMPORT(18, lwip_getsockopt)
int       lwip_setsockopt (int s, int level, int optname, const void *optval, socklen_t optlen);
#define I_lwip_setsockopt DECLARE_IMPORT(19, lwip_setsockopt)

/* Compatibility macros.  */

#define	accept			lwip_accept
#define	bind				lwip_bind
#define	disconnect		lwip_close
#define	connect			lwip_connect
#define	listen			lwip_listen
#define	recv				lwip_recv
#define	recvfrom			lwip_recvfrom
#define	send				lwip_send
#define	sendto			lwip_sendto
#define	socket			lwip_socket
#define	select			lwip_select
#define	ioctlsocket		lwip_ioctl


#if		!defined(INVALID_SOCKET)
#	define	INVALID_SOCKET -1
#endif

#if		!defined(htonl)
#	define	htonl(x)		((((x)&0xff)<<24)|(((x)&0xff00)<<8)|(((x)&0xff0000)>>8)|(((x)&0xff000000)>>24))
#endif
#if		!defined(htons)
#	define	htons(x)		((((x)&0xff)<<8)|(((x)&0xff00)>>8))
#endif
#if		!defined(ntohl)
#	define	ntohl(x)		htonl(x)
#endif
#if		!defined(ntohs)
#	define	ntohs(x)		htons(x)
#endif


typedef struct
{
	char					netif_name[4];
	struct in_addr		ipaddr;
	struct in_addr		netmask;
	struct in_addr		gw;
	u32					dhcp_enabled;
	u32					dhcp_status;
	u8						hw_addr[8];
} t_ip_info;


#endif /* IOP_PS2IP_H */
