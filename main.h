<<<<<<< HEAD
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

/* Function prototypes */
uv_tcp_t* create_server(uv_loop_t* loop);
void on_connect(uv_stream_t *server, int status);
void on_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf);
void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
void on_close(uv_handle_t *handle);
void extract_http_request_path(const char *url, char *path, size_t size);
#endif
