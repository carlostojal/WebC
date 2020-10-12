
/*
 * WebC
 *
 * include/utils.h
 *
 * Various functions to parse / create data, for example.
 *
 * 
 * Carlos Tojal (carlostojal)
 *
 */

#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 8192
#define MAX_SUBSTR_SIZE 250

#ifndef LOG_REQUEST_DATA
#define LOG_REQUEST_DATA 1
#endif

void error(char *msg) {
  perror(msg);
  exit(1);
}

void client_error(FILE *stream, int status, char *msg) {
  fprintf(stream, "HTTP/1.1 %d %s\n", status, msg);
  fprintf(stream, "Content-Type: text/html\n");
  fprintf(stream, "Server: WebC by carlostojal\n");
  fprintf(stream, "\r\n");
  fprintf(stream, "<html><title>WebC Error</title>");
  fprintf(stream, "<body>\n");
  fprintf(stream, "%d: %s\n", status, msg);
  fprintf(stream, "<hr><em>WebC by carlostojal</em>\n");
}

void print_headers_data(Header *headers) {
  int i;
  for(i = 0; i < MAX_HEADERS && strcmp(headers[i].name, "\r\n") && strcmp(headers[i].name, ""); i++)
    printf("%s: %s", headers[i].name, headers[i].value);
}

void print_request_data(Request request) {
  printf("Method: %s\n", request.method);
  printf("Route: %s\n", request.route);
  printf("HTTP Version: %s\n", request.http_version);
  printf("\n");
}

char *substr(char *str, int position, int size) {

  char *sub = malloc(size);

  int i;
  for(i = 0; i < size; i++)
    sub[i] = str[i + position + 2];
  
  return sub;
}

/* TODO */
Arg *extract_get_args(char *route) {
  
  Arg *args = malloc(MAX_ARGS * sizeof(Arg));
  char *buffer = malloc(BUFFER_SIZE);
  char *aux = malloc(BUFFER_SIZE);

  buffer = strdup(route);
  aux = strdup(buffer);
  strtok(aux, "?");

  buffer = substr(buffer, strlen(aux), MAX_SUBSTR_SIZE);

  return args;
}

int extract_headers(char *request, Header *headers) {

  char *buffer = (char *) malloc(BUFFER_SIZE);
  char *buffer_copy = (char *) malloc(BUFFER_SIZE);

  printf("Extracting request headers...\n");

  int i = 0;

  strcpy(buffer_copy, buffer);
  strcpy(headers[i].name, strtok(buffer_copy, ": "));
  strcpy(headers[i].value, substr(buffer, strlen(buffer_copy), MAX_SUBSTR_SIZE));
  i++;
  while(strcmp(buffer, "\r\n") && i < MAX_HEADERS) {
    strcpy(buffer_copy, buffer);
    strcpy(headers[i].name, strtok(buffer_copy, ": "));
    strcpy(headers[i].value, substr(buffer, strlen(buffer_copy), MAX_SUBSTR_SIZE));
    i++;
  }

  free(buffer);
  free(buffer_copy);
  
  printf("Done.\n");

  return i-1;
}

Request extract_request_data(char *raw_request) {

  char *buffer = malloc(BUFFER_SIZE); // string buffer

  memcpy(buffer, raw_request, BUFFER_SIZE);

  Request request;

  sscanf(buffer, "%s %s %s", request.method, request.route, request.http_version);

  if(LOG_REQUEST_DATA)
    print_request_data(request);

  free(buffer);

  return request;
}