/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verbose.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 07:23:04 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/07/13 07:40:00 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_ping.h"
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <unistd.h>

static const icmp_msg_t g_msgs[] = {
    {ICMP_DEST_UNREACH, ICMP_NET_UNREACH, "Destination Net Unreachable"},
    {ICMP_DEST_UNREACH, ICMP_HOST_UNREACH, "Destination Host Unreachable"},
    {ICMP_DEST_UNREACH, ICMP_PROT_UNREACH, "Destination Protocol Unreachable"},
    {ICMP_DEST_UNREACH, ICMP_PORT_UNREACH, "Destination Port Unreachable"},
    {ICMP_DEST_UNREACH, ICMP_FRAG_NEEDED, "Fragmentation needed and DF set"},
    {ICMP_DEST_UNREACH, ICMP_SR_FAILED, "Source Route Failed"},
    {ICMP_TIME_EXCEEDED, ICMP_EXC_TTL, "Time to live exceeded"},
    {ICMP_TIME_EXCEEDED, ICMP_EXC_FRAGTIME, "Frag reassembly time exceeded"},
};

static bool_t is_verbose(void)
{
    return (get_options(NULL)[OPT_VERBOSE_INDEX].b);
}

static const char *icmp_msg(uint8_t type, uint8_t code)
{
    size_t i = 0;

    for (i = 0; i < sizeof(g_msgs) / sizeof(g_msgs[0]); i++)
    {
        if (g_msgs[i].type == type && g_msgs[i].code == code)
            return (g_msgs[i].msg);
    }

    return (NULL);
}

static int our_echo(const icmphdr_t *embed)
{
    if (!embed || embed->type != ICMP_ECHO)
        return (1);

    return (embed->un.echo.id == htons((uint16_t)(getpid() & 0xFFFF)));
}

static int should_show(context_t *ctx, const iphdr_t *embed)
{
    if (!ctx || !embed)
        return (0);

    if (is_verbose())
        return (1);

    return (embed->daddr == ctx->sin.sin_addr.s_addr);
}

static void print_from(iphdr_t *outer, int icmplen, const char *msg)
{
    char ip[INET_ADDRSTRLEN];

    if (!outer || !msg || icmplen <= 0)
        return;

    memset(ip, 0, sizeof(ip));

    inet_ntop(AF_INET, &outer->saddr, ip, sizeof(ip));
    printf("%d bytes from %s: %s\n", icmplen, ip, msg);
}

static void print_error(iphdr_t *outer, int icmplen, uint8_t type, uint8_t code)
{
    char buf[0x40];
    const char *msg = NULL;

    if (!outer || icmplen <= 0)
        return;

    memset(buf, 0, sizeof(buf));

    msg = icmp_msg(type, code);

    if (msg)
        print_from(outer, icmplen, msg);
    else if (type == ICMP_DEST_UNREACH)
    {
        snprintf(buf, sizeof(buf), "Dest Unreachable, Unknown Code: %d", code);
        print_from(outer, icmplen, buf);
    }
    else if (type == ICMP_TIME_EXCEEDED)
    {
        snprintf(buf, sizeof(buf), "Time exceeded, Unknown Code: %d", code);
        print_from(outer, icmplen, buf);
    }
}

void verbose_dump(iphdr_t *embed_ip, icmphdr_t *embed_icmp)
{
    size_t i = 0;
    size_t hlen = 0;
    uint16_t off = 0;
    int size = 0;
    char src[INET_ADDRSTRLEN];
    char dst[INET_ADDRSTRLEN];

    if (!is_verbose() || !embed_ip || !embed_icmp)
        return;

    hlen = sizeof(*embed_ip);
    
	printf("IP Hdr Dump:\n ");
    for (i = 0; i < hlen; i++)
        printf("%02x%s", ((unsigned char *)embed_ip)[i], (i % 2) ? " " : "");
    printf("\n");

    off = ntohs(embed_ip->frag_off);
    
	memset(src, 0, sizeof(src));
    memset(dst, 0, sizeof(dst));

    inet_ntop(AF_INET, &embed_ip->saddr, src, sizeof(src));
    inet_ntop(AF_INET, &embed_ip->daddr, dst, sizeof(dst));

    printf("Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src\tDst\tData\n");
    printf(" %1x  %1x  %02x %04x %04x   %1x %04x  %02x  %02x %04x %s  %s \n",
           embed_ip->version, embed_ip->ihl, embed_ip->tos,
           ntohs(embed_ip->tot_len), ntohs(embed_ip->id), (off & 0xe000) >> 13,
           off & 0x1fff, embed_ip->ttl, embed_ip->protocol,
           ntohs(embed_ip->check), src, dst);

    size = ntohs(embed_ip->tot_len) - (embed_ip->ihl * 4);
    printf("ICMP: type %u, code %u, size %u", embed_icmp->type, embed_icmp->code, size);
    
	if (embed_icmp->type == ICMP_ECHO || embed_icmp->type == ICMP_ECHOREPLY) {
        printf(", id 0x%04x, seq 0x%04x", ntohs(embed_icmp->un.echo.id),
               ntohs(embed_icmp->un.echo.sequence));
	}
    
	printf("\n");
}

int handle_icmp_error(context_t *ctx, const char *buf, int size)
{
    iphdr_t *outer = NULL;
    icmphdr_t *err = NULL;
    iphdr_t *embed_ip = NULL;
    icmphdr_t *embed_icmp = NULL;
    int outer_hlen = 0;
    int embed_off = 0;
    int embed_hlen = 0;

    if (!ctx || !buf || size <= 0)
        return (1);
    if (!ip_icmp_ok(buf, size, 0, &outer_hlen))
        return (1);

    outer = (iphdr_t *)buf;
    err = (icmphdr_t *)(buf + outer_hlen);
    embed_off = outer_hlen + (int)sizeof(icmphdr_t);
    if (!ip_icmp_ok(buf, size, embed_off, &embed_hlen))
        return (1);

    embed_ip = (iphdr_t *)(buf + embed_off);
    embed_icmp = (icmphdr_t *)(buf + embed_off + embed_hlen);

    if (!our_echo(embed_icmp))
        return (1);

    if (should_show(ctx, embed_ip))
    {
        print_error(outer, size - outer_hlen, err->type, err->code);
        verbose_dump(embed_ip, embed_icmp);
    }

    return (ICMP_OURS_ERROR);
}
