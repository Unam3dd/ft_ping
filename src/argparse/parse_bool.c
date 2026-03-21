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
	
	static struct {
		const char *keyword;
		bool_t value;
	} tab[] = {
		{ "true", TRUE }, { "TRUE", TRUE },
		{ "True", TRUE }, { "t",    TRUE },
		{ "1",    TRUE }, { "false", FALSE },
		{ "FALSE", FALSE }, { "False", FALSE },
		{ "f", FALSE }, { "0", FALSE }
	};

	for (uint16_t i = 0; i < sizeof(tab)/sizeof(tab[0]); i++) {
		if (!strcmp(tab[i].keyword, value)) {
			arg->bool = tab[i].value;
			ARGPARSE_MARK_ARG(arg);
			return (E_ARG_OK);
		}
	}

	return (E_ARG_PARSE_NOT_BOOL);
}
