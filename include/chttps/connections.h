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
#include <string.h>         /* strcpy        */
#include <stdio.h>          /* printing      */
#include <pthread.h>        /* pthread_setcancelstate */
#include <unistd.h>         /* close         */

#include <chttps/types.h>
#include <chttps/logger.h>
#include <chttps/parser.h>
#include <chttps/route.h>
#include <chttps/router.h>
#include <chttps/macros.h>
#include <chttps/server.h>
#include <chttps/util.h>

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
// TODO: Make connection persistant
static void *chttps_connection_thread(void* args)
{
  chttps_thread_info *info_ptr = (chttps_thread_info *) args;
  chttps_thread_info info = *info_ptr;
  free(info_ptr);
  chttps_debug("Spawned connection thread", &(info.server->conf));
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

  /* Receive message */
  chttps_error err;
  printf("Thread: Ready to receive message...\n");
  const size_t MAX_MESSAGE_SIZE = 1024;
  char received_message[MAX_MESSAGE_SIZE];
  strcpy(received_message, "");
  recv(info.client->cfd, received_message, MAX_MESSAGE_SIZE, 0);
  chttps_debug("Thread: Done Reading\n", &(info.server->conf));

  /* Parse the request */
  chttps_debug("Thread: parsing request...\n", &(info.server->conf));
  chttps_request *req;
  err = chttps_parse_request(received_message, &req);
  if (err != CHTTPS_NO_ERROR)
    chttps_thread_handle_error(err);

  /* Match the route */
  chttps_route *match;
  err = chttps_router_match(&(info.server->router), req->header.uri, &match);
  if (err != CHTTPS_NO_ERROR)
    chttps_thread_handle_error(err);

  /* Execute the callback */
  chttps_debug("Thread: executing callback...\n", &(info.server->conf));
  char *out;
  switch(req->header.method)
    {
    case CHTTPS_GET:
      err = chttps_route_get(match, req, &out);
      if (err != CHTTPS_NO_ERROR)
	chttps_thread_handle_error(err);
      break;
    case CHTTPS_HEAD:
      err = chttps_route_head(match, req, &out);
      if (err != CHTTPS_NO_ERROR)
	chttps_thread_handle_error(err);
      break;
    case CHTTPS_POST:
      err = chttps_route_head(match, req, &out);
      if (err != CHTTPS_NO_ERROR)
	chttps_thread_handle_error(err);
      break;
    default:
      // Should never happen
      break;
    }

  /* Create a response */
  chttps_debug("Thread: setting up response...\n", &(info.server->conf));
  chttps_response_header res_h = {
    .status_code = 200,
  };
  strcpy(res_h.version, "1.0");
  chttps_response res = {
    .header = res_h,
    .body_len = strlen(out),
    .body = out,
  };
  chttps_debug("Thread: creating response...\n", &(info.server->conf));
  char* res_str;
  err = chttps_create_response(&res, &res_str);
  if (err != CHTTPS_NO_ERROR)
    chttps_thread_handle_error(err);
  
  /* Send the resposnse */
  ssize_t sent = send(info.client->cfd, res_str, strlen(res_str), 0);
  if (sent != (ssize_t) strlen(res_str))
    chttps_thread_handle_error_msg("Thread send");
  
  /* Cleanup */
  if (err != CHTTPS_NO_ERROR)
    chttps_thread_handle_error(err);
  free(res_str);
  free(req);
  free(out);
  SSL_shutdown(info.client->ssl);
  SSL_free(info.client->ssl);
  close(info.client->cfd);
  chttps_remove_connection(&(info.server->connections), info.client);
  return NULL;
}

#ifdef __cplusplus
}
#endif

#endif
