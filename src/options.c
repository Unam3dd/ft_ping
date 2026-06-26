/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 22:35:18 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/06/25 23:04:30 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"

/////////////////////////////////////
//
//			INCLUDES
//
////////////////////////////////////

#include <bits/getopt_core.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

/////////////////////////////////////
//
//			STATIC
//
////////////////////////////////////

static struct option opt[] = {
	{ "count", optional_argument, 0, 'c' },
	{ "help", no_argument, 0, 'h'},
	{ 0, 0, 0, 0 }
};

static int parse_options(int opt, options_t *options)
{
	switch (opt) {
		
		case 'c':
			options->count = strtol(optarg, NULL, 10);
			break;

		case 'h':
			show_usage();
			return (1);

		case '?':
			fprintf(stderr, "option inconnue: -%c\n", optopt);
			show_usage();
			return (1);
		
		default:
			fprintf(stderr, "Error: invalid argument !\n");
			show_usage();
			return (1);
	}

	return (0);
}


/////////////////////////////////////
//
//			PARSING
//
////////////////////////////////////
	
int parse_arguments(int ac, char **av, options_t *options)
{
	if (ac < 2) {
		show_usage();
		return (1);
	}

	int o = 0;

	while (1) {

		o = getopt_long(ac, av, "c:h", opt, NULL);

		if (o < 0)
			break ;

		if (parse_options(o, options))
			return (1);
	}

	if (optind >= ac) {
		fprintf(stderr, "Error: missing required argument !\n");
		show_usage();
		return (1);
	}

	return (0);
}
