#include "main.h"

/**
 * Extract the path of the requested file from the URL and store it in file_req->path
 * @file_req: a pointer to a file_request_t struct
 * 
 * Returns: Nothing
*/
void extract_http_request_path(file_request_t *file_req)
{
    const char *path_end;
    const char *path_start = strstr(file_req->path, "://");
    if (path_start == NULL) 
    {
        path_start = file_req->path; 
    } 
    else 
    {
        path_start += 3; 
        path_start = strchr(path_start, '/');
        if (path_start == NULL) 
        {
            path_start = file_req->path + strlen(file_req->path);
        }
    }

    path_end = file_req->path + strlen(file_req->path);

    size_t path_len = path_end - path_start;
    if (path_len >= sizeof(file_req->path)) 
    {
        path_len = sizeof(file_req->path) - 1;  
    }
    strncpy(file_req->path, path_start, path_len);
    file_req->path[path_len] = '\0';
}
