/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argparse.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 08:10:55 by stales            #+#    #+#             */
/*   Updated: 2026/03/19 16:19:20 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_ARGS_H
#define FT_ARGS_H

/////////////////////////////////////
//
//			INCLUDES
//
////////////////////////////////////

#include "ft_ping.h"
#include <stddef.h>
#include <stdint.h>

/////////////////////////////////////
//
//			TYPEDEFS
//
////////////////////////////////////

typedef struct s_args_t args_t;
typedef struct s_arg_opt_t arg_opt_t;

/////////////////////////////////////
//
//			ENUM
//
////////////////////////////////////

typedef enum e_arg_flag_t
{
    ARG_OPTIONAL = 1 << 0,
    ARG_HELP = 1 << 1,
    ARG_REQUIRED = 1 << 2,
    ARG_PARAM = 1 << 3,
    ARG_MARK = 1 << 4,
} arg_flag_t;

typedef enum e_arg_type_t
{
    ARG_NONE,
    ARG_BOOL,
    ARG_SHORT,
    ARG_USHORT,
    ARG_INT,
    ARG_UINT,
    ARG_LONG,
    ARG_ULONG,
    ARG_STRING,
} arg_type_t;

typedef enum e_arg_status_t
{
    E_ARG_OK = 0,
    E_ARG_NOT_OPTIONAL = -1,
    E_ARG_UNK = -2,
    E_ARG_BAD_FMT = -3,
    E_ARG_NULL = -4,
    E_ARG_NOT_FLAGS = -5,
    E_ARG_ALREADY_SET = -6,
    E_ARG_PARSE_NOT_NBR = -7,
    E_ARG_PARSE_NOT_BOOL = -8,
	E_ARG_MISS_PARAMS = -9
} arg_status_t;

/////////////////////////////////////
//
//			STRUCT
//
////////////////////////////////////

struct s_arg_opt_t
{
    const char *opt;
    const char *alias;
    const char *help;

    union
    {
        void *ptr;
        char *str;
        uint64_t u64;
        int64_t i64;
        uint32_t u32;
        int32_t i32;
        uint16_t u16;
        int16_t i16;
        uint8_t u8;
        int8_t i8;
        bool_t bool;
    };

    arg_flag_t flags;
    arg_type_t type;
};

struct s_args_t
{
    const char *name;
    const char *usage;
    const char *credit;
    arg_opt_t *opt;
    size_t nopt;
};

/////////////////////////////////////
//
//			MACRO
//
////////////////////////////////////

#define ARGPARSE_CNT(opt) (sizeof(opt) / sizeof(arg_opt_t))

#define ARGPARSE_GET(n) args_##n

#define ARGPARSE(n) arg_opt_t args_##n[] = {

#define ARGPARSE_END() };

#define ARGPARSE_OPT(opt, alias, help, value, flags, type) {opt, alias, help, { value }, flags, type}

#define ARGPARSE_REQUIRED(opt, help, type) ARGPARSE_OPT(opt, opt, help, NULL, ARG_REQUIRED | ARG_PARAM, type)

#define ARGPARSE_HELP() ARGPARSE_OPT("h", "help", "\tshow this help", NULL, ARG_HELP | ARG_OPTIONAL, ARG_NONE)

#define ARGPARSE_ZERO(args) memset(&args, 0, sizeof(args_t))

#define ARGPARSE_NAME(args, n) args.name = n;
#define ARGPARSE_USAGE(args, n) args.usage = n;
#define ARGPARSE_CREDIT(args, n) args.credit = n;

#define ARGPARSE_MARK_ARG(arg) (arg->flags |= ARG_MARK)
#define ARGPARSE_UNMARK_ARG(arg) (arg->flags &= ~(ARG_MARK))

#define ARGPARSE_ARG_HAS(arg, test) (arg->flags & test)

#define ARGPARSE_ARG_HAS_NOT(arg, test) (!(arg->flags & test))
#define ARGPARSE_ARG_HAS_NOT_REQUIRED(arg) (ARGPARSE_ARG_HAS_NOT(arg, ARG_REQUIRED))

/////////////////////////////////////
//
//			STATIC INLINE
//
////////////////////////////////////

static inline void argparse_init(args_t *args, arg_opt_t *opt,
                                 const size_t nopt)
{
    if (!args || !opt || !nopt)
        return;

    args->opt = opt;
    args->nopt = nopt;
}

/////////////////////////////////////
//
//			ARGPARSE
//
////////////////////////////////////

int argparse_parse(args_t *args, int ac, char **av);
void argparse_show_usage(const args_t *args);

#endif
