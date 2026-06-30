/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checksum.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 22:00:09 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/06/30 22:02:32 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"

/////////////////////////////////////
//
//			CHECKSUM
//
////////////////////////////////////

unsigned short checksum(void *b, int len)
{
	if (!b || len < 0)
		return (0);

    unsigned short *buf = (unsigned short*)b;
    uint64_t sum = 0;

    for (sum = 0; len > 1; len -= sizeof(*buf))
        sum += *buf++;

    if (len == 1)
        sum += *(unsigned char*)buf;

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);

    return (unsigned short)(~sum);
}

