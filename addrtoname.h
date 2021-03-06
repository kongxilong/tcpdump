/*
 * Copyright (c) 1990, 1992, 1993, 1994, 1995, 1996, 1997
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that: (1) source code distributions
 * retain the above copyright notice and this paragraph in its entirety, (2)
 * distributions including binary code include the above copyright notice and
 * this paragraph in its entirety in the documentation or other materials
 * provided with the distribution, and (3) all advertising materials mentioning
 * features or use of this software display the following acknowledgement:
 * ``This product includes software developed by the University of California,
 * Lawrence Berkeley Laboratory and its contributors.'' Neither the name of
 * the University nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior
 * written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*
 * Definitions to let us compile most of the IPv6 code even on systems
 * without IPv6 support.
 */

#include "extract.h"

#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN	46
#endif

/* Name to address translation routines. */

enum {
    LINKADDR_ETHER,
    LINKADDR_FRELAY,
    LINKADDR_IEEE1394,
    LINKADDR_ATM,
    LINKADDR_OTHER
};

#define BUFSIZE 128

extern const char *linkaddr_string(netdissect_options *, const uint8_t *, const unsigned int, const unsigned int);
extern const char *etheraddr_string(netdissect_options *, const uint8_t *);
extern const char *le64addr_string(netdissect_options *, const uint8_t *);
extern const char *tcpport_string(netdissect_options *, u_short);
extern const char *udpport_string(netdissect_options *, u_short);
extern const char *isonsap_string(netdissect_options *, const uint8_t *, u_int);
extern const char *dnaddr_string(netdissect_options *, u_short);
extern const char *ipxsap_string(netdissect_options *, u_short);
extern const char *ipaddr_string(netdissect_options *, const u_char *);
extern const char *ip6addr_string(netdissect_options *, const u_char *);
extern const char *intoa(uint32_t);

extern void init_addrtoname(netdissect_options *, uint32_t, uint32_t);
extern struct hnamemem *newhnamemem(netdissect_options *);
extern struct h6namemem *newh6namemem(netdissect_options *);
extern const char * ieee8021q_tci_string(const uint16_t);

/* macro(s) and inline function(s) with setjmp/longjmp logic to call
 * the X_string() function(s) after bounds checking.
 * The macro(s) must be used on a packet buffer pointer.
 */

static inline const char *
get_ipaddr_string(netdissect_options *ndo, const u_char *p)
{
        if (!ND_TTEST_4(p))
                longjmp(ndo->ndo_truncated, 1);
        return ipaddr_string(ndo, p);
}

static inline const char *
get_ip6addr_string(netdissect_options *ndo, const u_char *p)
{
        if (!ND_TTEST_16(p))
                longjmp(ndo->ndo_truncated, 1);
        return ip6addr_string(ndo, p);
}

static inline const char *
get_le64addr_string(netdissect_options *ndo, const u_char *p)
{
        if (!ND_TTEST_8(p))
                longjmp(ndo->ndo_truncated, 1);
        return le64addr_string(ndo, p);
}

#define GET_IPADDR_STRING(p) get_ipaddr_string(ndo, (const u_char *)(p))
#define GET_IP6ADDR_STRING(p) get_ip6addr_string(ndo, (const u_char *)(p))
#define GET_LE64ADDR_STRING(p) get_le64addr_string(ndo, (const u_char *)(p))
