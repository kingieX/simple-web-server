#include "main.h"

/**
 * extract the path of the requested file from the URL
 * @url: the url 
 * @path: the path of the requested file 
 * @size: the size of the path 
 * 
 * Returns: Nothing
*/

void extract_http_request_path(const char *url, char *path, size_t size)
{
    const char *path_end;
    const char *path_start = strstr(url, "://");
    if (path_start == NULL) 
    {
        path_start = url; 
    } 
    else 
    {
        path_start += 3; 
        path_start = strchr(path_start, '/');
        if (path_start == NULL) 
        {
            path_start = url + strlen(url);
        }
    }

    path_end = url + strlen(url);

    size_t path_len = path_end - path_start;
    if (path_len >= size) 
    {
        path_len = size - 1;  
    }
    strncpy(path, path_start, path_len);
    path[path_len] = '\0';
}
