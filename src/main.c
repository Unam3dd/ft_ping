/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 12:10:09 by stales            #+#    #+#             */
/*   Updated: 2026/03/21 09:24:27 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/argparse.h"
#include "../inc/ft_ping.h"
#include <stdio.h>

int main(int ac, char **av)
{
	args_t args = {NULL, NULL, NULL, NULL, 0};

	ARGPARSE(opt)
		ARGPARSE_REQUIRED("ip", "Target IP Address", ARG_IPV4),
		ARGPARSE_BOOL("b", "bool", "simple bool", FALSE, ARG_OPTIONAL | ARG_PARAM),
		ARGPARSE_OPT("v", "version", "Show version", "1.7", ARG_OPTIONAL, ARG_STRING),
  		ARGPARSE_HELP(),
  	ARGPARSE_END()

  	ARGPARSE_NAME(args, av[0]);
  	ARGPARSE_USAGE(args, "This is a simple ping program !");
  	ARGPARSE_CREDIT(args, "sam0verfl0w");

	argparse_init(&args, args_opt, ARGPARSE_CNT(ARGPARSE_GET(opt)));

	if (ac < 2 || argparse_parse(&args, ac, av)) {
		argparse_show_usage(&args);
		return (1);
	}

	return (0);
}
