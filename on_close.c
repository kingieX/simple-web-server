#include "main.h"

/**
 * on_close - frees buffers
 * @handle: pointer to structure to client socket
 * return: void
 */
void on_close(uv_handle_t *handle)
{
	uv_tcp_t *client = (uv_tcp_t *)handle;
	http_parser *parser = (http_parser *)client->data;

	if (parser != NULL)
	{
		free(parser);
	}

	free(client);
}
