/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_rtt.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 10:52:00 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/08/03 10:52:00 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_ping.h"
#include "../inc/test.h"
#include <unistd.h>
#include <string.h>

void	test_rtt(void)
{
	rtt_t	r;
	double	avg = 0;

	memset(&r, 0, sizeof(rtt_t));

	TEST_SECTION("rtt");

	rtt_init(NULL);
	TEST_ASSERT(1, "rtt_init(NULL) does not crash");

	rtt_init(&r);
	TEST_ASSERT(r.count == 0 && r.sum == 0.0 && r.min == 0.0 && r.max == 0.0,
		"rtt_init zeroes fields");

	rtt_add(NULL, 1.0);
	TEST_ASSERT(1, "rtt_add(NULL) does not crash");

	rtt_add(&r, -1.0);
	TEST_ASSERT(r.count == 0, "rtt_add ignores negative values");

	rtt_add(&r, 10.0);
	rtt_add(&r, 12.0);
	rtt_add(&r, 14.0);
	rtt_add(&r, 50.0);
	TEST_ASSERT(r.count == 4, "rtt_add accumulates count");
	TEST_ASSERT_EQ_D(r.min, 10.0, 0.0001, "rtt min is 10");
	TEST_ASSERT_EQ_D(r.max, 50.0, 0.0001, "rtt max is 50");
	TEST_ASSERT_EQ_D(r.sum, 86.0, 0.0001, "rtt sum is 86");
	avg = r.sum / (double)r.count;
	TEST_ASSERT_EQ_D(avg, 21.5, 0.0001, "rtt avg is 21.5");

	rtt_init(&r);
	rtt_start(&r);
	usleep(20000);
	rtt_stop(&r);
	TEST_ASSERT(r.elapsed_ms >= 15 && r.elapsed_ms < 200,
		"rtt_stop measures elapsed time (~20ms)");
}

void	test_get_ms(void)
{
	struct timeval	t;
	double			ms;

	TEST_SECTION("get_ms");

	TEST_ASSERT_EQ_D(get_ms(NULL), 0.0, 0.0001, "get_ms(NULL) returns 0");

	gettimeofday(&t, NULL);
	usleep(10000);
	ms = get_ms(&t);
	TEST_ASSERT(ms >= 8.0 && ms < 100.0, "get_ms returns ~10ms delta");
}
