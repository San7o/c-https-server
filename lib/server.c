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

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>  /* byte ordering   */
#include <unistd.h>     /* close           */
#include <stdlib.h>     /* malloc and free */

#include <chttps/server.h>
#include <chttps/macros.h>

CHTTPS_ERROR chttps_server_init(chttps_config *conf,
				chttps_server *server)
{
  if (conf == NULL)
    return -CHTTPS_NULL_CONF_ERROR;

  server->waiting_queue_size = conf->waiting_queue_size;
  server->sfd = socket(AF_INET, SOCK_STREAM, 0); /* tcp socket */
  if (server->sfd == 0) 
    return -CHTTPS_SOCKET_ERROR;

  struct in_addr addr;
  if (inet_aton(conf->listen_ip, &addr) == 0)  /* From char* IP to addr */
    return -CHTTPS_IP_CONVERSION_ERROR;
  struct sockaddr_in saddr = {
    /* sa_family_t    */ .sin_family = AF_INET,
    /* in_port_t      */ .sin_port   = conf->port,
    /* struct in_addr */ .sin_addr   = addr
  };

  if (bind(server->sfd, (struct sockaddr*) &saddr, sizeof(saddr)) == -1)
    return -CHTTPS_BIND_ERROR;

  return CHTTPS_NO_ERROR;
}

CHTTPS_ERROR chttps_server_listen(chttps_server *server)
{
  if (server == NULL)
    return -CHTTPS_SERVER_IS_NULL_ERROR;
    
  if (listen(server->sfd, server->waiting_queue_size) == -1)
    return -CHTTPS_LISTEN_ERROR;

  /*
  while(1)
    {
  */
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);
  int cfd = accept(server->sfd, (struct sockaddr *) &client_addr,
		   &client_addr_len);
  if (cfd == -1)
    return -CHTTPS_ACCEPT_CONNECTION_ERROR;

  printf("Hello World!\n");
  /*
    }
  */

  return CHTTPS_NO_ERROR;
}

CHTTPS_ERROR chttps_server_close(chttps_server *server)
{
  if (server == NULL)
    return -CHTTPS_SERVER_IS_NULL_ERROR;

  if (close(server->sfd) == -1)
    return -CHTTPS_CLOSE_SERVER_SOCKET_ERROR;

  return CHTTPS_NO_ERROR;
}
