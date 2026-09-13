/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 15:12:55 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/07/13 07:40:00 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_ping.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

/////////////////////////////////////
//
//			STATIC
//
////////////////////////////////////

static int show_reply(iphdr_t *ip, icmp_pkt_t *pkt, rtt_t *rtt, int icmplen)
{
	double	ms;
	char	src[INET_ADDRSTRLEN];

	if (!ip || !pkt || !rtt || icmplen <= 0)
		return (-1);
	if (pkt->h.un.echo.id != (getpid() & 0xFFFF))
		return (1);
	ms = get_ms(&pkt->t);
	rtt_add(rtt, ms);
	memset(src, 0, sizeof(src));
	inet_ntop(AF_INET, &ip->saddr, src, sizeof(src));
	printf("%d bytes from %s: icmp_seq=%u ttl=%d time=%.3f ms\n",
		icmplen,
		src,
		(unsigned)BIG16(pkt->h.un.echo.sequence),
		ip->ttl,
		ms);
	return (0);
}

static int show_response(context_t *ctx, const char *buf, int size)
{
	iphdr_t		*ip;
	icmphdr_t	*icmp;
	int		hlen;
	int		icmplen;

	if (!ctx || !buf || size <= 0)
		return (-1);
	ip = (iphdr_t *)buf;
	hlen = ip->ihl * 4;
	icmp = (icmphdr_t *)(buf + hlen);
	icmplen = size - hlen;
	if (icmp->type == ICMP_ECHOREPLY)
		return (show_reply(ip, (icmp_pkt_t *)icmp, &ctx->rtt, icmplen));
	if (icmp->type == ICMP_DEST_UNREACH || icmp->type == ICMP_TIME_EXCEEDED)
		return (handle_icmp_error(ctx, buf, size));
	return (1);
}

/////////////////////////////////////
//
//			ICMP
//
////////////////////////////////////

int send_icmp_echo(context_t *ctx, const fd_t fd, const sin_t *dst)
{
	static uint64_t	seq = BIG16(0x0);
	icmp_pkt_t		pkt;
	int			bytes;

	if (fd < 0 || !dst || !ctx)
		return (-1);
	memset(&pkt, 0, sizeof(icmp_pkt_t));
	pkt.h.un.echo.id = getpid() & 0xFFFF;
	pkt.h.un.echo.sequence = seq;
	pkt.h.code = 0;
	pkt.h.type = ICMP_ECHO;
	memcpy(pkt.data, "\x00\x01\x02\x03\x04\x05\x06"
		"\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
		"\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19"
		"\x1a\x1b\x1c\x1d\x1e\x1f !\"#$%&'", sizeof(pkt.data));
	if (gettimeofday(&pkt.t, NULL) < 0)
		return (-1);
	pkt.h.checksum = checksum(&pkt, sizeof(icmp_pkt_t));
	bytes = sendto(fd, &pkt, sizeof(icmp_pkt_t), 0,
			(struct sockaddr *)dst, sizeof(sin_t));
	if (bytes < 0)
		return (-1);
	seq = BIG16(seq);
	seq++;
	seq = BIG16(seq);
	return (bytes);
}

int recv_icmp_echo(context_t *ctx)
{
	char		buf[0x100];
	socklen_t	len;
	int		bytes;

	if (!ctx || ctx->fd < 0)
		return (-1);
	len = 0;
	memset(buf, 0, sizeof(buf));
	bytes = recvfrom(ctx->fd, buf, sizeof(buf), 0, NULL, &len);
	if (bytes < 0)
		return (-1);
	return (show_response(ctx, buf, bytes));
}
