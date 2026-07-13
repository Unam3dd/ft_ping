/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   host.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 12:47:17 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/07/06 09:08:17 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../inc/ft_ping.h"

/////////////////////////////////////
//
//			INCLUDES
//
////////////////////////////////////

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>

/////////////////////////////////////
//
//			HOST
//
////////////////////////////////////

int resolve_host(const char *host, sin_t *sin)
{
	if (!host)
		return (1);

	struct addrinfo spec, *res = NULL;
	int status = 0;

	memset(&spec, 0, sizeof(struct addrinfo));

	spec.ai_family = AF_INET;
	status = getaddrinfo(host, NULL, &spec, &res);

	if (status) {
		fprintf(stderr, "Error: getaddrinfo: %s\n", gai_strerror(status));
		return (1);
	}

	if (sin && res)
		*sin = *(sin_t *)res->ai_addr;

	freeaddrinfo(res);

	return (0);
}

