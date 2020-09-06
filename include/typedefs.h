
/*
 * WebC
 *
 * include/typedefs.h
 *
 * Type definitions used to create
 * the web app.
 *
 * 
 * Carlos Tojal (carlostojal)
 *
 */

#define MAX_ROUTE_LEN 20
#define MAX_PATH_LEN 50
#define MAX_CONTENT_LEN 1024
#define MAX_FILETYPE_LEN 15
#define MAX_ROUTES 30


// Defines each route the app will handle
typedef struct Route {
  char name[MAX_ROUTE_LEN]; // ex: "/"
  char filepath[MAX_PATH_LEN]; // path of the file to serve
  char content[MAX_CONTENT_LEN]; // can also be defined hardcoded content
  char filetype[MAX_FILETYPE_LEN]; // in case the content is hardcoded, you should provide this (text/html, text/plain, etc.)
} Route;

typedef struct Config {
  int port; // port to serve the app on
  int n_routes; // number of routes
  Route routes[MAX_ROUTES];
} Config;
