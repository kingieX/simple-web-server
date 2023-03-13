#include "main.h"

/**
 * alloc_buffer - allocates buffer for reading data from socket
 * @handle: pointer to socket being read
 * @suggested_size: size of buffer
 * @buf: pointer to struct containing sllocated buffer details
 * return: void
 */
void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
	buf->base = (char *)malloc(suggested_size);
	buf->len = suggested_size;
}
