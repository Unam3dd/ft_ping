/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 12:10:09 by stales            #+#    #+#             */
/*   Updated: 2026/07/01 17:58:09 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

static volatile int lock = 1;

static void sig_handler(int sig)
{
	if (sig == SIGINT)
		lock = 0;
}

static uint16_t biginc(uint16_t n, uint32_t *seq)
{
	uint16_t be = n >> 8 | n << 8;

	if (seq)
		*seq = be;

	be++;

	return (be >> 8 | be << 8);
}

static void loop(context_t *ctx, options_t *opt)
{
	if (!ctx || !opt)
		return ;

	struct epoll_event evs[2] = {
		{ 0, .data.fd = 0 },
		{ 0, .data.fd = 0 },
	};
	
	icmp_packet_t icmp;

	static const char d[] = "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f !\"#$%&'()*+,-./01234567";

	int n = 0, i = 0;

	memset(&icmp, 0, sizeof(icmp_packet_t));
	memcpy(icmp.data, d, strlen(d));

	icmp.hdr.un.echo.id = getpid();
	ctx->seq = 0x0100;

	icmp.hdr.un.echo.sequence = ctx->seq;

	while (lock) {

		n = epoll_wait(ctx->efd, evs, sizeof(evs)/sizeof(struct epoll_event), -1);

		if (n < 0)
			perror("epoll_wait");

		for (i = 0; i < n; i++) {
			
			if (evs[i].data.fd == ctx->tfd) {

				uint64_t expirations = 0;

				if (read(ctx->tfd, &expirations, sizeof(expirations)) < 0)
					perror("read");

				if (send_echo(ctx->fd, &ctx->dst, &icmp) < 0)
					perror("send_echo");

				icmp.hdr.un.echo.sequence = biginc(icmp.hdr.un.echo.sequence, &ctx->seq);
			}

			if (evs[i].events == EPOLLIN)
				recv_echo(ctx->fd, ctx);
		}
	}

}

int main(int ac, char **av)
{
	options_t opt = { 0 };
	context_t ctx = { 0 };

	memset(&ctx, 0, sizeof(context_t));

	if (parse_arguments(ac, av, &opt))
		return (1);

	if (init_context(&ctx, av[1]))
		return (1);

	signal(SIGINT, sig_handler);

	loop(&ctx, &opt);

	close_context(&ctx);

	return (0);
}
