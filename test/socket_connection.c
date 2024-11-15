#include <chttps/chttps.h>

#include <pthread.h>
#include <stdlib.h>     /* exit  */
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>  /* byte ordering   */
#include <string.h>     /* strcmp */
#include <unistd.h>     /* close */

/* Default values */
#define LISTEN_IP "0.0.0.0"
#define PORT 6969

#define chttps_handle_error(err) \
  do { fprintf(stderr, "Error in test socket_connection: %s\n", chttps_err_str(-err)); \
    exit(EXIT_FAILURE); } while (0)
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
  char received_message[6];
  ssize_t received = recv(client->cfd, received_message, 6, 0);
  if (received != 6)
    handle_error("Server Received wrong message size");
  if (strcmp(received_message, "Hello"))
    handle_error("Server Wrong message received");
  
  /* Send a resposnse */
  const char *message = "OK\0";
  ssize_t sent = send(client->cfd, message, sizeof(message), 0);
  if (sent != sizeof(message))
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
    /* in_port_t      */ .sin_port   = PORT,
    /* struct in_addr */ .sin_addr   = addr
  };
  sfd = socket(AF_INET, SOCK_STREAM, 0); /* tcp socket */
  if (sfd == 0) 
    handle_error("Client socket");
  err = connect(sfd, (struct sockaddr*) &saddr, sizeof(saddr));
  if (err != 0)
    handle_error("Client connect");

  /* Send a message */
  const char *message = "Hello\0";
  ssize_t sent = send(sfd, message, sizeof(message), 0);
  if (sent != sizeof(message))
    handle_error("Client send");

  /* Get response */
  char received_message[3];
  ssize_t received = recv(sfd, received_message, 3, 0);
  if (received != 3)
    handle_error("Client Received wrong message size");
  if (strcmp(received_message, "OK"))
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
  
  send_message();

  /* Close thread */
  err = pthread_join(tid, NULL);
  if (err)
    handle_error("pthread_join");
  
  return 0;
}
