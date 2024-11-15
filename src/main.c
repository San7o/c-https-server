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

#include <chttps/chttps.h>  /* Amalgamated header */
#include <stdlib.h>         /* exit  */
#include <stdio.h>

#define handle_error(err) \
  do { fprintf(stderr, "Error: %s\n", chttps_err_str(-err)); \
    exit(EXIT_FAILURE); } while (0)

/*
 * Deamon main
 */
int main(void)
{
  chttps_error err;
  chttps_server server = {};
  err = chttps_server_init(&server, NULL);
  if (err != CHTTPS_NO_ERROR)
    handle_error(err);

  /*
  err = chttps_server_listen(&server);
  if (err != CHTTPS_NO_ERROR)
    handle_error(err);
  */

  chttps_server_close(&server);
  if (err != CHTTPS_NO_ERROR)
    handle_error(err);

  return 0;
}
