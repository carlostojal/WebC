
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

#define MAX_PATH_LEN 50
#define MAX_CONTENT_LEN 1024
#define MAX_FILETYPE_LEN 15
#define MAX_ROUTES 30

#define MAX_ARG_NAME_LEN 1024
#define MAX_ARG_VALUE_LEN 1024
#define MAX_ARGS 15

#define MAX_HEADER_NAME_LEN 1024
#define MAX_HEADER_VALUE_LEN 1024
#define MAX_HEADERS 20

#define MAX_METHOD_LEN 4
#define MAX_ROUTE_LEN 20
#define MAX_HTTP_VERSION_LEN 15

// Arguments passed in URL
typedef struct Arg {
  char name[MAX_ARG_NAME_LEN];
  char value[MAX_ARG_VALUE_LEN];
} Arg;

// Request and response header data structure
typedef struct Header {
  char name[MAX_HEADER_NAME_LEN];
  char value[MAX_HEADER_VALUE_LEN];
} Header;

// Request data structure
typedef struct Request {
  char method[MAX_METHOD_LEN];
  char route[MAX_ROUTE_LEN];
  char http_version[MAX_HTTP_VERSION_LEN];
  Arg args[MAX_ARGS];
  Header headers[MAX_HEADERS];
} Request;

// Response data structure
typedef struct Response {
  Header headers[MAX_HEADERS];
  char content[MAX_CONTENT_LEN];
} Response;