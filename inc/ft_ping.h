/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 21:15:42 by stales            #+#    #+#             */
/*   Updated: 2026/06/30 22:21:49 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
#define FT_PING_H

#if ! defined(__linux__)
#error "This project compile only on Linux"
#endif

/////////////////////////////////////
//
//			INCLUDES
//
////////////////////////////////////

#include <stddef.h>
#include <stdint.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

/////////////////////////////////////
//
//			ENUM
//
////////////////////////////////////

typedef enum e_bool_t
{
	FALSE,
	TRUE
} bool_t;

/////////////////////////////////////
//
//			TYPEDEFS
//
////////////////////////////////////

typedef int fd_t;
typedef struct sockaddr_in sin_t;
typedef struct icmphdr icmphdr_t;
typedef struct s_options_t options_t;
typedef struct s_context_t context_t;

/////////////////////////////////////
//
//			STRUCTS
//
////////////////////////////////////

struct s_options_t
{
	uint32_t count;
};

struct s_context_t
{
	sin_t dst;
	fd_t fd;
	fd_t efd;
};

/////////////////////////////////////
//
//			OPTIONS
//
////////////////////////////////////

int parse_arguments(int ac, char **av, options_t *opt);

/////////////////////////////////////
//
//			HELP
//
////////////////////////////////////

void show_usage(void);

/////////////////////////////////////
//
//			INIT
//
////////////////////////////////////

int init_context(context_t *ctx, const char *dest);
void close_context(context_t *ctx);

/////////////////////////////////////
//
//			CHECKSUM
//
////////////////////////////////////

unsigned short checksum(void *b, int len);

/////////////////////////////////////
//
//			ICMP
//
////////////////////////////////////

int send_ping(const context_t *ctx, const sin_t *dst, const icmphdr_t *icmp);

#endif
