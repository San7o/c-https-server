/* 
 * MIT License
 * 
 * Copyright (c) 2024 Giovanni Santini
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _CHTTPS_DAEMON_ARGS_H
#define _CHTTPS_DAEMON_ARGS_H

#include <string.h>         /* strcmp        */
#include <stdio.h>          /* printing      */

#include <chttps/types.h>
#include <chttps/util.h>

#ifdef __cplusplus
extern "C" {
#endif

void chttps_print_banner(void);

void chttps_print_help(void);

/**
 * Takes an initialized chttps_config and modifies it based
 * on the arguments.
 * Accepted args:
 * - --port,-p
 * - --ip,-i
 * - --log-level,-ll
 * - --no-banner,-nb
 * - --help,-h
 */
chttps_error chttps_parse_args(int argc, char** argv,
			       chttps_config *conf);

#ifdef __cplusplus
}
#endif

#endif
