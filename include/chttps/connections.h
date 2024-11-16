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

#ifndef _CHTTPS_CONNECTIONS_H
#define _CHTTPS_CONNECTIONS_H

#include <stdlib.h>         /* malloc & free */

#include <chttps/types.h>
#include <chttps/logger.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This structure will be passed to each thread upon creation.
 */
typedef struct
{
  chttps_server* server;
  chttps_client* client;
} chttps_thread_info;

chttps_connections chttps_connections_init(const size_t max_connections);

chttps_error chttps_connections_free(chttps_connections *connections);

chttps_error chttps_add_connection(chttps_server *server,
				   chttps_client *client);

chttps_error chttps_remove_connection(chttps_connections *connections,
				      chttps_client *client);

/**
 * Thread execution starts here for each client.
 */
static void *chttps_connection_thread(void* args)
{
  chttps_thread_info *info_ptr = (chttps_thread_info *) args;
  chttps_thread_info info = *info_ptr;
  free(info_ptr);
  chttps_debug("Spawned connection thread", &(info.server->conf));

  const char *ok_message = "OK\0";
  ssize_t sent = send(info.client->cfd, ok_message, sizeof(ok_message), 0);
  if (sent != sizeof(ok_message))
    chttps_err("Error sending message to client", &(info.server->conf));

  chttps_remove_connection(&(info.server->connections), info.client);
  return NULL;
}

#ifdef __cplusplus
}
#endif

#endif
