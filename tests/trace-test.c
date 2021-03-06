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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <simple-trace/simple-trace.h>

#define fatal(ec, fmt, args...) do {                            \
        fprintf(stderr, "[ERROR] "fmt"\n", ## args);            \
        fflush(stderr);                                         \
        exit(ec);                                               \
    } while (0)

#define info(fmt, args...) do {                                 \
        fprintf(stdout, "[INFO] "fmt"\n", ## args);             \
        fflush(stdout);                                         \
    } while (0)





int main(int argc, char *argv[])
{
    int  ctx, i, n, done;
    char command[256];

    /* trace flags */
    int DBG_GRAPH, DBG_VAR, DBG_RESOLVE, DBG_ACTION, DBG_VM;

    TRACE_DECLARE_MODULE(test, "test",
        TRACE_FLAG("graph"  , "dependency graph"    , &DBG_GRAPH),
        TRACE_FLAG("var"    , "variable handling"   , &DBG_VAR),
        TRACE_FLAG("resolve", "dependency resolving", &DBG_RESOLVE),
        TRACE_FLAG("action" , "action processing"   , &DBG_ACTION),
        TRACE_FLAG("vm"     , "VM execution"        , &DBG_VM));

    
    trace_init();

    if ((ctx = trace_context_open("test")) < 0)
        fatal(1, "failed to create test trace context");
  
    if (trace_module_add(ctx, &test) != 0)
        fatal(1, "failed to add trace module test (%d: %s)",
              errno, strerror(errno));
            
    info("registered trace module test...");
            
    info("got debug flags:");
    info("graph: 0x%x, var: 0x%x, res: 0x%x, act: 0x%x, vm: 0x%x",
         DBG_GRAPH, DBG_VAR, DBG_RESOLVE, DBG_ACTION, DBG_VM);


#define SHOW(n) do {                               \
        printf("%s,%s,%s,%s,%s\n",                 \
               i & 0x01 ? "GRAPH" : "-",           \
               i & 0x02 ? "VAR" : "-",             \
               i & 0x04 ? "RESOLVE" : "-",         \
               i & 0x08 ? "ACTION" : "-",          \
               i & 0x10 ? "VM" : "-");             \
    } while (0)

#define FLIP(id, b)                                                     \
    ((i) & (0x1<<(b)) ? trace_flag_set : trace_flag_clr)(DBG_##id)


    trace_configure("*.*=+all");
    trace_context_enable(ctx);

    done = FALSE;
    while (!done) {

        trace_write(DBG_GRAPH  , "DBG_GRAPH");
        trace_write(DBG_VAR    , "DBG_VAR");
        trace_write(DBG_RESOLVE, "DBG_RESOLVE");
        trace_write(DBG_ACTION , "DBG_ACTION");
        trace_write(DBG_VM     , "DBG_VM");

        while (!done) {
            char *nl;

            printf("trace> ");
            if (fgets(command, sizeof(command), stdin) == NULL)
                continue;
            if ((nl = strchr(command, '\n')) != NULL)
                *nl = '\0';

            if (!strcmp(command, "quit") || !strcmp(command, "exit"))
                done = TRUE;
            else if (!strcmp(command, "show"))
                printf("I would if I could...\n");
            else if (!strcmp(command, "test") || !strcmp(command, "run"))
                break;
            else if (!strcmp(command, "help")) {
                printf("Possible commands are:\n");
                printf("quit: quit\n");
                printf("show: show current trace settings\n");
                printf("test: write one test trace message per flag\n");
                printf("context.module=[+|-]f1,...: change trace flags\n");
                printf("context > file: redirect trace context to file\n");
            }
            else
                trace_configure(command);
        }
    }

    if (trace_module_del(ctx, "test") != 0)
        fatal(1, "failed to remove trace module %s (%d: %s)", "test",
              errno, strerror(errno));
            
    info("test trace module %s removed...", "test");
    
    return 0;
}




/* 
 * Local Variables:
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 * vim:set expandtab shiftwidth=4:
 */
