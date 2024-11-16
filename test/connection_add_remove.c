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

#include <chttps/chttps.h>

#include <pthread.h>
#include <stdlib.h>     /* exit            */
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>  /* byte ordering   */
#include <string.h>     /* strcmp          */
#include <unistd.h>     /* close, sleep    */

/* Default values */
#define LISTEN_IP "0.0.0.0"
#define PORT 6970
#define LOG_LEVEL CHTTPS_DEBUG

#define chttps_handle_error(err) \
  do { fprintf(stderr, "Error in test connection_add_remove: %s\n", chttps_err_str(-err)); \
    exit(EXIT_FAILURE); } while (0)
#define handle_error(err) \
  do { fprintf(stderr, "Error in test connection_add_remove: %s\n", err); \
    exit(EXIT_FAILURE); } while (0)

int main(void)
{
  chttps_error err;
  chttps_config config = chttps_config_default();
  config.log_level          = LOG_LEVEL;
  config.listen_ip          = LISTEN_IP;
  config.port               = PORT;
  chttps_server my_server = {};
  err = chttps_server_init(&my_server, &config);
  if (err != CHTTPS_NO_ERROR)
    chttps_handle_error(err);

  chttps_client *client = malloc(sizeof(chttps_client));
  err = chttps_add_connection(&my_server, client);
  if (err)
    chttps_handle_error(err);

  /* Check that the connection has been registered */
  if (!my_server.connections.threads[0])
    handle_error("Thread not registered");
  
  /* Cleanup */
  chttps_server_close(&my_server);
  if (err != CHTTPS_NO_ERROR)
    chttps_handle_error(err);
  return 0;
}
