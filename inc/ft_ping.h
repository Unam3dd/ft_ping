/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 21:15:42 by stales            #+#    #+#             */
/*   Updated: 2026/07/01 17:54:29 by sam0verfl0w      ###   ########.fr       */
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
#include <sys/time.h>
#include <sys/epoll.h>

/////////////////////////////////////
//
//			DEFINES
//
////////////////////////////////////

#ifndef NI_MAXHOST
#define NI_MAXHOST 1025
#endif

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
typedef struct s_icmp_packet_t icmp_packet_t;

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
	char 	ni_name[NI_MAXHOST];
	sin_t  	dst;
	fd_t 	fd;
	fd_t 	efd;
	fd_t    tfd;
	uint32_t seq;
};

struct s_icmp_packet_t
{
	icmphdr_t 		hdr;
	struct timeval 	timestamp;
	uint8_t 		data[0x28];
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

int 	send_echo(const fd_t fd, const sin_t *sin, icmp_packet_t *pkt);
int     recv_echo(const fd_t fd, const context_t *ctx);

/////////////////////////////////////
//
//			DISPLAY
//
////////////////////////////////////

void display_response(const context_t *ctx, const icmphdr_t *hdr, const size_t size);

#endif
