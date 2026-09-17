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

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

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

    if (status || !res)
    {
        fprintf(stderr, "ft_ping: unknown host\n");
        if (!status)
            freeaddrinfo(res);
        return (1);
    }

    if (sin)
        *sin = *(sin_t *)res->ai_addr;

    freeaddrinfo(res);

    return (0);
}
