/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usage.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 16:00:02 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/07/03 16:04:25 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_ping.h"

/////////////////////////////////////
//
//			INCLUDES
//
////////////////////////////////////

#include <stdio.h>

/////////////////////////////////////
//
//			USAGE
//
////////////////////////////////////

void show_usage(void)
{
	puts("usage: ft_ping [OPTION...] HOST ...");
	puts("Send ICMP ECHO_REQUEST packets to network hosts.");
	puts("Options:");
	puts("\t-v, --verbose\t\tverbose output\n");
	puts("\t-c, --count=NUMBER\tstop after sending NUMBER packets\n");
	puts("\t    --ttl=NUMBER\tset IP time to live\n");
	puts("\t-?, --help\t\tshow help\n");
	puts("\t    --usage\t\tshow help\n");
	puts("\t-V, --version\t\tprint program version\n");

	show_version();
}

