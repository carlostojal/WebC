
/*
 * WebC
 *
 * src/main.c
 *
 * This is an example implementation
 * of the library. 
 *
 * 
 * Carlos Tojal (carlostojal)
 *
 */

#include <stdio.h>
#include <string.h>

#define PORT 8081
#define LOG_REQUEST_DATA 1 // 1 - logs each request and respective headers; 0 - doesn't log

#include "../include/webc.h"

int main() {

  // simply use this line to start the server
  run_server(PORT);

  return 0;
}

// this function is called when the server gets a request
// you should returns a "Response" type variable.
Response on_request(Request request) {

  Response response;

  printf("On request\n");

  strcpy(response.content_type, "text/html");

  int found = 0;

  int i;
  for(i = 0; i < request.n_headers; i++) {
    if(strcmp(request.headers[i].name, "User-Agent") == 0) {
      found = 1;
      sprintf(response.body, "<html><body><h1>You requested \"%s\"</h1><h2>You are using %s</h2></body>", request.route, request.headers[i].value);
    }
  }

  return response;
}