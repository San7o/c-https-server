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
#define PORT 6971

#define handle_error(err) \
  do { fprintf(stderr, "Error in test socket_connection: %s\n", err); \
    exit(EXIT_FAILURE); } while (0)

/* Create a test server */
static void *server(void*)
{
  chttps_error err;
  chttps_config config = chttps_config_default();
  config.log_level          = CHTTPS_DISABLED;
  config.listen_ip          = LISTEN_IP;
  config.port               = PORT;
  chttps_server my_server = {};
  err = chttps_server_init(&my_server, &config);
  if (err != CHTTPS_NO_ERROR)
    chttps_handle_error(err);

  chttps_client *client; /* Client file descriptor */
  err = chttps_server_listen(&my_server, &client);
  if (err != CHTTPS_NO_ERROR)
    chttps_handle_error(err);

  /* Wait for a message */
  char received_message[20];
  strcpy(received_message, "");
  ssize_t received = recv(client->cfd, received_message, 20, 0);
  if (received != strlen("Hello folks"))
    {
      fprintf(stderr, "Server: Received %ld bytes\n", received);
      handle_error("Server Received wrong message size");
    }
  if (strncmp("Hello folks", received_message, 11) != 0)
    {
      handle_error("Server Wrong message received");
    }
  
  /* Send a resposnse */
  const char *message = "OK\0";
  ssize_t sent = send(client->cfd, message, strlen(message), 0);
  if (sent != (ssize_t) strlen(message))
    handle_error("Server send");
  
  /* Cleanup */
  chttps_server_close(&my_server);
  if (err != CHTTPS_NO_ERROR)
    chttps_handle_error(err);
  free(client);
  return NULL;
}

void send_message(void)
{
  int sfd;
  int err;
  struct in_addr addr;
  if (inet_aton("127.0.0.1", &addr) == 0)  /* From char* IP to addr */
    handle_error("inet_aton");
  struct sockaddr_in saddr = {
    /* sa_family_t    */ .sin_family = AF_INET,
    /* in_port_t      */ .sin_port   = htons(PORT),
    /* struct in_addr */ .sin_addr   = addr
  };
  sfd = socket(AF_INET, SOCK_STREAM, 0); /* tcp socket */
  if (sfd == 0) 
    handle_error("Client socket");
  err = connect(sfd, (struct sockaddr*) &saddr, sizeof(saddr));
  if (err != 0)
    handle_error("Client connect");

  /* Send a message */
  char message[] = "Hello folks\0";
  ssize_t sent = send(sfd, message, strlen(message), 0);
  if (sent != (ssize_t) strlen(message))
    handle_error("Client send");

  /* Get response */
  char received_message[3];
  ssize_t received = recv(sfd, received_message, 3, 0);
  if (received != 2)
    handle_error("Client Received wrong message size");
  if (strncmp(received_message, "OK", 2))
    handle_error("Client Wrong message received");

  err = close(sfd);
  if (err)
    handle_error("Client close");
  
  return;
}

int main(void)
{
  /* Spawn Server thread */
  int err;
  pthread_attr_t attr;
  err = pthread_attr_init(&attr);
  if (err)
    handle_error("pthread_attr_init");
  pthread_t tid;
  err = pthread_create(&tid, &attr, &server, NULL);
  if (err)
    handle_error("pthread_create");
  
  sleep(2);
  send_message();

  /* Close thread */
  err = pthread_join(tid, NULL);
  if (err)
    handle_error("pthread_join");
  
  return 0;
}
