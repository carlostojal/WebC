
/*
 * WebC
 *
 * include/server.h
 *
 * Sockets creation and connection handle.
 *
 * 
 * Carlos Tojal (carlostojal)
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "utils.h"

#define CONNECTION_QUEUE_LEN 5

// this is the function the user must implement
Response on_request(Request request);

void run_server(int port) {

  int serverfd; // server socket file descriptor
  int clientfd; // client socket file descriptor
  struct sockaddr_in serveraddr; // server address
  struct sockaddr_in clientaddr; // client address

  FILE *stream; // client socket file stream

  printf("Server starting...\n\n");

  // create socket
  serverfd = socket(AF_INET, SOCK_STREAM, 0); // 0 is the protocol value for Internet Protocol (IP)
  if(serverfd < 0)
    error("Error opening socket");
  
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
    error("Error on binding");

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
      error("Error on accept");

    // structure that the programmer will get with request data
    Request request;

    // structure that the programmer will send
    Response response;
    
    // open client socket stream
    if((stream = fdopen(clientfd, "r+")) == NULL)
      error("Error on socket open");

    request = extract_request_data(stream);

    // user uses POST, PUT, or any other method
    if(strcasecmp("GET", request.method)) {
      client_error(stream, 405, "Method not allowed");
      continue;
    }

    // programmer does what he wants with the request data and sends a response
    response = on_request(request);

    fflush(stream);

    // response is sent
    fprintf(stream, "HTTP/1.1 200 OK\n");
    fprintf(stream, "Server: WebC by carlostojal\n");
    fprintf(stream, "Content-Type: %s\n", response.content_type);
    fprintf(stream, "\r\n");
    fflush(stream);
    fprintf(stream, "%s", response.body);

    printf("Sent response.\n");

    // clean up
    fclose(stream);
    close(clientfd);

  }
}