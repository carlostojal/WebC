
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


#define MAX_ARG_NAME_LEN 1024
#define MAX_ARG_VALUE_LEN 1024
#define MAX_ARGS 50

#define MAX_HEADER_NAME_LEN 1024
#define MAX_HEADER_VALUE_LEN 1024
#define MAX_HEADERS 50

#define MAX_METHOD_LEN 4
#define MAX_STATUS_DESCRIPTION_LEN 25
#define MAX_CONTENT_TYPE_LEN 15
#define MAX_BODY_LEN 8192
#define MAX_ROUTE_LEN 1024
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
} Request;

// Response data structure
typedef struct Response {
  int status;
  char status_description[MAX_STATUS_DESCRIPTION_LEN];
  char content_type[MAX_CONTENT_TYPE_LEN];
  char body[MAX_BODY_LEN];
} Response;