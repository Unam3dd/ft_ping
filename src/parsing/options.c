/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 16:56:56 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/07/03 16:04:49 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_ping.h"

/////////////////////////////////////
//
//			INCLUDES
//
////////////////////////////////////

#include <errno.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/////////////////////////////////////
//
//			OPTIONS
//
////////////////////////////////////

opt_t *get_options(size_t *size)
{
	static opt_t options[] = { 
		[OPT_HOST_INDEX] = ARGOPT("host", TRUE, STRING, sizeof(char *)),
		[OPT_COUNT_INDEX] = ARGOPT("count", FALSE, UINT_64, sizeof(uint64_t)),
		[OPT_VERBOSE_INDEX] = ARGOPT("verbose", FALSE, BOOL, sizeof(bool_t)),
		[OPT_TTL_INDEX] = ARGOPT("ttl", FALSE, UINT_32, sizeof(uint32_t)),
		[OPT_NUMERIC_INDEX] = ARGOPT("numeric", FALSE, BOOL, sizeof(bool_t)),
		[OPT_TIMEOUT_INDEX] = ARGOPT("timeout", FALSE, UINT_64, sizeof(uint64_t))
	};

	if (size)
		*size = sizeof(options)/sizeof(opt_t);

	return (options);
}

