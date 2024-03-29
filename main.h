#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>
#include "http_parser.h"

#define DEFAULT_BACKLOG 128

/* Structure to hold client file request information */
typedef struct {
  uv_tcp_t *client;
  char path[1024];
} file_request_t;

/* Structure to hold the url and its length*/
typedef struct {
    char* url;
} parser_data_t;

/* Function prototypes */
uv_tcp_t* create_server(uv_loop_t* loop);
void on_connect(uv_stream_t *server, int status);
void on_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf);
void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
void on_close(uv_handle_t *handle);
int on_url(http_parser* parser, const char* at, size_t length);
void extract_http_request_path(file_request_t *file_req);
char* read_file(file_request_t *file_req);
void serve_file(file_request_t *file_req);
void send_response(uv_tcp_t *client, int status_code, uv_buf_t *iov, int iov_count, const char *content_type);
void on_write_end(uv_write_t *write_req, int status);
char* read_file_path(char* file_name);
long get_file_size(char* file_name);
char* get_content_type(const char* path);
#endif
