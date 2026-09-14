/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 11:51:01 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/07/08 11:10:00 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_ping.h"

/////////////////////////////////////
//
//			INCLUDES
//
////////////////////////////////////

#include <errno.h>
#include <signal.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

/////////////////////////////////////
//
//			VOLATILE
//
////////////////////////////////////

static volatile bool_t run = TRUE;

/////////////////////////////////////
//
//			SIGNAL
//
////////////////////////////////////

static void sig_handler(int sig)
{
	if (sig == SIGINT)
		write(STDOUT_FILENO, "\n", 1);
	if (sig == SIGINT || sig == SIGALRM)
		run = FALSE;
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

static int	send_one(context_t *ctx)
{
	if (send_icmp_echo(ctx, ctx->fd, &ctx->sin) < 0) {
		perror("send_icmp_echo");
		return (1);
	}
	ctx->s.transmitted++;
	return (0);
}

static int	on_timer(context_t *ctx, opt_t *opt)
{
	uint64_t	exp = 0;

	if (read(ctx->tfd, &exp, sizeof(exp)) < 0)
		perror("read");
	if (count_done(opt[OPT_COUNT_INDEX].u64, ctx->s.transmitted))
		return (0);
	return (send_one(ctx));
}

static int	on_icmp(context_t *ctx, opt_t *opt)
{
	if (!recv_icmp_echo(ctx))
		ctx->s.received++;
	return (count_done(opt[OPT_COUNT_INDEX].u64, ctx->s.received));
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

	uint64_t expiration = 0;
	int bytes = 0;
	int s = 0;
	size_t n = 0;
	nfds_t nfd = 2;

	struct pollfd fds[0x2] = {
		{ .fd = ctx->fd, .events = POLLIN  },
		{ .fd = ctx->tfd, .events = POLLIN },
	};

	ctx->s.transmitted = 0;
	ctx->s.received = 0;

	signal(SIGINT, sig_handler);
	signal(SIGALRM, sig_handler);

	if (opt[OPT_DEADLINE_INDEX].u64)
		alarm((unsigned int)opt[OPT_DEADLINE_INDEX].u64);

	if (send_one(ctx))
		return (1);

	nfd = sizeof(fds) / sizeof(fds[0]);

	while (run) {
		if (count_done(opt[OPT_COUNT_INDEX].u64, ctx->s.transmitted))
			nfd = 1;

		s = poll(fds, nfd, 3500);
		
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
				if (fds[n].fd == ctx->tfd && on_timer(ctx, opt))
					return (1);
				if (fds[n].fd == ctx->fd && on_icmp(ctx, opt))
					return (0);
			}
			if (fds[n].revents & (POLLERR | POLLHUP | POLLNVAL)) {
				run = FALSE;
				break ;
			}
		}
	}

	return (0);
}
