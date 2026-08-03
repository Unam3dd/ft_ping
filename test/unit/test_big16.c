/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_big16.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 10:52:00 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/08/03 10:52:00 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_ping.h"
#include "../inc/test.h"

void	test_big16(void)
{
	TEST_SECTION("BIG16");

	TEST_ASSERT(BIG16(0x0000) == 0x0000, "BIG16(0) == 0");
	TEST_ASSERT(BIG16(0x0001) == 0x0100, "BIG16 swaps 0x0001 -> 0x0100");
	TEST_ASSERT(BIG16(0x0100) == 0x0001, "BIG16 swaps 0x0100 -> 0x0001");
	TEST_ASSERT(BIG16(0x1234) == 0x3412, "BIG16 swaps 0x1234 -> 0x3412");
	TEST_ASSERT(BIG16(BIG16(0xABCD)) == 0xABCD, "BIG16 is involutive");
}
