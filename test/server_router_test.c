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
#define PORT 6972

#define chttps_handle_error(err) \
  do { fprintf(stderr, "Error in test server_router_test: %s\n", chttps_err_str(-err)); \
    exit(EXIT_FAILURE); } while (0)
#define handle_error(err) \
  do { fprintf(stderr, "Error in test setver_router_test: %s\n", err); \
    exit(EXIT_FAILURE); } while (0)

chttps_error greet_get_callback(chttps_request *req, char **out)
{
  const char* message = "";
  *out = malloc(sizeof(message));
  strcpy(*out, message);
  return CHTTPS_NO_ERROR;
}

/* Create a test server */
static void *server(void*)
{
  /* Initialize the server */
  chttps_error err;
  chttps_config config = chttps_config_default();
  config.log_level          = CHTTPS_DISABLED;
  config.listen_ip          = LISTEN_IP;
  config.port               = PORT;
  chttps_server my_server = {};
  err = chttps_server_init(&my_server, &config);
  if (err != CHTTPS_NO_ERROR)
    chttps_handle_error(err);

  /* Create a bunch of routes with their callbacks*/
  chttps_route *route1;
  err = chttps_route_create("/api/greet", &route1);
  if (err != CHTTPS_NO_ERROR)
    chttps_handle_error(err);
  err = chttps_route_set_get(route1, greet_get_callback);
  if (err != CHTTPS_NO_ERROR)
    chttps_handle_error(err);

  /* Register the routes */
  err = chttps_router_add(&(my_server.router), route1);
  if (err != CHTTPS_NO_ERROR)
    chttps_handle_error(err);

  /* Listen client connection */
  chttps_client *client;
  err = chttps_server_listen(&my_server, &client);
  if (err != CHTTPS_NO_ERROR)
    chttps_handle_error(err);

  /* Receive message */
  printf("Server: Ready to receive message...\n");
  char received_message[255];
  strcpy(received_message, "");
  const char *expected_message = "GET /api/greet HTTP/1.0\t\n";
  recv(client->cfd, received_message, 255, 0);
  if (strcmp(received_message, expected_message) != 0)
    {
      handle_error("Server: Message received is not the expected one");
    }
  fprintf(stdout, "Server: Done Reading\n");
  fprintf(stdout, "Server: Message Received: %s\n", received_message);

  /* Parse the request */
  fprintf(stdout, "Server: parsing request...\n");
  chttps_request *req;
  err = chttps_parse_request(received_message, &req);
  if (err != CHTTPS_NO_ERROR)
    chttps_handle_error(err);

  /* Match the route */
  chttps_route *match;
  err = chttps_router_match(&(my_server.router), req->header.uri, &match);
  if (err != CHTTPS_NO_ERROR)
    chttps_handle_error(err);

  /* Execute the callback */
  fprintf(stdout, "Server: executing callback...\n");
  char *out;
  if (req->header.method == CHTTPS_GET)
    {
      err = chttps_route_get(match, req, &out);
      if (err != CHTTPS_NO_ERROR)
	chttps_handle_error(err);
    }
  else handle_error("Server: Wrong method in client request");

  /* Create a response */
  fprintf(stdout, "Server: setting up response...\n");
  chttps_response_header res_h = {
    .status_code = 200,
  };
  strcpy(res_h.version, "1.0");
  chttps_response res = {
    .header = res_h,
    .body_len = strlen(out),
    .body = out,
  };
  fprintf(stdout, "Server: creating response...\n");
  char* res_str;
  err = chttps_create_response(&res, &res_str);
  if (err != CHTTPS_NO_ERROR)
    chttps_handle_error(err);
  
  /* Send the resposnse */
  fprintf(stdout, "Server: sending response \"%s\"...\n", res_str);
  ssize_t sent = send(client->cfd, res_str, strlen(res_str), 0);
  if (sent != (ssize_t) strlen(res_str))
    handle_error("Server send");
  fprintf(stdout, "Server: sent %ld bytes\n", sent);
  
  /* Cleanup */
  chttps_server_close(&my_server);
  if (err != CHTTPS_NO_ERROR)
    chttps_handle_error(err);
  free(client);
  free(res_str);
  free(req);
  free(out);
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
  printf("Client: Sending the message...\n");
  char message[30];
  strcpy(message, "GET /api/greet HTTP/1.0\t\n\0");
  ssize_t sent = send(sfd, message, strlen(message), 0);
  printf("Client: Sent %ld bytes\n", sent);

  /* Get response */
  char received_message[255];
  recv(sfd, received_message, 255, 0);
  fprintf(stdout, "Client: Message Received: %s\n", received_message);
  if (strcmp(received_message, "HTTP/1.0 200 OK\t\n"))
    {
      handle_error("Client Wrong message received");
    }
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
  
  sleep(3);
  send_message();

  /* Close thread */
  err = pthread_join(tid, NULL);
  if (err)
    handle_error("pthread_join");
  
  return 0;
}
