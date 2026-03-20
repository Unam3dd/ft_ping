/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 15:58:28 by stales            #+#    #+#             */
/*   Updated: 2026/03/19 16:07:12 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/argparse.h"
#include <stdint.h>

/////////////////////////////////////
//
//			INCLUDES
//
////////////////////////////////////

static const char *parse_get_string_status(const arg_status_t status)
{
	static const struct {
		const char *msg;
		const arg_status_t status;
	} msg_tab[] = {
		{ "Success", E_ARG_OK },
		{ "Argument is not optional", E_ARG_NOT_OPTIONAL },
		{ "Invalid argument !", E_ARG_UNK },
		{ "Invalid format !", E_ARG_BAD_FMT },
		{ "Argument value is missing !", E_ARG_NULL },
		{ "Argument has not expected flags", E_ARG_NOT_FLAGS },
		{ "Argument is already set !", E_ARG_ALREADY_SET },
		{ "Argument is not the format expected !", E_ARG_PARSE_NOT_NBR | E_ARG_PARSE_NOT_BOOL }
	};

	for (uint16_t i = 0; i < sizeof(msg_tab)/sizeof(msg_tab[0]); i++)
		if (status == msg_tab[i].status)
			return (msg_tab[i].msg);

	return (NULL);
}
