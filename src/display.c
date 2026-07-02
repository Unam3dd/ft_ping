/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 17:47:08 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/07/01 18:00:05 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"
#include <arpa/inet.h>
#include <bits/types/struct_timeval.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <sys/time.h>

/////////////////////////////////////
//
//			STATIC
//
////////////////////////////////////

static double get_ms(void)
{
	struct timeval t;

	gettimeofday(&t, NULL);

	return ((double)t.tv_sec * 1000.0 + (double)t.tv_usec / 1000.0);
}

static uint16_t read_be(const uint16_t data)
{
	return ((data >> 8 | data << 8) & 0xFFFF);
}

/////////////////////////////////////
//
//			DISPLAY
//
////////////////////////////////////

void display_response(const context_t *ctx, const iphdr_t *iphdr, const icmp_packet_t *pkt, const size_t size)
{
	if (!ctx || !iphdr || !pkt)
		return ;

	if (pkt->hdr.type == ICMP_DEST_UNREACH) {
		printf("%ld bytes from %s: icmp_seq=%d Destination Host Unreachable\n", size, inet_ntoa(*(struct in_addr*)&iphdr->saddr), read_be(pkt->hdr.un.echo.sequence));
		return ;
	}

	printf("%ld bytes from %s: icmp_seq=%d ttl=%d time=%.1lf ms\n", size, inet_ntoa(*(struct in_addr*)&iphdr->saddr), read_be(pkt->hdr.un.echo.sequence), iphdr->ttl, (double)get_ms() - (pkt->timestamp.tv_sec * 1000.0 + (double)pkt->timestamp.tv_usec / 1000.0));
}
