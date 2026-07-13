/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 16:49:12 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/07/09 08:08:47 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main(int ac, char **av)
{
	opt_t *options = get_options(NULL);
	context_t ctx= {
		.sin.sin_addr.s_addr = 0,
		.sin.sin_port = 0,
		.sin.sin_family = 0,
		.sin.sin_zero = { 0, 0, 0, 0, 0, 0, 0, 0 }
	};

	int ret = parse_arguments(ac, av, options);

	if (ret == 2)
		return (0);

	if (ret == 1)
		return (1);

	return (ping_program(&ctx, options[OPT_HOST_INDEX].value));
}
