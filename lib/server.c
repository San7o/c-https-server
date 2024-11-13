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
#include <arpa/inet.h>  /* byte ordering */
#include <unistd.h>     /* close */

#include <chttps/server.h>
#include <chttps/macros.h>

// TODO: real error handling
// TODO: __cpluplus
CHTTPS_ERROR chttps_start_server(const char* listener_ip,
			  chttps_port_t port,
			  int waiting_queue_size)
{
  int sfd = socket(AF_INET, SOCK_STREAM, 0); /* tcp socket */
  if (sfd == 0) 
    handle_error("socket");

  struct in_addr addr;
  if (inet_aton(listener_ip, &addr) == 0)  /* From char* IP to addr */
    handle_error("inet_aton");
  struct sockaddr_in saddr = {
    /* sa_family_t    */ .sin_family = AF_INET,
    /* in_port_t      */ .sin_port   = port,
    /* struct in_addr */ .sin_addr   = addr
  };

  if (bind(sfd, (struct sockaddr*) &saddr, sizeof(saddr)) == -1)
    handle_error("bind");

  if (listen(sfd, waiting_queue_size) == -1)
    handle_error("listen");

  /*
  while(1)
    {
  */
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);
  int cfd = accept(sfd, (struct sockaddr *) &client_addr,
		   &client_addr_len);
  if (cfd == -1)
    handle_error("accept");

  printf("Hello World!\n");
  /*
    }
  */

  if (close(sfd) == -1)
    handle_error("close");

  return CHTTPS_NO_ERROR;
}
