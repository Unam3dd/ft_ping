/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 22:47:04 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/06/23 22:52:39 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"

/////////////////////////////////////
//
//			INCLUDES
//
////////////////////////////////////

#include <stdio.h>

/////////////////////////////////////
//
//			HELP
//
////////////////////////////////////

void show_usage(void)
{
	printf("Usage: ft_ping [options] <destination>\nOptions:\n");
	printf("\t<destination>\tDNS name or IP address\n");
	printf("\t-c <count>\t\tstop after <count> replies\n");
}
