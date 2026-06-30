/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 12:10:09 by stales            #+#    #+#             */
/*   Updated: 2026/06/30 22:55:14 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>

static volatile int lock = 1;

static void sig_handler(int sig)
{
	if (sig == SIGINT)
		lock = 0;
}

int main(int ac, char **av)
{
	options_t opt = { 0 };
	context_t ctx = { 0 };

	icmphdr_t icmp = {
		.type = ICMP_ECHO,
		.code = 0,
    	.un.echo.id = getpid(),
		.un.echo.sequence = htons(1),
	};


	if (parse_arguments(ac, av, &opt))
		return (1);

	if (init_context(&ctx, av[1]))
		return (1);

	signal(SIGINT, sig_handler);

	while (lock) {
	
		icmp.checksum = checksum(&icmp, sizeof(icmp));

		if (send_ping(&ctx, &ctx.dst, &icmp))
			break;
		
		icmp.un.echo.sequence += htons(1);

		getc(stdin);
	}

	close_context(&ctx);

	return (0);
}
