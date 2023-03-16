#include "main.h"

/**
 * serve_file - send the content of the requested file to the client as an HTTP response
 * @file_req: a pointer to a struct file_request_t that holds the path and the client information
 */
void serve_file(file_request_t *file_req) {
    char *content = read_file(file_req);
    if (content == NULL) {
        send_response(file_req->client, 404, NULL, 0);
    } else {
        uv_buf_t iov = uv_buf_init(content, strlen(content));
        send_response(file_req->client, 200, &iov, 1);
        free(content);
    }
}

/**
 * send_response - send an HTTP response to the client
 * @client: the client to send the response to
 * @status_code: the HTTP status code of the response
 * @iov: an array of uv_buf_t structs containing the response data
 * @iov_count: the number of elements in the iov array
 */
void send_response(uv_tcp_t *client, int status_code, uv_buf_t *iov, int iov_count) {
    char status_text[64];
    switch (status_code) {
        case 200: strcpy(status_text, "OK"); break;
        case 404: strcpy(status_text, "Not Found"); break;
        // will add more status code after the test have been done
        default: strcpy(status_text, "Unknown"); break;
    }

    char header[1024];
    int content_length = 0;
    for (int i = 0; i < iov_count; i++) {
        content_length += iov[i].len;
    }
    snprintf(header, sizeof(header),
             "HTTP/1.1 %d %s\r\n"
             "Content-Length: %d\r\n"
             "Content-Type: text/html\r\n"
             "\r\n",
             status_code, status_text, content_length);

    uv_buf_t header_buf = uv_buf_init(header, strlen(header));
    uv_write_t *write_req = (uv_write_t *)malloc(sizeof(uv_write_t));
    uv_write(write_req, (uv_stream_t *)client, &header_buf, 1, on_write_end);

    if (iov_count > 0) {
        uv_write_t *write_req2 = (uv_write_t *)malloc(sizeof(uv_write_t) * iov_count);
        uv_write(write_req2, (uv_stream_t *)client, iov, iov_count, on_write_end);
    }
}

/**
 * on_write_end - callback function called when a write operation is complete
 * @write_req: the uv_write_t struct associated with the completed write operation
 * @status: the status code of the completed write operation
 */
void on_write_end(uv_write_t *write_req, int status) {
    if (status < 0) {
        fprintf(stderr, "Write error: %s\n", uv_strerror(status));
    }
}
