/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_resolve.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 10:52:00 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/08/03 10:52:00 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_ping.h"
#include "../inc/test.h"
#include <arpa/inet.h>
#include <netinet/in.h>

void	test_resolve_host(void)
{
	sin_t	sin;
	char	ip[INET_ADDRSTRLEN];


	memset(ip, 0, sizeof(ip[INET_ADDRSTRLEN]));

	TEST_SECTION("resolve_host");

	TEST_ASSERT(resolve_host(NULL, &sin) == 1, "NULL host fails");

	memset(&sin, 0, sizeof(sin));
	TEST_ASSERT(resolve_host("127.0.0.1", &sin) == 0, "resolves 127.0.0.1");
	TEST_ASSERT(sin.sin_family == AF_INET, "family is AF_INET");
	inet_ntop(AF_INET, &sin.sin_addr, ip, sizeof(ip));
	TEST_ASSERT(strcmp(ip, "127.0.0.1") == 0, "address is 127.0.0.1");

	memset(&sin, 0, sizeof(sin));
	TEST_ASSERT(resolve_host("localhost", &sin) == 0, "resolves localhost");
	TEST_ASSERT(sin.sin_family == AF_INET, "localhost family AF_INET");

	TEST_ASSERT(resolve_host("this.host.does.not.exist.invalid", &sin) == 1,
		"unknown host fails");
}
