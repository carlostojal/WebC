#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "utils.h"

#define CONNECTION_QUEUE_LEN 5

void start_server(Config config) {

  int serverfd; // server socket file descriptor
  struct sockaddr_in serveraddr;

  printf("Server starting...\n\n");

  // create socket
  serverfd = socket(AF_INET, SOCK_STREAM, 0); // 0 is the protocol value for Internet Protocol (IP)
  if(serverfd < 0)
    error("ERROR opening socket");
  
  printf("Socket created.\n");


  // prevent "port already in use" errors
  int optval = 1;
  setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, (const void *) &optval, sizeof(int));


  // bind port to socket
  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short) config.port);
  if(bind(serverfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
    error("ERROR on binding");

  printf("Port binded to socket.\n");


  // we are now ready to accept connection requests
  if(listen(serverfd, CONNECTION_QUEUE_LEN) < 0) // the second parameter is the number of requests that can be in queue
    error("ERROR on listen");

  printf("Ready to listen.\n");

}