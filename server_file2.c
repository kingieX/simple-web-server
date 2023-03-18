#include "main.h"
#include <string.h>

void serve_file(file_request_t *file_req) {
    char *content = read_file(file_req);
    if (content == NULL) {
        send_response(file_req->client, 404, NULL, 0, NULL, 0);
    } else {
        uv_buf_t iov = uv_buf_init(content, strlen(content));
        const char *file_ext = strrchr(file_req->path, '.');
        const char *content_type;
        if (file_ext != NULL && strcmp(file_ext, ".html") == 0) {
            content_type = "text/html";
        } else if (file_ext != NULL && strcmp(file_ext, ".css") == 0) {
            content_type = "text/css";
        } else if (file_ext != NULL && strcmp(file_ext, ".js") == 0) {
            content_type = "application/javascript";
        } else {
            content_type = "text/plain";
        }
        uv_buf_t headers[] = {
            uv_buf_init("HTTP/1.1 200 OK\r\n", strlen("HTTP/1.1 200 OK\r\n")),
            uv_buf_init("Content-Type: ", strlen("Content-Type: ")),
            uv_buf_init(content_type, strlen(content_type)),
            uv_buf_init("\r\n\r\n", strlen("\r\n\r\n"))
        };
        send_response(file_req->client, 200, headers, 4, &iov, 1);
        free(content);
    }
}

void send_response(uv_tcp_t *client, int status_code, uv_buf_t *headers, int headers_count, uv_buf_t *iov, int iov_count) {
    int total_size = 0;
    for (int i = 0; i < iov_count; i++) {
        total_size += iov[i].len;
    }
    for (int i = 0; i < headers_count; i++) {
        total_size += headers[i].len;
    }
    uv_buf_t *bufs = (uv_buf_t *)malloc(sizeof(uv_buf_t) * (headers_count + iov_count));
    int buf_idx = 0;
    for (int i = 0; i < headers_count; i++) {
        bufs[buf_idx++] = headers[i];
    }
    for (int i = 0; i < iov_count; i++) {
        bufs[buf_idx++] = iov[i];
    }
    uv_write_t *write_req = (uv_write_t *)malloc(sizeof(uv_write_t));
    uv_write(write_req, (uv_stream_t *)client, bufs, headers_count + iov_count, on_write_end);
    free(bufs);
}

void on_write_end(uv_write_t *write_req, int status) {
    if (status < 0) {
        fprintf(stderr, "Write error: %s\n", uv_strerror(status));
    }
    free(write_req->data);
    free(write_req);
}


/****/

void send_response(uv_tcp_t *client, int status_code, uv_buf_t *iov, int iov_count, const char *content_type)
{
    char status_text[64];
    switch (status_code)
    {
    case 200:
        strcpy(status_text, "OK");
        break;
    case 404:
        strcpy(status_text, "Not Found");
        break;
    // add more status codes here as needed
    default:
        strcpy(status_text, "Unknown");
        break;
    }

    char header[1024];
    int content_length = 0;
    for (int i = 0; i < iov_count; i++)
    {
        content_length += iov[i].len;
    }

    snprintf(header, sizeof(header),
             "HTTP/1.1 %d %s\r\n"
             "Content-Length: %d\r\n"
             "Content-Type: %s\r\n" // include the content type of the file being sent
             "\r\n",
             status_code, status_text, content_length, content_type);

    uv_buf_t header_buf = uv_buf_init(header, strlen(header));
    uv_write_t *write_req = (uv_write_t *)malloc(sizeof(uv_write_t));

    // send the header first
    uv_write(write_req, (uv_stream_t *)client, &header_buf, 1, on_write_end);

    // send the iov buffers one by one, waiting for each write to complete before sending the next one
    for (int i = 0; i < iov_count; i++)
    {
        uv_write_t *write_req2 = (uv_write_t *)malloc(sizeof(uv_write_t));
        uv_buf_t *iov_buf = &iov[i];
        uv_write(write_req2, (uv_stream_t *)client, iov_buf, 1, on_write_end);
    }
}
