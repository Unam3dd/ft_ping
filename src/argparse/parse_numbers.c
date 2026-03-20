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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static arg_status_t parse_numbers(arg_opt_t *arg, const char *n)
{
	if (!arg || !n)
		return (E_ARG_NULL);

	switch (arg->type)
	{
		case ARG_SHORT:
			arg->i16 = (int16_t)strtol(n, NULL, 10);
			break;

		case ARG_INT:
			arg->i32 = (int32_t)strtol(n, NULL, 10);
			break;

		case ARG_LONG:
			arg->i64 = (int64_t)strtol(n, NULL, 10);
			break;

		case ARG_USHORT:
			arg->u16 = (uint16_t)strtoul(n, NULL, 10);
			break;

		case ARG_UINT:
			arg->u32 = (uint32_t)strtoul(n, NULL, 10);
			break;

		case ARG_ULONG:
			arg->u64 = (uint64_t)strtoul(n, NULL, 10);
			break;

		default:
			return (E_ARG_PARSE_NOT_NBR);
	}

	ARGPARSE_MARK_ARG(arg);

	return (E_ARG_OK);
}
