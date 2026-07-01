/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 09:13:07 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/07/01 18:00:10 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <netinet/icmp6.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

/////////////////////////////////////
//
//			ECHO
//
////////////////////////////////////

int 	send_echo(const fd_t fd, const sin_t *dst, icmp_packet_t *pkt)
{
	if (!dst || !pkt)
		return (-1);

	pkt->hdr.checksum = 0;
	pkt->hdr.code = 0;
	pkt->hdr.type = ICMP_ECHO;
	gettimeofday(&pkt->timestamp, NULL);

	pkt->hdr.checksum = checksum(pkt, sizeof(icmp_packet_t));

	return (sendto(fd, pkt, sizeof(icmp_packet_t), 0, (const struct sockaddr *)dst, sizeof(sin_t)));
}

int recv_echo(const fd_t fd, const context_t *ctx)
{
	if (fd < 0 || !ctx)
		return (-1);

	char buf[2048];
	struct iphdr *iphdr = NULL;
	icmphdr_t *icmphdr = NULL;
	sin_t sin;
	socklen_t len = sizeof(sin);
	int bytes = 0;

	memset(&sin, 0, sizeof(sin_t));
	memset(buf, 0, sizeof(buf));

	bytes = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&sin, &len);

	if (bytes < 0) {
		perror("recvfrom");
		return (-1);
	}

	iphdr = (struct iphdr *)buf;
	icmphdr = (icmphdr_t *)(buf + (iphdr->ihl * 4));

	if (icmphdr->type == ICMP_ECHOREPLY && icmphdr->un.echo.id == (getpid() & 0xFFFF)) {
		if (!icmphdr->code)
			display_response(ctx, (const icmphdr_t*)icmphdr, sizeof(icmp_packet_t));

		// handle error here
	}

	return (bytes);
}
