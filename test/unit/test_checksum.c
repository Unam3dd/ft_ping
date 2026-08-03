/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_checksum.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 10:52:00 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/08/03 10:52:00 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_ping.h"
#include "../inc/test.h"
#include <string.h>

void	test_checksum(void)
{
	uint8_t		odd[3] = { 0, 0, 0 };
	uint16_t	cs = 0;
	uint16_t	zeros[4] = { 0, 0, 0, 0 };
	icmp_pkt_t	pkt;

	TEST_SECTION("checksum");

	TEST_ASSERT(checksum(NULL, 8) == 0, "NULL buffer returns 0");
	TEST_ASSERT(checksum(zeros, -1) == 0, "negative len returns 0");

	memset(zeros, 0, sizeof(zeros));
	cs = checksum(zeros, (int)sizeof(zeros));
	TEST_ASSERT(cs == 0xFFFF, "all-zero buffer checksum is 0xFFFF");

	memset(&pkt, 0, sizeof(pkt));
	pkt.h.type = ICMP_ECHO;
	pkt.h.code = 0;
	pkt.h.un.echo.id = 0x1234;
	pkt.h.un.echo.sequence = 1;
	pkt.h.checksum = 0;
	cs = checksum(&pkt, (int)sizeof(pkt));
	pkt.h.checksum = cs;
	TEST_ASSERT(checksum(&pkt, (int)sizeof(pkt)) == 0,
		"valid ICMP packet checksum verifies to 0");

	odd[0] = 0x01;
	odd[1] = 0x02;
	odd[2] = 0x03;
	cs = checksum(odd, 3);
	TEST_ASSERT(cs != 0, "odd-length buffer produces a checksum");
}
