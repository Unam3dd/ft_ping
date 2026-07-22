/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   program.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 14:04:21 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/07/08 11:00:59 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_ping.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>

/////////////////////////////////////
//
//			STATIC
//
////////////////////////////////////

static int start_program(sin_t *sin, const char *host)
{
	if (!sin || !host)
		return (1);

	char			buf[0x40];
	opt_t			*o = get_options(NULL);
	pid_t			pid = getpid() & 0xFFFF;
	size_t			data_size = sizeof(icmp_pkt_t) - sizeof(icmphdr_t);
	size_t			total_size = data_size + sizeof(icmphdr_t) + sizeof(iphdr_t);

	memset(buf, 0, sizeof(buf));

	if (!inet_ntop(AF_INET, &sin->sin_addr, buf, sizeof(buf))) {
		perror("inet_ntop");
		return (1);
	}

	if (o[OPT_VERBOSE_INDEX].bool)
		printf("PING %s (%s) %zu(%zu) bytes of data, id = %d\n",
			host, buf, data_size, total_size, pid);
	else
		printf("PING %s (%s) %zu(%zu) bytes of data.\n",
			host, buf, data_size, total_size);

	return (0);
}

static int init_sockets(context_t *ctx)
{
	if (!ctx)
		return (1);

	opt_t	*opt = get_options(NULL);

	ctx->fd = create_socket();

	if (ctx->fd < 0)
		return (1);

	if (opt[OPT_TTL_INDEX].u32
		&& set_socket_ttl(ctx->fd, opt[OPT_TTL_INDEX].u32) < 0) {
		close(ctx->fd);
		return (1);
	}

	ctx->tfd = create_timefd(1);

	if (ctx->tfd < 0) {
		close(ctx->fd);
		return (1);
	}

	return (0);
}

static void close_sockets(context_t *ctx)
{
	if (!ctx)
		return ;

	close(ctx->fd);
	close(ctx->tfd);
}

/////////////////////////////////////
//
//			PROGRAM
//
////////////////////////////////////

int ping_program(context_t *ctx, const char *host)
{
	static char	numeric_host[INET_ADDRSTRLEN];
	opt_t		*opt;

	if (!ctx || !host)
		return (1);

	opt = get_options(NULL);
	if (init_sockets(ctx))
		return (1);

	if (resolve_host(host, &ctx->sin)) {
		close_sockets(ctx);
		return (1);
	}

	if (opt[OPT_NUMERIC_INDEX].bool) {
		memset(numeric_host, 0, sizeof(numeric_host));
		if (!inet_ntop(AF_INET, &ctx->sin.sin_addr, numeric_host,
				sizeof(numeric_host))) {
			perror("inet_ntop");
			close_sockets(ctx);
			return (1);
		}
		opt[OPT_HOST_INDEX].str = numeric_host;
		host = numeric_host;
	}

	if (start_program(&ctx->sin, host)) {
		close_sockets(ctx);
		return (1);
	}

	rtt_init(&ctx->rtt);
	rtt_start(&ctx->rtt);

	// Loop
	ping_loop(ctx);

	rtt_stop(&ctx->rtt);

	// Stats
	show_stats(&ctx->s, &ctx->rtt);

	// Close sockets
	close_sockets(ctx);

	return (!ctx->s.received);
}
