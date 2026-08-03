/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_number.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 10:52:00 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/08/03 10:52:00 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_ping.h"
#include "../inc/test.h"
#include <errno.h>

void	test_parse_unumber(void)
{
	uint64_t	v = 0;

	TEST_SECTION("parse_unumber");

	errno = 0;
	v = parse_unumber("42");
	TEST_ASSERT(errno == 0 && v == 42, "parses 42");

	errno = 0;
	v = parse_unumber("0");
	TEST_ASSERT(errno == 0 && v == 0, "parses 0");

	errno = 0;
	v = parse_unumber("255");
	TEST_ASSERT(errno == 0 && v == 255, "parses 255");

	errno = 0;
	v = parse_unumber("abc");
	TEST_ASSERT(errno == EINVAL && v == 0, "rejects abc");

	errno = 0;
	v = parse_unumber("-1");
	TEST_ASSERT(errno == EINVAL && v == 0, "rejects -1");

	errno = 0;
	v = parse_unumber("+1");
	TEST_ASSERT(errno == EINVAL && v == 0, "rejects +1");

	errno = 0;
	v = parse_unumber("");
	TEST_ASSERT(errno == EINVAL && v == 0, "rejects empty string");

	errno = 0;
	v = parse_unumber("12a");
	TEST_ASSERT(errno == EINVAL && v == 0, "rejects trailing junk");

	errno = 0;
	v = parse_unumber(NULL);
	TEST_ASSERT(errno == EINVAL && v == 0, "rejects NULL");
}
