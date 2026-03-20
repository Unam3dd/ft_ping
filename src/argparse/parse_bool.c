/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bool.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 15:42:27 by stales            #+#    #+#             */
/*   Updated: 2026/03/19 16:08:12 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/argparse.h"
#include <stdint.h>
#include <string.h>

/////////////////////////////////////
//
//			INCLUDES
//
////////////////////////////////////

static arg_status_t parse_bool(arg_opt_t *arg, char *value)
{
	if (!arg || !value)
		return (E_ARG_NULL);

	size_t cnt = strspn(value, " \t\r\n");
	uint16_t i = 0;

	static char *keyword[] = { 
		"true", "TRUE", "True", "t", "1",
		"false", "False", "False", "f", "0"
	};

	if (cnt == strlen(value))
		return (E_ARG_PARSE_NOT_BOOL);

	value += cnt;

	for (i = 0; i < (sizeof(keyword)/sizeof(char *)) / 2; i++) {

		if (strcmp(keyword[i], value))
			continue ;

		arg->bool = TRUE;
		
		ARGPARSE_MARK_ARG(arg);

		return (E_ARG_OK);
	}

	for (i = 5; i < sizeof(keyword)/sizeof(char *); i++) {

		if (strcmp(keyword[i], value))
			continue ;

		arg->bool = FALSE;
		
		ARGPARSE_MARK_ARG(arg);

		return (E_ARG_OK);
	}

	return (E_ARG_PARSE_NOT_BOOL);
}
