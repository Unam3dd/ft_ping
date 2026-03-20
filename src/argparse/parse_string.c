/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 15:37:49 by stales            #+#    #+#             */
/*   Updated: 2026/03/19 15:40:51 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/argparse.h"

/////////////////////////////////////
//
//			INCLUDES
//
////////////////////////////////////

#include <string.h>

static arg_status_t parse_string(arg_opt_t *arg, char *value)
{
	if (!arg || !value)
		return (E_ARG_NULL);

	size_t len = strlen(value);

	if (!len || strspn(value, " \t\r\n") == len)
		return (E_ARG_STRING_EMPTY);

	arg->str = value;

	ARGPARSE_MARK_ARG(arg);

	return (E_ARG_OK);
}
