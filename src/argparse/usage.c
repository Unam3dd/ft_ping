/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usage.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 09:34:11 by stales            #+#    #+#             */
/*   Updated: 2026/03/18 09:40:42 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/argparse.h"
#include <bits/types/struct_iovec.h>
#include <stdio.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>

/////////////////////////////////////
//
//			STATIC
//
////////////////////////////////////

static void show_options(arg_opt_t *opt)
{
	if (!opt || opt->flags & ARG_REQUIRED)
		return;

	printf("\033[1;37m\t-%s", opt->opt);

	if (opt->alias)
		printf("\t--%s", opt->alias);

	if (opt->help)
		printf("\t%s\033[00m   \n", opt->help);
}

static void show_required(arg_opt_t *opt)
{
	if (!opt || !(opt->flags & ARG_REQUIRED))
		return ;

	printf(" \033[1;37m[ %s ]\033[00m ", opt->opt);
}

/////////////////////////////////////
//
//			USAGE
//
////////////////////////////////////

void argparse_show_usage(const args_t *args)
{
	if (!args)
		return;

	size_t i = 0;
	
	printf("usage: \033[1;37m%s", args->name);

	for (i = 0; i < args->nopt; i++)
		show_required(&args->opt[i]);

    if (args->usage)
		printf("\n\n\n\t%s\n", args->usage);

	printf("\n\n\033[1;37moptions:\033[00m\n");

	for (i = 0; i < args->nopt; i++)
		show_options(&args->opt[i]);

	printf("\n\nCredits: \033[1;37m%s\033[00m\n", args->credit);
}
