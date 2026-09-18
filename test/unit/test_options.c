/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_options.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 10:52:00 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/09/15 22:41:00 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_ping.h"
#include "../inc/test.h"

void	test_options(void)
{
	opt_t	*o = NULL;
	size_t	n = 0;

	TEST_SECTION("get_options");

	o = get_options(&n);
	TEST_ASSERT(o != NULL, "get_options returns non-NULL");
	TEST_ASSERT(n >= 7, "get_options exposes at least 7 options");
	TEST_ASSERT(strcmp(o[OPT_HOST_INDEX].key, "host") == 0, "host key");
	TEST_ASSERT(strcmp(o[OPT_COUNT_INDEX].key, "count") == 0, "count key");
	TEST_ASSERT(strcmp(o[OPT_VERBOSE_INDEX].key, "verbose") == 0, "verbose key");
	TEST_ASSERT(strcmp(o[OPT_TTL_INDEX].key, "ttl") == 0, "ttl key");
	TEST_ASSERT(strcmp(o[OPT_NUMERIC_INDEX].key, "numeric") == 0, "numeric key");
	TEST_ASSERT(strcmp(o[OPT_TIMEOUT_INDEX].key, "timeout") == 0, "timeout key");
	TEST_ASSERT(strcmp(o[OPT_LINGER_INDEX].key, "linger") == 0, "linger key");
}

void	test_parse_arguments(void)
{
	opt_t	*o = NULL;
	char	*av_help[] = {"ft_ping", "--help", NULL};
	char	*av_qmark[] = {"ft_ping", "-?", NULL};
	char	*av_usage[] = {"ft_ping", "--usage", NULL};
	char	*av_ver[] = {"ft_ping", "-V", NULL};
	char	*av_ok[] = {"ft_ping", "-v", "-n", "-c", "3", "--ttl", "64",
		"-w", "5", "-W", "2", "127.0.0.1", NULL};
	char	*av_linger[] = {"ft_ping", "--linger", "3", "127.0.0.1", NULL};
	char	*av_bad_linger[] = {"ft_ping", "-W", "0", "127.0.0.1", NULL};
	char	*av_bad_ttl[] = {"ft_ping", "--ttl", "999", "127.0.0.1", NULL};
	char	*av_bad_c[] = {"ft_ping", "-c", "abc", "127.0.0.1", NULL};
	char	*av_nohost[] = {"ft_ping", "-v", NULL};
	char	*av_badopt[] = {"ft_ping", "-Z", "127.0.0.1", NULL};
	char	*av_hosts[] = {"ft_ping", "-c", "1", "127.0.0.1", "127.0.0.2",
		NULL};
	int		ret;

	TEST_SECTION("parse_arguments");

	o = get_options(NULL);

	reset_options();
	ret = parse_arguments(2, av_help, o);
	TEST_ASSERT(ret == PARSE_DONE, "--help returns PARSE_DONE (exit 0)");

	reset_options();
	ret = parse_arguments(2, av_qmark, o);
	TEST_ASSERT(ret == PARSE_DONE, "-? returns PARSE_DONE (exit 0)");

	reset_options();
	ret = parse_arguments(2, av_usage, o);
	TEST_ASSERT(ret == PARSE_DONE, "--usage returns PARSE_DONE (exit 0)");

	reset_options();
	ret = parse_arguments(2, av_ver, o);
	TEST_ASSERT(ret == PARSE_DONE, "-V returns PARSE_DONE (exit 0)");

	reset_options();
	ret = parse_arguments(1, av_nohost, o);
	TEST_ASSERT(ret == PARSE_USAGE, "missing host returns PARSE_USAGE (64)");

	reset_options();
	ret = parse_arguments(3, av_badopt, o);
	TEST_ASSERT(ret == PARSE_USAGE, "invalid option returns PARSE_USAGE (64)");

	reset_options();
	ret = parse_arguments(4, av_bad_ttl, o);
	TEST_ASSERT(ret == PARSE_ERROR, "ttl > 255 rejected");

	reset_options();
	ret = parse_arguments(4, av_bad_c, o);
	TEST_ASSERT(ret == PARSE_ERROR, "invalid count rejected");

	reset_options();
	ret = parse_arguments(12, av_ok, o);
	TEST_ASSERT(ret == PARSE_OK, "valid args parse OK");
	TEST_ASSERT(o[OPT_VERBOSE_INDEX].b == TRUE, "verbose set");
	TEST_ASSERT(o[OPT_NUMERIC_INDEX].b == TRUE, "numeric set");
	TEST_ASSERT(o[OPT_COUNT_INDEX].u64 == 3, "count = 3");
	TEST_ASSERT(o[OPT_TTL_INDEX].u32 == 64, "ttl = 64");
	TEST_ASSERT(o[OPT_TIMEOUT_INDEX].u64 == 5, "timeout = 5");
	TEST_ASSERT(o[OPT_LINGER_INDEX].u64 == 2, "linger = 2");
	TEST_ASSERT(o[OPT_HOST_INDEX].str
		&& strcmp(o[OPT_HOST_INDEX].str, "127.0.0.1") == 0, "host set");

	reset_options();
	ret = parse_arguments(4, av_linger, o);
	TEST_ASSERT(ret == PARSE_OK, "--linger parses OK");
	TEST_ASSERT(o[OPT_LINGER_INDEX].u64 == 3, "linger = 3");

	reset_options();
	ret = parse_arguments(4, av_bad_linger, o);
	TEST_ASSERT(ret == PARSE_ERROR, "linger < 1 rejected");

	reset_options();
	ret = parse_arguments(5, av_hosts, o);
	TEST_ASSERT(ret == PARSE_OK, "multiple hosts parse OK");
	TEST_ASSERT(get_first_host_index() == 3, "first host index points at 127.0.0.1");
	TEST_ASSERT(strcmp(av_hosts[get_first_host_index()], "127.0.0.1") == 0,
		"host list starts at 127.0.0.1");
	TEST_ASSERT(strcmp(av_hosts[get_first_host_index() + 1], "127.0.0.2") == 0,
		"second host reachable from the list");
}
