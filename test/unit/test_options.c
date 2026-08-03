/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_options.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 10:52:00 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/08/03 10:52:00 by sam0verfl0w      ###   ########.fr       */
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
	TEST_ASSERT(n >= 6, "get_options exposes at least 6 options");
	TEST_ASSERT(strcmp(o[OPT_HOST_INDEX].key, "host") == 0, "host key");
	TEST_ASSERT(strcmp(o[OPT_COUNT_INDEX].key, "count") == 0, "count key");
	TEST_ASSERT(strcmp(o[OPT_VERBOSE_INDEX].key, "verbose") == 0, "verbose key");
	TEST_ASSERT(strcmp(o[OPT_TTL_INDEX].key, "ttl") == 0, "ttl key");
	TEST_ASSERT(strcmp(o[OPT_NUMERIC_INDEX].key, "numeric") == 0, "numeric key");
	TEST_ASSERT(strcmp(o[OPT_DEADLINE_INDEX].key, "deadline") == 0, "deadline key");
}

void	test_parse_arguments(void)
{
	opt_t	*o = NULL;
	char	*av_help[] = {"ft_ping", "-h", NULL};
	char	*av_qmark[] = {"ft_ping", "-?", NULL};
	char	*av_ver[] = {"ft_ping", "-V", NULL};
	char	*av_ok[] = {"ft_ping", "-v", "-n", "-c", "3", "-t", "64",
		"-w", "5", "127.0.0.1", NULL};
	char	*av_bad_ttl[] = {"ft_ping", "-t", "999", "127.0.0.1", NULL};
	char	*av_bad_c[] = {"ft_ping", "-c", "abc", "127.0.0.1", NULL};
	char	*av_nohost[] = {"ft_ping", "-v", NULL};
	int		ret;

	TEST_SECTION("parse_arguments");

	o = get_options(NULL);

	reset_options();
	ret = parse_arguments(2, av_help, o);
	TEST_ASSERT(ret == 1, "-h returns 1 (usage)");

	reset_options();
	ret = parse_arguments(2, av_qmark, o);
	TEST_ASSERT(ret == 1, "-? returns 1 (usage)");

	reset_options();
	ret = parse_arguments(2, av_ver, o);
	TEST_ASSERT(ret == 2, "-V returns 2 (version)");

	reset_options();
	ret = parse_arguments(1, av_nohost, o);
	TEST_ASSERT(ret == 1, "missing host returns 1");

	reset_options();
	ret = parse_arguments(4, av_bad_ttl, o);
	TEST_ASSERT(ret == 1, "ttl > 255 rejected");

	reset_options();
	ret = parse_arguments(4, av_bad_c, o);
	TEST_ASSERT(ret == 1, "invalid count rejected");

	reset_options();
	ret = parse_arguments(10, av_ok, o);
	TEST_ASSERT(ret == 0, "valid args parse OK");
	TEST_ASSERT(o[OPT_VERBOSE_INDEX].bool == TRUE, "verbose set");
	TEST_ASSERT(o[OPT_NUMERIC_INDEX].bool == TRUE, "numeric set");
	TEST_ASSERT(o[OPT_COUNT_INDEX].u64 == 3, "count = 3");
	TEST_ASSERT(o[OPT_TTL_INDEX].u32 == 64, "ttl = 64");
	TEST_ASSERT(o[OPT_DEADLINE_INDEX].u64 == 5, "deadline = 5");
	TEST_ASSERT(o[OPT_HOST_INDEX].str
		&& strcmp(o[OPT_HOST_INDEX].str, "127.0.0.1") == 0, "host set");
}
