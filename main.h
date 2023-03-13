#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

void extract_http_request_path(const char *url, char *path, size_t size);