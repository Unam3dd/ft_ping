/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_numbers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 15:03:52 by stales            #+#    #+#             */
/*   Updated: 2026/03/19 16:08:17 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/argparse.h"

/////////////////////////////////////
//
//			INCLUDES
//
////////////////////////////////////

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

/////////////////////////////////////
//
//			STATIC
//
////////////////////////////////////

static arg_status_t parse_numbers(arg_opt_t *arg, const char *n)
{
	if (!arg || !n)
		return (E_ARG_NULL);

	int64_t value = 0;
	char *end = NULL;

	if (*n == 0)
		return (E_ARG_PARSE_NOT_NBR);
	
	errno = 0;
	value = strtoll(n, &end, 10);

	if (errno == ERANGE)
		return (E_ARG_OVERFLOW);

	if (*end || end == n)
		return (E_ARG_PARSE_NOT_NBR);

	arg->i64 = value;

	ARGPARSE_MARK_ARG(arg);

	return (E_ARG_OK);
}
