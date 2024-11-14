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

#ifndef _CHTTPS_TYPES_H
#define _CHTTPS_TYPES_H

#include <netinet/in.h>  /* in_port_t */

#ifdef __cplusplus
extern "C" {
#endif

typedef enum 
{
  CHTTPS_NO_ERROR = 0,
  CHTTPS_NULL_CONF_ERROR,
  CHTTPS_SERVER_IS_NULL_ERROR,
  CHTTPS_IP_CONVERSION_ERROR,
  CHTTPS_SOCKET_ERROR,
  CHTTPS_BIND_ERROR,
  CHTTPS_LISTEN_ERROR,
  CHTTPS_ACCEPT_CONNECTION_ERROR,
  CHTTPS_CLOSE_SERVER_SOCKET_ERROR,
  _CHTTPS_ERROR_MAX
} CHTTPS_ERROR;

typedef in_port_t chttps_port_t; 

typedef struct
{
  const char *listen_ip;  /* The IP to listen from */
  chttps_port_t port;     /* Port to listen from   */
  int waiting_queue_size; /* Waiting queue for new connections */
} chttps_config;

typedef struct
{
  int sfd;
  int waiting_queue_size;
} chttps_server;

#ifdef __cplusplus
}
#endif

#endif
