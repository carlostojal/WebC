
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
#define LOG_REQUEST_DATA 1

#include "../include/webc.h"

int main() {

  start_server(PORT);

  return 0;
}

Response on_request(Request request) {

  Response response;

  printf("On request\n");

  return response;
}