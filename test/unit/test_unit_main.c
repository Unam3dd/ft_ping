/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_unit_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 10:52:00 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/08/03 10:58:00 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/test.h"

int	main(void)
{
	int	status = 0;

	printf(C_BOLD C_BLUE "======== ft_ping unit tests ========" C_RESET "\n");

	test_checksum();
	test_parse_unumber();
	test_big16();
	test_rtt();
	test_get_ms();
	test_options();
	test_parse_arguments();
	test_resolve_host();

	status = test_print_summary();

	return (status);
}
