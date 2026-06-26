/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 12:10:09 by stales            #+#    #+#             */
/*   Updated: 2026/06/25 23:13:15 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"
#include <stdio.h>

int main(int ac, char **av)
{
	options_t opt = { 0 };
	context_t ctx = { 0 };

	if (parse_arguments(ac, av, &opt))
		return (1);

	return (0);
}
