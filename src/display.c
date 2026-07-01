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
#include <stdio.h>

/////////////////////////////////////
//
//			DISPLAY
//
////////////////////////////////////

void display_response(const context_t *ctx, const icmphdr_t *hdr, const size_t size)
{
	if (!ctx || !hdr)
		return ;

	printf("%ld bytes from %s: icmp_seq=%d\n", size, inet_ntoa(ctx->dst.sin_addr), ctx->seq);
	fflush(stdout);
}
