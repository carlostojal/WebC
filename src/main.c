
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

#include "../include/webc.h"

int main() {
  
  // create config
  Config config;

  config.port = 8081;
  config.n_routes = 2; // in this example 2 routes are created

  // define routes config
  Route routes[config.n_routes];
  
  strcpy(routes[0].name, "/");
  strcpy(routes[0].filepath, "../htdocs/index.html");
  strcpy(routes[0].content, "none"); // content is not defined because a file path is provided
  strcpy(routes[0].filetype, "none"); // filetype will be detected

  strcpy(routes[1].name, "/hello");
  strcpy(routes[1].filepath, "none");
  strcpy(routes[1].content, "Hello"); // content is not defined because a file path is provided
  strcpy(routes[1].filetype, "text/plain"); // filetype is hardcoded because the content is also hardcoded

  start_server(config);

  return 0;
}
