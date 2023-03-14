#include "main.h"

/**
 * read_file - read the content of the requested file 
 * @file_req: a pointer to a struct file_request_t that holds the path and the client information 
 * 
 * Return: a string containing the content of the file to be read
*/

char* read_file(file_request_t *file_req) {
    char *path = file_req->path;
    uv_fs_t *open_req = (uv_fs_t *)malloc(sizeof(uv_fs_t));
    uv_fs_open(uv_default_loop(), open_req, path, O_RDONLY, 0, NULL);

    if (open_req->result < 0)
    {
        fprintf(stderr, "Error opening file: %s\n", uv_strerror(open_req->result));
        free(open_req);
        return NULL;
    }

    uv_fs_t *stat_req = (uv_fs_t *)malloc(sizeof(uv_fs_t));
    uv_fs_fstat(uv_default_loop(), stat_req, open_req->result, NULL);

    if (stat_req->result < 0)
    {
        fprintf(stderr, "Error getting file info: %s\n", uv_strerror(stat_req->result));
        free(open_req);
        free(stat_req);
        return NULL;
    }

    int size = stat_req->statbuf.st_size;
    char *buf = (char *)malloc(size);

    uv_buf_t iov = uv_buf_init(buf, size);
    uv_fs_t *read_req = (uv_fs_t *)malloc(sizeof(uv_fs_t));
    uv_fs_read(uv_default_loop(), read_req, open_req->result, &iov, 1, 0, NULL);

    if (read_req->result < 0)
    {
        fprintf(stderr, "Error reading file: %s\n", uv_strerror(read_req->result));
        free(open_req);
        free(stat_req);
        free(buf);
        free(read_req);
        return NULL;
    }

    uv_fs_req_cleanup(open_req);
    uv_fs_req_cleanup(stat_req);
    uv_fs_req_cleanup(read_req);

    return buf;
}
