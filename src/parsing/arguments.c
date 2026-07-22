/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 17:21:44 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/07/03 16:11:32 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/////////////////////////////////////
//
//			INCLUDES
//
////////////////////////////////////

#include "../../inc/ft_ping.h"
#include <bits/getopt_core.h>
#include <getopt.h>
#include <stdio.h>
#include <errno.h>

/////////////////////////////////////
//
//			PARSING
//
////////////////////////////////////

int parse_arguments(int ac, char **av, opt_t *option)
{
	static const struct option opt[] = {
		{ "count", required_argument, NULL, 'c' },
		{ "verbose", no_argument, NULL,     'v' },
		{ "version", no_argument, NULL,     'V' },
		{ "help", no_argument, NULL,        'h' },
		{ "usage", no_argument, NULL,       'u' },
		{ "ttl", required_argument, NULL,    't' },
		{ "numeric", no_argument, NULL,     'n' },
		{ "deadline", required_argument, NULL, 'w' },
		{ 0, 0, 0, 0 }
	};
	
	int o = 0;

	if (!option)
		return (1);

	while ((o = getopt_long(ac, av, "c:vVhut:nw:", opt, NULL)) != -1) {

		switch (o) {
			case 'c':
				errno = 0;
				option[OPT_COUNT_INDEX].u64 = parse_unumber(optarg);

				if (errno) {
					fprintf(stderr, "ft_ping: invalid value '%s'\n", optarg);
					return (1);
				}

				break;

			case 't':
				errno = 0;
				option[OPT_TTL_INDEX].u32 = (uint32_t)parse_unumber(optarg);

				if (errno) {
					fprintf(stderr, "ft_ping: invalid value '%s'\n", optarg);
					return (1);
				}

				if (option[OPT_TTL_INDEX].u32 < 1 || option[OPT_TTL_INDEX].u32 > 255) {
					
					if (option[OPT_TTL_INDEX].u32 < 1) {
						fprintf(stderr, "ft_ping: option value too small: %d\n", option[OPT_TTL_INDEX].u32);
						return (1);
					} else {
						fprintf(stderr, "ft_ping: option value too big: %d\n", option[OPT_TTL_INDEX].u32);
						return (1);
					}
				}
				break;

			case 'w':
				errno = 0;
				option[OPT_DEADLINE_INDEX].u64 = parse_unumber(optarg);

				if (errno) {
					fprintf(stderr, "ft_ping: invalid value '%s'\n", optarg);
					return (1);
				}

				if (option[OPT_DEADLINE_INDEX].u64 < 1) {
					fprintf(stderr, "ft_ping: option value too small: %lu\n",
						option[OPT_DEADLINE_INDEX].u64);
					return (1);
				}

				if (option[OPT_DEADLINE_INDEX].u64 > 2147483647ULL) {
					fprintf(stderr, "ft_ping: option value too big: %lu\n",
			               option[OPT_DEADLINE_INDEX].u64);
					return (1);
				}
				
				break;

			case 'v':
				option[OPT_VERBOSE_INDEX].bool = TRUE;
				break;

			case 'n':
				option[OPT_NUMERIC_INDEX].bool = TRUE;
				break;

			case 'V':
				show_version();
				return (2);

			case 'h':
			case 'u':
			case '?':
				show_usage();
				return (1);

			default:
				printf("Try 'ft_ping --help' or 'ft_ping --usage' for more information.\n");
				return (1);
		}
	}

	if (optind >= ac) {
		printf("ft_ping: missing host operand\nTry 'ft_ping --help' or 'ft_ping --usage' for more information.\n");
		return (1);
	}

	option[OPT_HOST_INDEX].str = av[optind];

	return (0);
}

