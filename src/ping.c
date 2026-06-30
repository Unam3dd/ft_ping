/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 22:47:57 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/06/30 22:56:11 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

/////////////////////////////////////
//
//			ICMP
//
////////////////////////////////////

int send_ping(const context_t *ctx, const sin_t *dst, const icmphdr_t *icmp)
{
	if (!ctx || !dst || !icmp)
		return (1);

	if (sendto(ctx->fd, icmp, sizeof(icmp), 0, (struct sockaddr*)dst, sizeof(struct sockaddr)) < 0) {
		perror("send_ping");
		return (1);
	}

	return (0);
}
