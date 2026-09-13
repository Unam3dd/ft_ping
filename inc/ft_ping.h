/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 21:15:42 by stales            #+#    #+#             */
/*   Updated: 2026/07/10 07:35:00 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
#define FT_PING_H

#include <bits/types/struct_timeval.h>
#include <sys/types.h>
#include <time.h>

#if ! defined(__linux__)
#error "This project compile only on Linux"
#endif

/////////////////////////////////////
//
//			CREDITS
//
////////////////////////////////////

#ifndef FT_PING_AUTHOR
#define FT_PING_AUTHOR "stales"
#endif

#ifndef FT_PING_VERSION
#define FT_PING_VERSION "0.0.1"
#endif

/////////////////////////////////////
//
//			INCLUDES
//
////////////////////////////////////

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <sys/time.h>

/////////////////////////////////////
//
//			TYPEDEFS
//
////////////////////////////////////

typedef enum e_bool_t bool_t;
typedef enum e_opt_type_t opt_type_t;
typedef enum e_opt_index_t opt_index_t;
typedef struct s_opt_t opt_t;
typedef struct sockaddr_in sin_t;
typedef struct s_context_t context_t;
typedef struct icmphdr icmphdr_t;
typedef struct iphdr iphdr_t;
typedef struct s_icmp_pkt_t icmp_pkt_t;
typedef struct s_icmp_dest_unreach_t icmp_dest_unreach_t;
typedef struct s_icmp_msg_t icmp_msg_t;
typedef struct s_icmp_res_t icmp_res_t;
typedef struct s_icmp_stat_t icmp_stat_t;
typedef struct s_rtt_t rtt_t;
typedef uint64_t timestamp_t;
typedef int fd_t;

/////////////////////////////////////
//
//			ENUM
//
////////////////////////////////////

enum __attribute__((packed)) e_bool_t
{
	FALSE,
	TRUE
};

enum e_opt_type_t
{
	BOOL,
	INT_32,
	UINT_32,
	INT_64,
	UINT_64,
	FLOAT_32,
	FLOAT_64,
	STRING,
};

enum e_opt_index_t
{
	OPT_HOST_INDEX,
	OPT_COUNT_INDEX,
	OPT_VERBOSE_INDEX,
	OPT_TTL_INDEX,
	OPT_NUMERIC_INDEX,
	OPT_DEADLINE_INDEX,
};

/////////////////////////////////////
//
//			STRUCTS
//
////////////////////////////////////

struct s_icmp_stat_t
{
	uint64_t transmitted;
	uint64_t received;
};

struct s_rtt_t
{
	double			min;
	double			max;
	double			sum;
	double			sumsq;
	uint64_t		count;
	uint64_t		elapsed_ms;
	struct timeval	start;
};

struct s_opt_t
{
	const char *key;
	bool_t     required;
	union {
		void    *value;
		char    *str;
		uint64_t u64;
		uint32_t u32;
		int64_t i64;
		int32_t i32;
		double  f64;
		float   f32;
		bool_t  bool;
	};
	
	opt_type_t type;
	
	size_t size;
};

struct s_context_t
{
	icmp_stat_t	s;
	rtt_t		rtt;
	sin_t		sin;
	fd_t		fd;
	fd_t		tfd;
};

struct s_icmp_pkt_t
{
	icmphdr_t h;
	struct timeval t;
	uint8_t data[0x28];
};

struct __attribute__((packed)) s_icmp_dest_unreach_t
{
	icmphdr_t hdr;
	iphdr_t iphdr;
	icmp_pkt_t pkt;
};

struct s_icmp_msg_t
{
	uint8_t		type;
	uint8_t		code;
	const char	*msg;
};


/////////////////////////////////////
//
//			MACRO
//
////////////////////////////////////

#define ARGOPT(k,r,t,s) { k, r, { NULL }, t, s}
#define BIG16(n) (((n >> 8 | n << 8) & 0xFFFF)) 

/////////////////////////////////////
//
//			OPTIONS
//
////////////////////////////////////

opt_t *get_options(size_t *size);

/////////////////////////////////////
//
//			PARSING
//
////////////////////////////////////

uint64_t parse_unumber(const char *str);
int parse_arguments(int ac, char **av, opt_t *options);

/////////////////////////////////////
//
//			HELP
//
////////////////////////////////////

void show_usage(void);

/////////////////////////////////////
//
//			VERSION
//
////////////////////////////////////

void show_version(void);

/////////////////////////////////////
//
//			NETWORK
//
////////////////////////////////////

int resolve_host(const char *host, sin_t *sin);
int create_socket(void);
int set_socket_ttl(fd_t fd, uint32_t ttl);
int send_icmp_echo(context_t *ctx, const fd_t fd, const sin_t *dst);
int recv_icmp_echo(context_t *ctx);
int ip_icmp_ok(const char *buf, int size, int offset, int *hlen);

/////////////////////////////////////
//
//			PROGRAM
//
////////////////////////////////////

int ping_program(context_t *ctx, const char *host);
int ping_loop(context_t *ctx);

/////////////////////////////////////
//
//			CHECKSUM
//
////////////////////////////////////

uint16_t checksum(void *b, int len);

/////////////////////////////////////
//
//			UTILS
//
////////////////////////////////////

int  create_timefd(const time_t seconds);
double get_ms(struct timeval *t);

/////////////////////////////////////
//
//			STATS
//
////////////////////////////////////

void show_stats(const icmp_stat_t *s, const rtt_t *r);

/////////////////////////////////////
//
//			RTT
//
////////////////////////////////////

void	rtt_init(rtt_t *r);
void	rtt_start(rtt_t *r);
void	rtt_stop(rtt_t *r);
void	rtt_add(rtt_t *r, double ms);
void	rtt_show(const rtt_t *r);

/////////////////////////////////////
//
//			VERBOSE
//
////////////////////////////////////

void	verbose_dump(iphdr_t *embed_ip, icmphdr_t *embed_icmp);
int	handle_icmp_error(context_t *ctx, const char *buf, int size);

#endif
