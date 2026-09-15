/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   number.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 18:31:25 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/07/02 18:32:06 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_ping.h"

/////////////////////////////////////
//
//			INCLUDES
//
////////////////////////////////////

#include <errno.h>
#include <stdlib.h>
#include <string.h>

/////////////////////////////////////
//
//			NUMBER
//
////////////////////////////////////

uint64_t parse_unumber(const char *str)
{
    if (!str || !*str)
    {
        errno = EINVAL;
        return (0);
    }

    if (strspn(str, "0123456789") != strlen(str))
    {
        errno = EINVAL;
        return (0);
    }

    uint64_t value = 0;

    errno = 0;
    value = strtoull(str, NULL, 10);

    return (errno == ERANGE ? 0 : value);
}
