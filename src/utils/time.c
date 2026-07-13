/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 09:09:12 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/07/06 14:37:47 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_ping.h"

/////////////////////////////////////
//
//			INCLUDES
//
////////////////////////////////////

#include <bits/types/struct_timeval.h>
#include <sys/time.h>
#include <sys/timerfd.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

/////////////////////////////////////
//
//			TIME
//
////////////////////////////////////

int create_timefd(const time_t seconds)
{
	struct itimerspec its = {
		.it_value.tv_sec = seconds,
		.it_value.tv_nsec = 0,
		.it_interval.tv_sec = seconds,
		.it_interval.tv_nsec = 0
	};

	fd_t tfd = timerfd_create(CLOCK_MONOTONIC, 0);

	if (tfd < 0) {
		perror("create_timerfd");
		return (-1);
	}

	if (timerfd_settime(tfd, 0, &its, NULL) < 0) {
		perror("timerfd_settime");
		close(tfd);
		return (-1);
	}

	return (tfd);
}

double get_ms(struct timeval *t)
{
	if (!t)
		return (0);

	struct timeval now = {
		.tv_sec = 0,
		.tv_usec = 0
	};
	
	long sec = 0;
	long usec = 0;

	if (gettimeofday(&now, NULL) < 0)
		return (0);

	sec = now.tv_sec - t->tv_sec;
	usec = now.tv_usec - t->tv_usec;

	return ((double) (sec * 1000.0) + (usec / 1000.0));
}
