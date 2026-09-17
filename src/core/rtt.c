/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 16:58:00 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/07/12 13:32:00 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_ping.h"
#include <math.h>
#include <stdio.h>
#include <sys/time.h>

/////////////////////////////////////
//
//			STATIC
//
////////////////////////////////////

static double rtt_mdev(const rtt_t *r)
{
    if (!r || !r->count)
        return (0.0);

    double avg = 0;
    double var = 0;

    avg = r->sum / (double)r->count;
    var = (r->sumsq / (double)r->count) - (avg * avg);

    if (var < 0.0)
        var = 0.0;

    return (sqrt(var));
}

/////////////////////////////////////
//
//			RTT
//
////////////////////////////////////

void rtt_init(rtt_t *r)
{
    if (!r)
        return;

    r->min = 0.0;
    r->max = 0.0;
    r->sum = 0.0;
    r->sumsq = 0.0;
    r->count = 0;
    r->elapsed_ms = 0;

    memset(&r->start, 0, sizeof(r->start));
}

void rtt_start(rtt_t *r)
{
    if (!r)
        return;

    gettimeofday(&r->start, NULL);
}

void rtt_stop(rtt_t *r)
{
    struct timeval now;
    long sec = 0;
    long usec = 0;

    if (!r)
        return;

    if (gettimeofday(&now, NULL) < 0)
        return;

    sec = now.tv_sec - r->start.tv_sec;
    usec = now.tv_usec - r->start.tv_usec;
    r->elapsed_ms = (uint64_t)((sec * 1000) + (usec / 1000));
}

void rtt_add(rtt_t *r, double ms)
{
    if (!r || ms < 0.0)
        return;

    if (!r->count)
    {
        r->min = ms;
        r->max = ms;
    }
    else
    {
        if (ms < r->min)
            r->min = ms;

        if (ms > r->max)
            r->max = ms;
    }

    r->sum += ms;
    r->sumsq += (ms * ms);
    r->count++;
}

void rtt_show(const rtt_t *r)
{
    if (!r || !r->count)
        return;

    double avg = 0;
    double mdev = 0;

    avg = r->sum / (double)r->count;
    mdev = rtt_mdev(r);
    printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n", r->min, avg,
           r->max, mdev);
}
