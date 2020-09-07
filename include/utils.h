#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
#define MAX_SUBSTR_SIZE 250

#ifndef LOG_REQUEST_DATA
#define LOG_REQUEST_DATA 1
#endif

void error(char *msg) {
  perror(msg);
  exit(1);
}

void print_headers_data(Header *headers) {
  int i;
  for(i = 0; i < MAX_HEADERS && strcmp(headers[i].name, "\r\n"); i++)
    printf("%s: %s", headers[i].name, headers[i].value);
}

void print_request_data(Request request) {
  printf("Method: %s\n", request.method);
  printf("Route: %s\n", request.route);
  printf("HTTP Version: %s\n", request.http_version);
  printf("\nHeaders:\n");
  print_headers_data(request.headers);
  printf("\n");
}

char *substr(char *str, int position, int size) {

  char *sub = malloc(size);

  int i;
  for(i = 0; i < size; i++)
    sub[i] = str[i + position + 2];
  
  return sub;
}

Request extract_request_data(FILE *stream) {

  char *buffer = malloc(BUFFER_SIZE); // string buffer

  Request request;

  // get HTTP request line
  fgets(buffer, BUFFER_SIZE, stream);
  sscanf(buffer, "%s %s %s", request.method, request.route, request.http_version);

  char *buffer_copy = malloc(BUFFER_SIZE);

  int i = 0;

  // read the HTTP headers
  fgets(buffer, BUFFER_SIZE, stream);
  printf("%s", buffer);
  strcpy(buffer_copy, buffer);
  strcpy(request.headers[i].name, strtok(buffer_copy, ": "));
  strcpy(request.headers[i].value, substr(buffer, strlen(buffer_copy), MAX_SUBSTR_SIZE));
  i++;
  while(strcmp(buffer, "\r\n") && i < MAX_HEADERS) {
    fgets(buffer, BUFFER_SIZE, stream);
    strcpy(buffer_copy, buffer);
    strcpy(request.headers[i].name, strtok(buffer_copy, ": "));
    strcpy(request.headers[i].value, substr(buffer, strlen(buffer_copy), MAX_SUBSTR_SIZE));
    i++;
  }

  if(LOG_REQUEST_DATA)
    print_request_data(request);

  free(buffer);
  free(buffer_copy);

  return request;
}