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

#include <sys/socket.h> /* UNIX sockets    */
#include <arpa/inet.h>  /* byte ordering   */
#include <unistd.h>     /* close           */
#include <stdlib.h>     /* malloc and free */
#include <string.h>     /* memcpy          */

#include <chttps/server.h>
#include <chttps/macros.h>
#include <chttps/logger.h>
#include <chttps/util.h>

chttps_error chttps_server_init(chttps_server *server,
                                chttps_config *conf)
{
  if (server == NULL)
    return -CHTTPS_SERVER_IS_NULL_ERROR;
    
  if (conf == NULL)
  {
    chttps_config default_config = chttps_config_default();
    memcpy(&server->conf, &default_config, sizeof(chttps_config));
  }
  else
  {
    memcpy(&server->conf, conf, sizeof(chttps_config));
  }
   
  server->sfd = socket(AF_INET, SOCK_STREAM, 0); /* tcp socket */
  if (server->sfd == 0) 
    return -CHTTPS_SOCKET_ERROR;

  struct in_addr addr;
  if (inet_aton(server->conf.listen_ip, &addr) == 0)  /* From char* IP to addr */
    return -CHTTPS_IP_CONVERSION_ERROR;
  struct sockaddr_in saddr = {
    /* sa_family_t    */ .sin_family = AF_INET,
    /* in_port_t      */ .sin_port   = server->conf.port,
    /* struct in_addr */ .sin_addr   = addr
  };

  if (bind(server->sfd, (struct sockaddr*) &saddr, sizeof(saddr)) == -1)
    return -CHTTPS_BIND_ERROR;

  chttps_info("Server started", &server->conf);
  return CHTTPS_NO_ERROR;
}

chttps_error chttps_server_listen(chttps_server *server,
				  chttps_client **client)
{
  if (server == NULL)
    return -CHTTPS_SERVER_IS_NULL_ERROR;
  if (client == NULL)
    return -CHTTPS_CLIENT_IS_NULL_ERROR;
    
  if (listen(server->sfd, server->conf.waiting_queue_size) == -1)
    return -CHTTPS_LISTEN_ERROR;

  *client = malloc(sizeof(chttps_client));
  socklen_t client_addr_len = sizeof((*client)->addr);
  (*client)->cfd = accept(server->sfd, (struct sockaddr *) &(*client)->addr,
		   &client_addr_len);
  if ((*client)->cfd == -1)
    return -CHTTPS_ACCEPT_CONNECTION_ERROR;
  
  chttps_info("New client connected", &server->conf);
  return CHTTPS_NO_ERROR;
}

chttps_error chttps_server_close(chttps_server *server)
{
  if (server == NULL)
    return -CHTTPS_SERVER_IS_NULL_ERROR;

  if (close(server->sfd) == -1)
    return -CHTTPS_CLOSE_SERVER_SOCKET_ERROR;

  chttps_info("Server closed", &server->conf);
  return CHTTPS_NO_ERROR;
}

pthread_mutex_t *chttps_get_stop_mutex()
{
  static pthread_mutex_t stop_mutex;
  return &stop_mutex;
}

bool *chttps_get_stop()
{
  static bool stop = false;
  return &stop;
}

void chttps_set_stop(bool stop)
{
  pthread_mutex_lock(chttps_get_stop_mutex());
  bool *stop_ptr = chttps_get_stop();
  *stop_ptr = stop;
  pthread_mutex_unlock(chttps_get_stop_mutex());
}
