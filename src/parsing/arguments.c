/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 17:21:44 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/09/15 10:40:00 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/////////////////////////////////////
//
//			INCLUDES
//
////////////////////////////////////

#include "../../inc/ft_ping.h"
#include <getopt.h>
#include <stdio.h>
#include <errno.h>

/////////////////////////////////////
//
//			DEFINES
//
////////////////////////////////////

/* --ttl has no short form in inetutils, so it gets a value outside ASCII */
#define OPT_TTL_LONG 0x100

#define TRY_MSG "Try 'ft_ping --help' or 'ft_ping --usage'" \
	" for more information.\n"

/////////////////////////////////////
//
//			STATIC
//
////////////////////////////////////

static int	g_host_index = 1;

static const struct option	g_long_opts[] = {
	{ "count", required_argument, NULL, 'c' },
	{ "verbose", no_argument, NULL, 'v' },
	{ "version", no_argument, NULL, 'V' },
	{ "help", no_argument, NULL, 'h' },
	{ "usage", no_argument, NULL, 'u' },
	{ "numeric", no_argument, NULL, 'n' },
	{ "timeout", required_argument, NULL, 'w' },
	{ "linger", required_argument, NULL, 'W' },
	{ "ttl", required_argument, NULL, OPT_TTL_LONG },
	{ 0, 0, 0, 0 }
};

/* Every option letter/value we actually know about.  Used to tell a missing
 * argument ("-c" with nothing after it) from a plain unknown option. */
static int	is_known_opt(int c)
{
	return (c == 'c' || c == 'v' || c == 'V' || c == 'n' || c == 'w'
		|| c == 'W' || c == OPT_TTL_LONG);
}

/* Mirrors the messages argp produces for inetutils' ping. */
static int	usage_error(char **av)
{
	const char	*arg = (optind >= 1) ? av[optind - 1] : NULL;

	if (optopt == '?') {
		show_usage();
		return (PARSE_DONE);
	}

	if (optopt == 0)
		fprintf(stderr, "ft_ping: unrecognized option '%s'\n",
			arg ? arg : "");
	else if (is_known_opt(optopt) && arg && arg[0] == '-' && arg[1] == '-')
		fprintf(stderr, "ft_ping: option '%s' requires an argument\n", arg);
	else if (is_known_opt(optopt))
		fprintf(stderr, "ft_ping: option requires an argument -- '%c'\n",
			optopt);
	else
		fprintf(stderr, "ft_ping: invalid option -- '%c'\n", optopt);

	fprintf(stderr, TRY_MSG);

	return (PARSE_USAGE);
}

static int	parse_count(opt_t *option, const char *arg)
{
	errno = 0;
	option[OPT_COUNT_INDEX].u64 = parse_unumber(arg);

	if (errno) {
		fprintf(stderr, "ft_ping: invalid value (`%s' near `%s')\n",
			arg, arg);
		return (PARSE_ERROR);
	}

	return (PARSE_OK);
}

static int	parse_ttl(opt_t *option, const char *arg)
{
	errno = 0;
	option[OPT_TTL_INDEX].u32 = (uint32_t)parse_unumber(arg);

	if (errno) {
		fprintf(stderr, "ft_ping: invalid value (`%s' near `%s')\n",
			arg, arg);
		return (PARSE_ERROR);
	}

	if (option[OPT_TTL_INDEX].u32 < 1) {
		fprintf(stderr, "ft_ping: option value too small: %u\n",
			option[OPT_TTL_INDEX].u32);
		return (PARSE_ERROR);
	}

	if (option[OPT_TTL_INDEX].u32 > 255) {
		fprintf(stderr, "ft_ping: option value too big: %u\n",
			option[OPT_TTL_INDEX].u32);
		return (PARSE_ERROR);
	}

	return (PARSE_OK);
}

static int	parse_seconds(opt_t *option, const char *arg)
{
	errno = 0;
	option->u64 = parse_unumber(arg);

	if (errno) {
		fprintf(stderr, "ft_ping: invalid value (`%s' near `%s')\n",
			arg, arg);
		return (PARSE_ERROR);
	}

	if (option->u64 < 1) {
		fprintf(stderr, "ft_ping: option value too small: %lu\n",
			(unsigned long)option->u64);
		return (PARSE_ERROR);
	}

	if (option->u64 > 2147483647ULL) {
		fprintf(stderr, "ft_ping: option value too big: %lu\n",
			(unsigned long)option->u64);
		return (PARSE_ERROR);
	}

	return (PARSE_OK);
}

/////////////////////////////////////
//
//			PARSING
//
////////////////////////////////////

int get_first_host_index(void)
{
	return (g_host_index);
}

int parse_arguments(int ac, char **av, opt_t *option)
{
	int	o = 0;
	int	ret = PARSE_OK;

	if (!option || !av)
		return (PARSE_ERROR);

	g_host_index = ac;
	opterr = 0;
	optopt = 0;

	while ((o = getopt_long(ac, av, "c:vVnw:W:", g_long_opts, NULL)) != -1) {

		switch (o) {

			case 'c':
				ret = parse_count(option, optarg);
				break;

			case OPT_TTL_LONG:
				ret = parse_ttl(option, optarg);
				break;

			case 'w':
				ret = parse_seconds(&option[OPT_TIMEOUT_INDEX], optarg);
				break;

			case 'W':
				ret = parse_seconds(&option[OPT_LINGER_INDEX], optarg);
				break;

			case 'v':
				option[OPT_VERBOSE_INDEX].b = TRUE;
				break;

			case 'n':
				option[OPT_NUMERIC_INDEX].b = TRUE;
				break;

			case 'V':
				show_version();
				return (PARSE_DONE);

			case 'h':
				show_usage();
				return (PARSE_DONE);

			case 'u':
				show_usage_short();
				return (PARSE_DONE);

			case '?':
			default:
				return (usage_error(av));
		}

		if (ret != PARSE_OK)
			return (ret);

		optopt = 0;
	}

	if (optind >= ac) {
		fprintf(stderr, "ft_ping: missing host operand\n");
		fprintf(stderr, TRY_MSG);
		return (PARSE_USAGE);
	}

	g_host_index = optind;
	option[OPT_HOST_INDEX].str = av[optind];

	return (PARSE_OK);
}
