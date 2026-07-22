/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 10:58:50 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/07/08 13:01:49 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_ping.h"
#include <stdio.h>

/////////////////////////////////////
//
//			STATS
//
////////////////////////////////////

void show_stats(const icmp_stat_t *s, const rtt_t *r)
{
	const opt_t	*o;
	float		percentage;

	if (!s || !r)
		return;

	o = get_options(NULL);
	percentage = 0;

	if (s->transmitted)
		percentage = (float)(100 - (((float)s->received
						/ (float)s->transmitted) * 100));

	printf("--- %s ping statistics ---\n", o[OPT_HOST_INDEX].str);
	printf("%ld packets transmitted, %ld received, %d%% packet loss\n",
		s->transmitted, s->received, (uint32_t)(percentage + 0.5f));
	rtt_show(r);
}
