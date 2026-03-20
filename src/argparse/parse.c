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

static arg_opt_t *argparse_get(args_t *args, const char *n)
{
	if (!args || !n)
		return (NULL);

	size_t i = 0, cnt = 0;

	cnt = strspn(n, "-");

	if (cnt != 1 && cnt != 2)
		return (cnt > 2 ? (arg_opt_t*)E_ARG_BAD_FMT : (arg_opt_t*)E_ARG_NOT_OPTIONAL);

	n += cnt;

	while (i < args->nopt) {

		if (cnt == 2 && args->opt[i].alias && !strcmp(args->opt[i].alias, n))
			return (&args->opt[i]);
		else if (cnt == 1 && args->opt[i].opt && !strcmp(args->opt[i].opt, n))
			return (&args->opt[i]);
		
		i++;
	}

	return ((arg_opt_t*)E_ARG_UNK);
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

static arg_status_t handle_argument(args_t *args, arg_opt_t *optional, int *ac, char **av, int flags)
{
	if (!ac || !av)
		return (E_ARG_NULL);

	arg_opt_t *current = optional ? optional : argparse_next(args, flags);

	if (!current)
		return (E_ARG_NULL);

	if (ARGPARSE_ARG_HAS(current, ARG_MARK))
		return (E_ARG_ALREADY_SET);

	if (ARGPARSE_ARG_HAS_NOT(current, ARG_PARAM)) {
		ARGPARSE_MARK_ARG(current);
		return (E_ARG_OK);
	}

	if (ARGPARSE_ARG_HAS_NOT_REQUIRED(current))
		*ac += 1;

	return (parse_argument(current, av[*ac]));
}

/////////////////////////////////////
//
//			PARSE
//
////////////////////////////////////

int argparse_parse(args_t *args, int ac, char **av)
{
	if (!args)
        return (1);

	arg_opt_t *optional = NULL;
	arg_status_t s = 0;
	const char *err = NULL;

	for (int i = 1; i < ac; i++) {

		optional = argparse_get(args, av[i]);

		if (optional == (arg_opt_t*)E_ARG_NOT_OPTIONAL) {
			printf("Argument Required: %s\n", av[i]);

			s = handle_argument(args, NULL, &ac, av, ARG_REQUIRED);

			if (s < E_ARG_OK)
				break;

			continue ;
		}

		if (optional == (arg_opt_t*)E_ARG_BAD_FMT || optional == (arg_opt_t*)E_ARG_UNK) {
			s = (arg_status_t)optional;
			continue ;
		}

		printf("Argument Optionnal Found: %s\n", av[i]);
		s = handle_argument(NULL, optional, &ac, av, ARG_OPTIONAL);
	}

	if (s < E_ARG_OK) {
		err = parse_get_string_status(s);
		printf("Error: %s\n", err);
	}

    return (s < E_ARG_OK ? 1 : 0);
}
