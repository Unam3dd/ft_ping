/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 10:52:00 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/08/03 10:52:00 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_ping.h"
#include "../inc/test.h"
#include <getopt.h>

t_test_stats	g_tests = {0, 0, 0};

void	reset_options(void)
{
	opt_t	*o = NULL;
	size_t	n = 0;
	size_t	i = 0;

	o = get_options(&n);
	i = 0;
	
	while (i < n) {
		o[i].u64 = 0;
		o[i].str = NULL;
		o[i].bool = FALSE;
		i++;
	}

	optind = 1;
	optarg = NULL;
	optopt = 0;
}

int	test_print_summary(void)
{
	printf("\n" C_BOLD "======== summary ========" C_RESET "\n");
	printf(C_GREEN "passed: %d" C_RESET "\n", g_tests.passed);
	
	if (g_tests.failed)
		printf(C_RED C_BOLD "failed: %d" C_RESET "\n", g_tests.failed);
	else
		printf(C_DIM "failed: %d" C_RESET "\n", g_tests.failed);
	
	printf("total:  %d\n", g_tests.total);
	
	if (g_tests.failed) {
		printf(C_RED C_BOLD "\nRESULT: FAIL\n" C_RESET);
		return (1);
	}

	printf(C_GREEN C_BOLD "\nRESULT: PASS\n" C_RESET);
	return (0);
}
