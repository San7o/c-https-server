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

#ifndef _CHTTPS_LOGGER_H
#define _CHTTPS_LOGGER_H

#include <chttps/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Log a message with the specified level
 * An optional config file can be passed for advanced
 * usage.
 */
void chttps_log(chttps_log_level level,
		char *message,
		chttps_config *conf);

/*
 * The following functions are wrappers around
 * chttps_log, specifying the log level to have
 * a shorter signature.
 */
void chttps_debug(char *message,
		  chttps_config *conf);

void chttps_info(char *message,
		 chttps_config *conf);

void chttps_warn(char *message,
		 chttps_config *conf);

void chttps_err(char *message,
		chttps_config *conf);

void chttps_out(char *message,
	        chttps_config *conf);

#ifdef __cplusplus
}
#endif

#endif
