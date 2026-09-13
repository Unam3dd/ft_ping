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
	const char	*host;
	int		loss;

	if (!s || !r)
		return;

	o = get_options(NULL);
	host = o[OPT_HOST_INDEX].str;
	if (!host)
		host = "?";
	loss = 0;
	if (s->transmitted)
		loss = (int)(((s->transmitted - s->received) * 100)
			/ s->transmitted);

	printf("--- %s ping statistics ---\n", host);
	printf("%lu packets transmitted, %lu packets received, ",
		(unsigned long)s->transmitted, (unsigned long)s->received);
	if (s->received > s->transmitted)
		printf("-- somebody is printing forged packets!");
	else if (s->transmitted)
		printf("%d%% packet loss", loss);
	printf("\n");
	rtt_show(r);
}
