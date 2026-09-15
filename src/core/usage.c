/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usage.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 16:00:02 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/09/15 10:40:00 by sam0verfl0w      ###   ########.fr       */
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

/* Long help, printed for -? and --help (same output as inetutils). */
void show_usage(void)
{
	puts("Usage: ft_ping [OPTION...] HOST ...");
	puts("Send ICMP ECHO_REQUEST packets to network hosts.");
	puts("");
	puts(" Options valid for all request types:");
	puts("");
	puts("  -c, --count=NUMBER         stop after sending NUMBER packets");
	puts("  -n, --numeric              do not resolve host addresses");
	puts("      --ttl=N                specify N as time-to-live");
	puts("  -v, --verbose              verbose output");
	puts("  -w, --timeout=N            stop after N seconds");
	puts("  -W, --linger=N             number of seconds to wait for response");
	puts("");
	puts("  -?, --help                 give this help list");
	puts("      --usage                give a short usage message");
	puts("  -V, --version              print program version");
	puts("");
	puts("Mandatory or optional arguments to long options are also mandatory"
		" or optional");
	puts("for any corresponding short options.");
}

/* Short synopsis, printed for --usage only. */
void show_usage_short(void)
{
	puts("Usage: ft_ping [-nv?V] [-c NUMBER] [-w N] [-W N] [--count=NUMBER]");
	puts("            [--numeric] [--ttl=N] [--verbose] [--timeout=N]");
	puts("            [--linger=N] [--help] [--usage] [--version] HOST ...");
}
