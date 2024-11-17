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

#ifndef _CHTTPS_MACROS_H
#define _CHTTPS_MACROS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG  

#include <stdio.h>      /* printf */
#include <stdlib.h>     /* malloc & free */

#define malloc(...) \
  malloc(__VA_ARGS__); \
  printf("Malloc at file: %s, line: %d\n", __FILE__, __LINE__)

#define free(...) \
  free(__VA_ARGS__); \
  printf("Free at file: %s, line: %d\n", __FILE__, __LINE__)

#endif
  
#define chttps_handle_error(err) \
  do { fprintf(stderr, "Error: %s\n", chttps_err_str(-err)); \
    exit(EXIT_FAILURE); } while (0)

#define chttps_handle_error_msg(err) \
  do { fprintf(stderr, "Error: %s\n", err); \
    exit(EXIT_FAILURE); } while (0)

#define chttps_thread_handle_error(error) \
  do { \
    char message[255]; \
    sscanf(message, "Error in execution thread: %s\n", chttps_err_str(-error)); \
    chttps_err(message, &(info.server->conf));			\
    pthread_exit((void*)error); } while (0)
  
#define chttps_thread_handle_error_msg(...) \
  do { \
    char err[255]; \
    sscanf(err, "Error in execution thread: %s\n", __VA_ARGS__);		\
    chttps_err(err, &(info.server->conf));			\
    pthread_exit(NULL); } while (0)

#ifdef __cplusplus
}
#endif

#endif
