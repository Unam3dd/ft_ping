/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getopt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/21 09:15:52 by stales            #+#    #+#             */
/*   Updated: 2026/03/21 09:21:39 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/argparse.h"

/////////////////////////////////////
//
//			INCLUDES
//
////////////////////////////////////

#include <string.h>

arg_opt_t *argparse_get(args_t *args, const char *name, const char *alias)
{
	if (!args || !name)
		return (NULL);

	for (uint32_t i = 0; i < args->nopt; i++) {

		if (!strcmp(args->opt[i].opt, name))
			return (&args->opt[i]);

		if (alias && args->opt[i].alias && !strcmp(args->opt[i].alias, name))
			return (&args->opt[i]);
	}

	return (NULL);
}
