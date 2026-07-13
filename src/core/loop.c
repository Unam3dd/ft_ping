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
	if (sig == SIGINT || sig == SIGALRM)
		run = FALSE;
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
	signal(SIGABRT, sig_handler);

	bytes = send_icmp_echo(ctx, ctx->fd, &ctx->sin);

	if (bytes < 0) {
		perror("send_icmp_echo");
		return (1);
	}

	ctx->s.transmitted++;

	nfd = sizeof(fds)/sizeof(fds[0]);

	while (run) {

		if (opt[OPT_COUNT_INDEX].u64 && ctx->s.transmitted >= opt[OPT_COUNT_INDEX].u64)
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

			if (!(fds[n].revents & POLLIN))
				continue ;

			if (fds[n].fd == ctx->tfd) {

				if (read(ctx->tfd, &expiration, sizeof(expiration)) < 0)
					perror("read");

				if (opt[OPT_COUNT_INDEX].u64 && ctx->s.transmitted >= opt[OPT_COUNT_INDEX].u64)
					continue ;

				bytes = send_icmp_echo(ctx, ctx->fd, &ctx->sin);

				if (bytes < 0) {
					perror("send_icmp_echo");
					return (1);
				}

				ctx->s.transmitted++;
			}

			if (fds[n].fd == ctx->fd) {

				if (!recv_icmp_echo(ctx))
					ctx->s.received++;

				if (opt[OPT_COUNT_INDEX].u64 && ctx->s.received >= opt[OPT_COUNT_INDEX].u64)
					return (0);
			}

		}
	}

	return (0);
}
