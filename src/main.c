
/*
 * WebC
 *
 * src/main.c
 *
 * This is an example implementation
 * of the library.
 * In this case, where "/" is requested
 * the current system time is write to logs,
 * and that logs appear to the client as a list.
 *
 *
 * Carlos Tojal (carlostojal)
 *
 */

#include <stdio.h>
#include <time.h>
#include <string.h>

#define PORT 8081
#define LOG_REQUEST_DATA 0 // 1 - logs each request and respective headers; 0 - doesn't log

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

  printf("%s\n", request.route);

  // client requested "/"
  if(strcmp(request.route, "/") == 0) {

    // get time
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    
    // write time to logs
    FILE *fp = fopen("./htdocs/logs.txt", "a");
    fprintf(fp, "%s", asctime(timeinfo));
    fclose(fp);

    const int buffer_len = 1024;
    char buffer[buffer_len];

    strcpy(response.body, "<html><body><h1>Access logs:</h1><ul>");

    // read time from logs and create response body
    fp = fopen("./htdocs/logs.txt", "r");
    while(fgets(buffer, buffer_len, fp))
      sprintf(response.body, "%s<li>%s</li>", response.body, buffer);
    fclose(fp);
    strcat(response.body, "</ul></body></html>");

    // set response headers
    strcpy(response.content_type, "text/html");
    response.status = 200;
    strcpy(response.status_description, "OK");
    
    return response;

  } else {

    strcpy(response.content_type, "text/html");
    response.status = 404;
    strcpy(response.status_description, "Not Found");

    strcpy(response.body, "<h1>404 Not Found");

    return response;
  }
}
