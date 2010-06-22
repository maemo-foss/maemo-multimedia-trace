/******************************************************************************/
/*  This file is part of libtrace                                             */
/*                                                                            */
/*  Copyright (C) 2010 Nokia Corporation.                                     */
/*                                                                            */
/*  This library is free software; you can redistribute                       */
/*  it and/or modify it under the terms of the GNU Lesser General Public      */
/*  License as published by the Free Software Foundation                      */
/*  version 2.1 of the License.                                               */
/*                                                                            */
/*  This library is distributed in the hope that it will be useful,           */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU          */
/*  Lesser General Public License for more details.                           */
/*                                                                            */
/*  You should have received a copy of the GNU Lesser General Public          */
/*  License along with this library; if not, write to the Free Software       */
/*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  */
/*  USA.                                                                      */
/******************************************************************************/

#ifndef __CHECK_LIBTRACE_H__
#define __CHECK_LIBTRACE_H__

#include <check.h>

void chktrace_context_tests(Suite *suite);
void chktrace_module_tests(Suite *suite);
void chktrace_flag_tests(Suite *suite);
void chktrace_format_tests(Suite *suite);
void chktrace_default_tests(Suite *suite);
void chktrace_target_tests(Suite *suite);
int capture_fd(int fd, int *pipe_fd, int *saved_fd);
int release_fd(int fd, int *pipe_fd, int saved_fd);

SRunner *chktrace_srunner(void);

#endif /* __CHECK_LIBTRACE_H__ */
