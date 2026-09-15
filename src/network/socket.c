/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 08:58:04 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/07/06 17:02:21 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_ping.h"
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

/////////////////////////////////////
//
//			SOCKET
//
////////////////////////////////////

int create_socket(void)
{
	fd_t fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	if (fd < 0) {
		if (errno == EPERM || errno == EACCES)
			fprintf(stderr, "ft_ping: Lacking privilege for icmp socket.\n");
		else
			fprintf(stderr, "ft_ping: socket: %s\n", strerror(errno));
		return (-1);
	}

	return (fd);
}

int set_socket_ttl(fd_t fd, uint32_t ttl)
{
	if (fd < 0 || !ttl)
		return (-1);

	int val = (int)ttl;

	if (setsockopt(fd, IPPROTO_IP, IP_TTL, &val, sizeof(val)) < 0) {
		fprintf(stderr, "ft_ping: setsockopt(IP_TTL): %s\n",
			strerror(errno));
		return (-1);
	}
	
	return (0);
}
