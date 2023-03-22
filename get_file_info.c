#include "main.h"

char* read_file_path(char* file_name) {
    char* buffer = (char*) malloc(strlen(file_name) + 1);
    if (buffer == NULL) {
        return NULL;
    }
    strcpy(buffer, file_name);
    return buffer;
}

long get_file_size(char* file_name) {
    FILE* fp = fopen(file_name, "rb");
    if (fp == NULL) {
        return -1;
    }
    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return size;
}

char* get_content_type(const char* path) {
    char* ext = strrchr(path, '.');
    if (ext == NULL) {
        return "application/octet-stream";
    } else if (strcmp(ext, ".html") == 0) {
        return "text/html";
    } else if (strcmp(ext, ".css") == 0) {
        return "text/css";
    } else if (strcmp(ext, ".js") == 0) {
        return "text/javascript";
    } else if (strcmp(ext, ".json") == 0) {
        return "application/json";
    } else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) {
        return "image/jpeg";
    } else if (strcmp(ext, ".png") == 0) {
        return "image/png";
    } else if (strcmp(ext, ".gif") == 0) {
        return "image/gif";
    } else if (strcmp(ext, ".pdf") == 0) {
        return "application/pdf";
    } else {
        return "application/octet-stream";
    }
}
