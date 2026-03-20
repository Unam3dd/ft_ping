/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 09:30:53 by stales            #+#    #+#             */
/*   Updated: 2026/03/19 16:19:37 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/argparse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/////////////////////////////////////
//
//			STATIC
//
////////////////////////////////////

#include "./parse_errors.c"
#include "./parse_bool.c"
#include "./parse_numbers.c"
#include "./parse_string.c"

static arg_opt_t *argparse_get(args_t *args, const char *n, arg_status_t *status)
{
	if (!args || !n)
		return (NULL);

	size_t i = 0, cnt = 0;

	cnt = strspn(n, "-");

	if (cnt != 1 && cnt != 2) {
		
		if (cnt > 2 && status)
			*status = E_ARG_BAD_FMT;
		else
			*status = E_ARG_IS_REQUIRED;

		return (NULL);
	}

	n += cnt;

	while (i < args->nopt) {

		if (cnt == 2 && args->opt[i].alias && !strcmp(args->opt[i].alias, n))
			return (&args->opt[i]);
		
		if (cnt == 1 && args->opt[i].opt && !strcmp(args->opt[i].opt, n))
			return (&args->opt[i]);
		
		i++;
	}

	if (status)
		*status = E_ARG_UNK;

	return (NULL);
}

static arg_opt_t *argparse_next(args_t *args, arg_flag_t flags)
{
	if (!args)
		return (NULL);

	for (size_t i = 0; i < args->nopt; i++)
		if (args->opt[i].flags & flags)
			return (&args->opt[i]);

	return (NULL);
}

static arg_status_t parse_argument(arg_opt_t *arg, char *value)
{
	arg_status_t s = 0;

	switch (arg->type)
	{
		case ARG_BOOL:
			s = parse_bool(arg, value);
			break;

		case ARG_INT:
		case ARG_UINT:
		case ARG_LONG:
		case ARG_ULONG:
			s = parse_numbers(arg, value);
			break;

		case ARG_STRING:
			s = parse_string(arg, value);
			break;

		default:
			return (E_ARG_BAD_FMT);
	}

	return (s);
}

static arg_status_t handle_argument(arg_ctx_t *ctx, int ac, char **av, int flags)
{
	if (!ac || !av || !ctx || !ctx->i)
		return (E_ARG_NULL);

	arg_opt_t *current = flags & ARG_REQUIRED ? argparse_next(ctx->args, flags) : ctx->current;

	if (!current)
		return (E_ARG_NULL);

	if (ARGPARSE_ARG_HAS(current, ARG_MARK))
		return (E_ARG_ALREADY_SET);

	if (ARGPARSE_ARG_HAS_NOT(current, ARG_PARAM)) {
		ARGPARSE_MARK_ARG(current);
		return (E_ARG_OK);
	}

	if (ARGPARSE_ARG_HAS_NOT_REQUIRED(current)) {

		if (*ctx->i + 1 >= ac)
			return (E_ARG_MISS_PARAMS);

		*ctx->i += 1;

		if (av[*ctx->i] == NULL || av[*ctx->i][0] == 0)
			return (E_ARG_MISS_PARAMS);
	}

	return (parse_argument(current, av[*ctx->i]));
}

static arg_status_t proceed_argument(arg_ctx_t *ctx, arg_status_t s, int ac, char **av)
{
	if (!ctx)
		return (E_ARG_NULL);

	switch (s) {
		case E_ARG_BAD_FMT:
			return (E_ARG_BAD_FMT);

		case E_ARG_UNK:
			return (E_ARG_UNK);

		case E_ARG_IS_REQUIRED:
			return (handle_argument(ctx, ac, av, ARG_REQUIRED));

		default:
			return (handle_argument(ctx, ac, av, ARG_OPTIONAL));
	}

	return (E_ARG_NULL);
}

/////////////////////////////////////
//
//			PARSE
//
////////////////////////////////////

int argparse_parse(args_t *args, int ac, char **av)
{
	if (!args || !av)
        return (1);

	int i = 1;
	arg_status_t s = 0;
	
	arg_ctx_t ctx = { 
		.args = args, 
		.current = NULL, 
		.i = &i 
	};

	for (i = 1; i < ac; i++) {
		ctx.current = argparse_get(args, av[i], &s);

		s = proceed_argument(&ctx, s, ac, av);
		
		if (s < E_ARG_OK)
			break ;
	}

	if (s < E_ARG_OK)
		printf("Error: %s\n", parse_get_string_status(s));

    return (s < E_ARG_OK);
}
