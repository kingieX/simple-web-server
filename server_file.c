#include "main.h"

/**
 * serve_file - send the content of the requested file to the client as an HTTP response
 * @file_req: a pointer to a struct file_request_t that holds the path and the client information
 */
void serve_file(file_request_t *file_req)
{
    char *content = read_file(file_req);
    if (content == NULL)
    {
        send_response(file_req->client, 404, NULL, 0, "text/plain");
    }
    else
    {
        char *css_file = NULL;
        char *html_file = NULL;

        if (strstr(file_req->path, ".html") != NULL)
        {
            html_file = file_req->path;
            char *link_tag = strstr(content, "<link");
            while (link_tag != NULL)
            {
                char *href_attr = strstr(link_tag, "href");
                if (href_attr != NULL)
                {
                    char *href_start = strchr(href_attr, '\"');
                    char *href_end = strchr(href_start + 1, '\"');
                    char href_value[256];
                    strncpy(href_value, href_start + 1, href_end - href_start - 1);
                    href_value[href_end - href_start - 1] = '\0';

                    if (strstr(href_value, ".css") != NULL)
                    {
                        css_file = href_value;
                        break;
                    }
                }

                link_tag = strstr(link_tag + 1, "<link");
            }
        }

        uv_buf_t iov[2];
        int iov_count = 0;

        if (html_file != NULL)
        {
            iov[iov_count++] = uv_buf_init(content, strlen(content));
        }
        else
        {
            iov[iov_count++] = uv_buf_init(content, get_file_size(file_req->path));
        }

        if (css_file != NULL)
        {
            iov[iov_count++] = uv_buf_init(read_file_path(css_file), get_file_size(css_file));
        }

        const char *content_type = "text/css";
        if (html_file != NULL)
        {
            content_type = "text/html";
        }
        else if (css_file != NULL)
        {
            content_type = "text/css";
        }

        send_response(file_req->client, 200, iov, iov_count, content_type);

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
             "Content-Type: %s\r\n"
             "\r\n",
             status_code, status_text, content_length, content_type);

    uv_buf_t header_buf = uv_buf_init(header, strlen(header));
    uv_write_t *write_req = (uv_write_t *)malloc(sizeof(uv_write_t));

    uv_write(write_req, (uv_stream_t *)client, &header_buf, 1, on_write_end);

    for (int i = 0; i < iov_count; i++)
    {
        uv_write_t *write_req2 = (uv_write_t *)malloc(sizeof(uv_write_t));
        uv_buf_t *iov_buf = &iov[i];
        uv_write(write_req2, (uv_stream_t *)client, iov_buf, 1, on_write_end);
    }
}

void on_write_end(uv_write_t *req, int status)
{
    free(req);
}
