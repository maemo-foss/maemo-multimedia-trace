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
#include <check.h>

#include <simple-trace/simple-trace.h>
#include "check-libtrace.h"

#define CONTEXT_NAME "test"


static int cid;
static int DBG_FOO, DBG_BAR, DBG_FOOBAR;

TRACE_DECLARE_MODULE(moduletest, "test",
                     TRACE_FLAG("foo"   , "flag foo"   , &DBG_FOO),
                     TRACE_FLAG("bar"   , "flag bar"   , &DBG_BAR),
                     TRACE_FLAG("foobar", "flag foobar", &DBG_FOOBAR));


static void
setup(void)
{
    fail_unless(trace_init() == 0);
    fail_unless((cid = trace_context_open(CONTEXT_NAME)) >= 0);
}


static void
teardown(void)
{
    fail_unless(trace_context_close(cid) == 0);
}


START_TEST(multiple_register)
{
    fail_unless(trace_add_module(cid, &moduletest) >= 0);
    fail_unless(DBG_FOO >= 0 && DBG_BAR >= 0 && DBG_FOOBAR >= 0);
    fail_unless(trace_add_module(cid, &moduletest) < 0);
    fail_unless(trace_del_module(cid, moduletest.name) == 0);
}
END_TEST


START_TEST(spurious_unregister)
{
    fail_unless(trace_del_module(cid, "foobar") < 0);
}
END_TEST


START_TEST(register_unregister)
{
    fail_unless(trace_add_module(cid, &moduletest) >= 0);
    fail_unless(DBG_FOO >= 0 && DBG_BAR >= 0 && DBG_FOOBAR >= 0);
    fail_unless(trace_del_module(cid, moduletest.name) == 0);
}
END_TEST


START_TEST(multiple_unregister)
{
    fail_unless(trace_add_module(cid, &moduletest) >= 0);
    fail_unless(DBG_FOO >= 0 && DBG_BAR >= 0 && DBG_FOOBAR >= 0);
    fail_unless(trace_del_module(cid, moduletest.name) == 0);
    fail_unless(trace_del_module(cid, moduletest.name) < 0);
}
END_TEST


void
chktrace_module_tests(Suite *suite)
{
    TCase *tc;

    tc = tcase_create("trace modules");
    tcase_add_checked_fixture(tc, setup, teardown);

    tcase_add_test(tc, multiple_register);
    tcase_add_test(tc, spurious_unregister);
    tcase_add_test(tc, register_unregister);
    tcase_add_test(tc, multiple_unregister);
    suite_add_tcase(suite, tc);
}



/* 
 * Local Variables:
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 * vim:set expandtab shiftwidth=4:
 */


