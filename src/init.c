/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 22:45:20 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/06/25 23:15:36 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"

/////////////////////////////////////
//
//			INCLUDES
//
////////////////////////////////////

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netdb.h>

/////////////////////////////////////
//
//			INIT
//
////////////////////////////////////

int init_context(context_t *ctx, const char *dest)
{
	if (!ctx)
		return (1);

	if (ctx->fd)
		return (0);

	ctx->fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	if (ctx->fd < 0) {
		perror("socket");
		return (1);
	}

	return (0);
}
