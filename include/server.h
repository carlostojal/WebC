#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "utils.h"

#define CONNECTION_QUEUE_LEN 5

// this is the function the user must implement
Response on_request(Request request);

void start_server(int port) {

  int serverfd; // server socket file descriptor
  int clientfd; // client socket file descriptor
  struct sockaddr_in serveraddr; // server address
  struct sockaddr_in clientaddr; // client address

  FILE *stream; // client socket file stream

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
  serveraddr.sin_port = htons((unsigned short) port);
  if(bind(serverfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
    error("ERROR on binding");

  printf("Port binded to socket.\n");


  // we are now ready to accept connection requests
  if(listen(serverfd, CONNECTION_QUEUE_LEN) < 0) // the second parameter is the number of requests that can be in queue
    error("ERROR on listen");

  printf("\nServer running on port %d.\n", port);

  
  int clientlen = sizeof(clientaddr);

  // loop where requests are handled and responses sent
  while(1) {
    
    // wait for a connection request
    clientfd = accept(serverfd, (struct sockaddr *) &clientaddr, &clientlen);
    if(clientfd < 0) 
      error("ERROR on accept");

    // structure that the programmer will get with request data
    Request request;

    // structure that the programmer will send
    Response response;
    
    // open client socket stream
    if((stream = fdopen(clientfd, "r+")) == NULL)
      error("ERROR on fdopen");

    request = extract_request_data(stream);

    // programmer does what he wants with the request data and sends a response
    response = on_request(request);
  }
}