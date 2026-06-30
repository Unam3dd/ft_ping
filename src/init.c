/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 22:45:20 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/06/25 23:15:36 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"

/////////////////////////////////////
//
//			INCLUDES
//
////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

/////////////////////////////////////
//
//			INFO
//
////////////////////////////////////

static int resolve_host(const char *dest, sin_t *sin)
{
	struct addrinfo spec, *res = NULL;
	int status = 0;
	
	memset(&spec, 0, sizeof(spec));

	spec.ai_family = AF_INET;
	spec.ai_flags = AI_CANONNAME;

	status = getaddrinfo(dest, NULL, &spec, &res);

	if (status) {
		fprintf(stderr, "error: %s\n", gai_strerror(status));
		return (1);
	}

	if (sin)
		*sin = *(sin_t *)res->ai_addr;

	freeaddrinfo(res);

	return (0);
}

/////////////////////////////////////
//
//			INIT
//
////////////////////////////////////

int init_context(context_t *ctx, const char *dest)
{
	if (!ctx || !dest)
		return (1);

	if (ctx->fd)
		return (0);

	struct epoll_event ev = { EPOLLIN | EPOLLET | EPOLLOUT, .data.fd = 0 };

	ctx->fd = socket(AF_INET, SOCK_RAW | SOCK_NONBLOCK, IPPROTO_ICMP);

	if (ctx->fd < 0) {
		perror("init_context");
		return (1);
	}

	if (resolve_host(dest, &ctx->dst)) {
		close(ctx->fd);
		return (1);
	}

	ctx->efd = epoll_create1(0);

	if (ctx->efd < 0) {
		perror("init_context");
		close(ctx->fd);
		return (1);
	}

	ev.data.fd = ctx->fd;

	if (epoll_ctl(ctx->efd, EPOLL_CTL_ADD, ctx->fd, &ev) < 0) {
		perror("init_context");
		close(ctx->fd);
		close(ctx->efd);
		return (1);
	}

	return (0);
}

/////////////////////////////////////
//
//			CLOSE
//
////////////////////////////////////

void close_context(context_t *ctx)
{
	if (!ctx)
		return ;

	if (epoll_ctl(ctx->efd, EPOLL_CTL_DEL, ctx->fd, NULL) < 0) {
		perror("close_context");
		return ;
	}

	close(ctx->fd);
	close(ctx->efd);

	memset(ctx, 0, sizeof(context_t));
}

