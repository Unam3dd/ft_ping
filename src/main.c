/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 16:49:12 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/09/15 10:40:00 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"
#include <arpa/inet.h>
#include <stdio.h>

/////////////////////////////////////
//
//			MAIN
//
////////////////////////////////////

int main(int ac, char **av)
{
    opt_t *options = get_options(NULL);
    context_t ctx = {.sin.sin_addr.s_addr = 0,
                     .sin.sin_port = 0,
                     .sin.sin_family = 0,
                     .sin.sin_zero = {0, 0, 0, 0, 0, 0, 0, 0}};

    int ret = parse_arguments(ac, av, options);
    int failed = 0;
    int i = 0;

    if (ret == PARSE_DONE)
        return (0);

    if (ret == PARSE_USAGE)
        return (EXIT_USAGE);

    if (ret != PARSE_OK)
        return (1);

    i = get_first_host_index();
    while (i < ac)
    {
        options[OPT_HOST_INDEX].str = av[i];

        ret = ping_program(&ctx, av[i]);

        if (ret == PING_FATAL)
            return (1);

        if (ret)
            failed = 1;

        i++;
    }

    return (failed);
}
