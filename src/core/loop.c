/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 11:51:01 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/09/15 10:40:00 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_ping.h"

/////////////////////////////////////
//
//			INCLUDES
//
////////////////////////////////////

#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/////////////////////////////////////
//
//			VOLATILE
//
////////////////////////////////////

/* Not reset between hosts: a Ctrl-C stops the whole run, exactly like
 * inetutils does when several hosts are given. */
static volatile sig_atomic_t	g_run = 1;

/////////////////////////////////////
//
//			SIGNAL
//
////////////////////////////////////

static void sig_handler(int sig)
{
	if (sig == SIGINT || sig == SIGALRM)
		g_run = 0;
}

/////////////////////////////////////
//
//			STATIC
//
////////////////////////////////////

static int	count_done(uint64_t count, uint64_t n)
{
	return (count && n >= count);
}

static uint64_t	now_ms(void)
{
	struct timespec	ts;

	if (clock_gettime(CLOCK_MONOTONIC, &ts) < 0)
		return (0);

	return ((uint64_t)ts.tv_sec * 1000 + (uint64_t)(ts.tv_nsec / 1000000));
}

static void	send_one(context_t *ctx)
{
	if (send_icmp_echo(ctx, ctx->fd, &ctx->sin) < 0)
		fprintf(stderr, "ft_ping: sendto: %s\n", strerror(errno));

	ctx->s.transmitted++;
}

static void	on_timer(context_t *ctx, opt_t *opt)
{
	uint64_t	exp = 0;

	if (read(ctx->tfd, &exp, sizeof(exp)) < 0)
		return ;
	if (count_done(opt[OPT_COUNT_INDEX].u64, ctx->s.transmitted))
		return ;
	send_one(ctx);
}

static int	on_icmp(context_t *ctx, opt_t *opt, uint64_t *answered)
{
	int	r = recv_icmp_echo(ctx);

	if (!r) {
		ctx->s.received++;
		(*answered)++;
	} else if (r == ICMP_OURS_ERROR)
		(*answered)++;

	return (count_done(opt[OPT_COUNT_INDEX].u64, ctx->s.received));
}

static uint64_t	linger_ms(opt_t *opt)
{
	if (opt[OPT_LINGER_INDEX].u64)
		return (opt[OPT_LINGER_INDEX].u64 * 1000ULL);
	return (MAXWAIT_MS);
}

static int	poll_timeout(nfds_t nfd, uint64_t wait_start, uint64_t maxwait)
{
	uint64_t	spent = 0;
	uint64_t	left = 0;

	if (nfd != 1)
		return (TICK_MS);

	spent = now_ms() - wait_start;

	if (spent >= maxwait)
		return (0);

	left = maxwait - spent;
	if (left > (uint64_t)INT_MAX)
		return (INT_MAX);
	return ((int)left);
}

/////////////////////////////////////
//
//			LOOP
//
////////////////////////////////////

int ping_loop(context_t *ctx)
{
	if (!ctx)
		return (1);

	opt_t *opt = get_options(NULL);

	if (!opt)
		return (1);

	int s = 0;
	size_t n = 0;
	nfds_t nfd = 2;
	uint64_t wait_start = 0;
	uint64_t answered = 0;

	struct pollfd fds[0x2] = {
		{ .fd = ctx->fd, .events = POLLIN, .revents = 0  },
		{ .fd = ctx->tfd, .events = POLLIN, .revents = 0 },
	};

	ctx->s.transmitted = 0;
	ctx->s.received = 0;

	signal(SIGINT, sig_handler);
	signal(SIGALRM, sig_handler);

	if (opt[OPT_TIMEOUT_INDEX].u64)
		alarm((unsigned int)opt[OPT_TIMEOUT_INDEX].u64);

	send_one(ctx);

	nfd = sizeof(fds) / sizeof(fds[0]);

	while (g_run) {
		if (nfd != 1
			&& count_done(opt[OPT_COUNT_INDEX].u64, ctx->s.transmitted)) {
			nfd = 1;
			wait_start = now_ms();
		}

		if (nfd == 1 && answered >= ctx->s.transmitted)
			break ;

		s = poll(fds, nfd, poll_timeout(nfd, wait_start, linger_ms(opt)));
		
		if (s < 0) {
			if (errno == EINTR)
				continue ;
			break ;
		}

		if (!s) {
			if (nfd == 1)
				break ;
			continue ;
		}

		for (n = 0; n < nfd; n++) {
			if (fds[n].revents & POLLIN) {
				if (fds[n].fd == ctx->tfd)
					on_timer(ctx, opt);
				if (fds[n].fd == ctx->fd
					&& on_icmp(ctx, opt, &answered))
					return (0);
			}
			if (fds[n].revents & (POLLERR | POLLHUP | POLLNVAL)) {
				g_run = 0;
				break ;
			}
		}
	}

	return (0);
}
