/*************************************************************************
This file is part of libtrace

Copyright (C) 2010 Nokia Corporation.

This library is free software; you can redistribute
it and/or modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation
version 2.1 of the License.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301
USA.
*************************************************************************/


#ifndef __SIMPLE_TRACE_H__
#define __SIMPLE_TRACE_H__

#include <stddef.h>                                 /* NULL */
#include <sys/types.h>                              /* size_t */
#include <sys/time.h>

#if !defined(likely) || !defined(unlikely)
#  undef likely
#  undef unlikely
#  ifdef __GNUC__
#    define likely(cond)   __builtin_expect((cond), 1)
#    define unlikely(cond) __builtin_expect((cond), 0)
#  else
#    define likely(cond)   (cond)
#    define unlikely(cond) (cond)
#  endif
#endif


#if !defined(FALSE) || !defined(TRUE)
#  undef FALSE
#  undef TRUE
#  define FALSE 0
#  define TRUE  1
#endif

#define TRACE_DEFAULT_CONTEXT 0
#define TRACE_DEFAULT_NAME    "default"
#define TRACE_DEFAULT_FORMAT  "[%C] "

/*
 * a trace flag definition
 */

typedef struct {
    char *name;                              /* symbolic flag name */
    char *descr;                             /* description of flag */
    int  *flagptr;                           /* reported to trace 'client' */
} trace_flagdef_t;


#define TRACE_FLAG(n, d, fptr) {                          \
        .name    = n,                                     \
        .descr   = d,                                     \
        .flagptr = fptr,                                  \
    }

#define TRACE_FLAG_END { .name = NULL, .descr = NULL }



/*
 * a trace module definition (a named set of trace flags)
 */

typedef struct {
    char            *name;                   /* symbolic module name */
    trace_flagdef_t *flags;                  /* trace flags of this module */
    int              nflag;                  /* number of flags */
} trace_moduledef_t;


#define TRACE_DECLARE_MODULE(v, n, ...)                                   \
    trace_flagdef_t __trace_flags_##v[] = {                               \
        __VA_ARGS__,                                                      \
        TRACE_FLAG_END,                                                   \
    };                                                                    \
    trace_moduledef_t v = {                                               \
        .name  = (n),                                                     \
        .flags = __trace_flags_##v,                                       \
        .nflag = sizeof(__trace_flags_##v) / sizeof(__trace_flags_##v[0]) \
    }



/*
 * trace destinations
 */

#define TRACE_TO_STDERR     ((char *)0x0)
#define TRACE_TO_STDOUT     ((char *)0x1)
#define TRACE_TO_FILE(path) (path)



/*
 * macro to generate trace messages
 */

#define trace_write(id, format, args...)        \
    __trace_printf(id, __FILE__, __LINE__, __FUNCTION__, format"\n", ## args)
#define trace_printf(id, format, args...)        \
    __trace_printf(id, __FILE__, __LINE__, __FUNCTION__, format"\n", ## args)




/*
 * public API
 */

int  trace_init(void);
void trace_exit(void);

int  trace_context_open(const char *name);
int  trace_context_close(int cid);
int  trace_context_format(int cid, const char *format);
int  trace_context_target(int cid, const char *target);
int  trace_context_enable(int cid);
int  trace_context_disable(int cid);

int  trace_add_module(int cid, trace_moduledef_t *module);
int  trace_del_module(int cid, const char *name);

int  trace_flag_set(int id);
int  trace_flag_clr(int id);
int  trace_flag_tst(int id);

int  trace_configure(const char *config);
int  trace_show(char *context, char *buf, size_t bufsize, const char *format);

int  __trace_printf(int id, const char *file, int line, const char *func,
                    const char *format, ...);


#endif /* __SIMPLE_TRACE_H__ */


/* 
 * Local Variables:
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 * vim:set expandtab shiftwidth=4:
 */



