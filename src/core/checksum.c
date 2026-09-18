/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checksum.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 22:00:09 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/09/18 18:02:32 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_ping.h"

/////////////////////////////////////
//
//			CHECKSUM
//
////////////////////////////////////

uint16_t checksum(void *b, int len)
{
	if (!b || len < 0)
		return (0);

    uint16_t *buf = (unsigned short*)b;
    uint64_t s = 0;

    for (s = 0; len > 1; len -= sizeof(*buf))
        s += *buf++;

    if (len == 1)
        s += *(unsigned char*)buf;

    s = (s >> 16) + (s & 0xFFFF);
    s += (s >> 16);

    return (unsigned short)(~s);
}

