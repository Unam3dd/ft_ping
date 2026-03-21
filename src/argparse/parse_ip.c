/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ip.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/21 09:31:29 by stales            #+#    #+#             */
/*   Updated: 2026/03/21 09:58:04 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/argparse.h"

/////////////////////////////////////
//
//			INCLUDES
//
////////////////////////////////////

#include <string.h>
#include <stdlib.h>

/////////////////////////////////////
//
//			STATIC
//
////////////////////////////////////

static arg_status_t check_ipv4(char *value)
{
	size_t l = 0;

	for (uint32_t i = 0; i < 4; i++) {

		l = strspn(value, "0123456789");

		if (!l || l > 3 || (l > 1 && *value == '0') )
			return (E_ARG_BAD_FMT);

		if ((strtoul(value, NULL, 10) > 0xFF) || (i != 3 && value[l] != '.'))
			return (E_ARG_BAD_FMT);

		value += (i != 3) ? l + 1 : l;
	}

	return (*value == 0 ? E_ARG_OK : E_ARG_BAD_FMT);
}

/////////////////////////////////////
//
//			PARSE IP
//
////////////////////////////////////

arg_status_t parse_ip(arg_opt_t *opt, char *value)
{
	if (!opt || !value)
		return (E_ARG_NULL);
	
	value += strspn(value, " \t\r\n");

	if (check_ipv4(value) < E_ARG_OK)
		return (E_ARG_BAD_FMT);

	opt->str = value;

	ARGPARSE_MARK_ARG(opt);

	return (E_ARG_OK);
}
