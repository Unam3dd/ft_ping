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

    struct iovec ios[] = {
        {.iov_base = "\033[1;37m\t-", .iov_len = sizeof("\033[1;37m\t-")},
        {.iov_base = (char *)opt->opt, .iov_len = strlen(opt->opt)},
        {.iov_base = "\t--", .iov_len = 3},
        {.iov_base = (char *)opt->alias, .iov_len = strlen(opt->alias)},
        {.iov_base = "\t\033[00m\033[1;37m",
         .iov_len = sizeof("\t\033[00m\033[1;37m")},
        {.iov_base = (char *)opt->help, .iov_len = strlen(opt->help)},
        {.iov_base = "\033[00m   \n", .iov_len = sizeof("\033[00m   \n")}};

    writev(STDOUT_FILENO, ios, sizeof(ios) / sizeof(ios[0]));
}

static void show_required(arg_opt_t *opt)
{
	if (!opt)
		return ;

	struct iovec ios[] = {
		{ " \033[1;37m[ ", sizeof(" \033[1;37m[ ") },
		{ (char *)opt->opt, strlen(opt->opt) },
		{ " ] ", 3}
	};

	writev(STDOUT_FILENO, ios, sizeof(ios)/sizeof(ios[0]));
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

    write(STDOUT_FILENO, "usage: \033[1;37m", sizeof("usage: \033[1;37m"));
    write(STDOUT_FILENO, args->name, strlen(args->name));
    write(STDOUT_FILENO, "\033[00m", sizeof("\033[00m"));

	for (i = 0; i < args->nopt; i++) {

		if (!(args->opt[i].flags & ARG_REQUIRED))
			continue ;
	
		show_required(&args->opt[i]);
	}

    if (args->usage)
    {
        write(STDOUT_FILENO, "\n\n\n\t", 4);
        write(STDOUT_FILENO, args->usage, strlen(args->usage));
        write(STDOUT_FILENO, "\n", 1);
    }

    write(STDOUT_FILENO, "\n\n\033[1;37mOptions:\033[00m\n",
          sizeof("\n\n\033[1;37mOptions:\033[00m\n"));

    for (i = 0; i < args->nopt; i++)
        show_options(&args->opt[i]);

    write(STDOUT_FILENO, "\n\nCrédits: \033[1;37m",
          sizeof("\n\nCrédits: \033[1;37m"));
    write(STDOUT_FILENO, args->credit, strlen(args->credit));
    write(STDOUT_FILENO, "\033[1;37m\n", sizeof("\033[1;37m"));
}
