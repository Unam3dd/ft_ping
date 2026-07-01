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

#include <bits/types/struct_itimerspec.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/timerfd.h>

/////////////////////////////////////
//
//			INFO
//
////////////////////////////////////

static int resolve_host(const char *dest, sin_t *dst, char *cname, const size_t csize)
{
	struct addrinfo spec, *res = NULL;
	int status = 0;

	if (!dest || !dst || !cname || !csize)
		return (1);
	
	memset(&spec, 0, sizeof(spec));

	spec.ai_family = AF_INET;
	spec.ai_flags = AI_CANONNAME;

	status = getaddrinfo(dest, NULL, &spec, &res);

	if (status) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		return (1);
	}

	if (dst)
		*dst = *(sin_t *)res->ai_addr;

	status = getnameinfo((struct sockaddr *)res->ai_addr, sizeof(sin_t), cname, csize, NULL, 0, 0);

	if (status) {
		freeaddrinfo(res);
		fprintf(stderr, "getnameinfo error: %s\n", gai_strerror(status));
		return (1);
	}

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

	struct itimerspec its = { .it_value.tv_sec = 1, .it_value.tv_nsec = 0, .it_interval.tv_sec = 1, .it_interval.tv_nsec = 0 };

	struct epoll_event ev = { EPOLLIN, .data.fd = 0 };

	ctx->fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	if (ctx->fd < 0) {
		perror("init_context");
		return (1);
	}

	if (resolve_host(dest, &ctx->dst, ctx->ni_name, sizeof(ctx->ni_name))) {
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

	ctx->tfd = timerfd_create(CLOCK_MONOTONIC, 0);

	timerfd_settime(ctx->tfd, 0, &its, NULL);
	
	ev.data.fd = ctx->tfd;
	ev.events = EPOLLIN;

	if (epoll_ctl(ctx->efd, EPOLL_CTL_ADD, ctx->tfd, &ev) < 0) {
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

	if (epoll_ctl(ctx->efd, EPOLL_CTL_DEL, ctx->tfd, NULL) < 0) {
		perror("close_context");
		return ;
	}

	close(ctx->fd);
	close(ctx->efd);
	close(ctx->tfd);

	memset(ctx, 0, sizeof(context_t));
}

