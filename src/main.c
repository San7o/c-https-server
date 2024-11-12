#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>  /* byte ordering */
#include <stdlib.h>     /* exit  */
#include <unistd.h>     /* close */

#define HOST_IP "0.0.0.0"
#define PORT 6969
#define LISTEN_BACKLOG 50    /* max connections buffer */
#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while (0)

// Listen to TCP socket
// - TODO: Listen to TLS socket
// Spawn a new thread to handle connection
// Send http payload
int main(void)
{

  int sfd = socket(AF_INET, SOCK_STREAM, 0); /* tcp socket */
  if (sfd == 0) 
    handle_error("socket");

  struct in_addr addr;
  if (inet_aton(HOST_IP, &addr) == 0)  /* From char* IP to addr */
    handle_error("inet_aton");
  struct sockaddr_in saddr = {
    /* sa_family_t    */ .sin_family = AF_INET,
    /* in_port_t      */ .sin_port   = PORT,
    /* struct in_addr */ .sin_addr   = addr
  };

  if (bind(sfd, (struct sockaddr*) &saddr, sizeof(saddr)) == -1)
    handle_error("bind");

  if (listen(sfd, LISTEN_BACKLOG) == -1)
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
		 
  return 0;
}
