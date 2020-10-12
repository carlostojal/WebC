
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

#if defined(_WIN32) || defined(WIN32) // windows systems
#include <winsock2.h>
#else // unix systems
#include <sys/socket.h>
#include <netinet/in.h>
#endif

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

  #if defined(_WIN32) || defined(WIN32)
  WSADATA wsa;
  printf("Initializing Winsock...\n");
  if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
    error("Error initializing Winsock.");
  #endif

  // create socket
  serverfd = socket(AF_INET, SOCK_STREAM, 0); // 0 is the protocol value for Internet Protocol (IP)
  if(serverfd < 0)
    error("Error opening socket");

  printf("Socket created.\n");


  // prevent "port already in use" errors
  int optval = 1;
  setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, (const void *) &optval, sizeof(int));


  // bind port to socket
  // bzero((char *) &serveraddr, sizeof(serveraddr));
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

    int readed, bytes_loaded = 0;
    char *buffer = (char *) malloc(BUFFER_SIZE);

    recv(clientfd, buffer, BUFFER_SIZE, 0);

    // printf("%s", buffer);

    request = extract_request_data(buffer);

    char raw_response[16348];

    response = on_request(request);
    
    sprintf(raw_response, "HTTP/1.1 %d %s\nServer: WebC\nContent-Type: %s\n\r\n%s", response.status, response.status_description, response.content_type, response.body);

    send(clientfd, raw_response, strlen(raw_response), 0);

    // clean up
    #if defined(__WIN32) || defined(WIN32)
    closesocket(clientfd);
    #else
    close(clientfd);
    #endif

    free(buffer);
  }
}
