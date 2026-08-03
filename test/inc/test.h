/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 10:52:00 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/08/03 10:58:00 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
# define TEST_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdint.h>
# include <math.h>
# include <unistd.h>

typedef struct s_test_stats
{
	int	passed;
	int	failed;
	int	total;
}	t_test_stats;

extern t_test_stats	g_tests;

# define C_RESET   "\033[0m"
# define C_BOLD    "\033[1m"
# define C_DIM     "\033[2m"
# define C_RED     "\033[31m"
# define C_GREEN   "\033[32m"
# define C_YELLOW  "\033[33m"
# define C_BLUE    "\033[34m"
# define C_CYAN    "\033[36m"

# define TEST_ASSERT(cond, msg) do { \
	g_tests.total++; \
	if (cond) { \
		g_tests.passed++; \
		printf("  " C_GREEN C_BOLD "[OK]" C_RESET "   %s\n", msg); \
	} else { \
		g_tests.failed++; \
		printf("  " C_RED C_BOLD "[FAIL]" C_RESET " %s\n", msg); \
	} \
} while (0)

# define TEST_ASSERT_EQ_U64(a, b, msg) \
	TEST_ASSERT((uint64_t)(a) == (uint64_t)(b), msg)

# define TEST_ASSERT_EQ_I(a, b, msg) \
	TEST_ASSERT((a) == (b), msg)

# define TEST_ASSERT_EQ_D(a, b, eps, msg) \
	TEST_ASSERT(fabs((double)(a) - (double)(b)) <= (eps), msg)

# define TEST_SECTION(name) \
	printf("\n" C_CYAN C_BOLD "== %s ==" C_RESET "\n", name)

# define TEST_SKIP(msg) \
	printf("  " C_YELLOW C_BOLD "[SKIP]" C_RESET " %s\n", msg)

void	test_checksum(void);
void	test_parse_unumber(void);
void	test_rtt(void);
void	test_big16(void);
void	test_options(void);
void	test_resolve_host(void);
void	test_parse_arguments(void);
void	test_get_ms(void);

void	reset_options(void);
int		test_print_summary(void);

#endif
