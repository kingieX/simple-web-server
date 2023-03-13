#include "main.h"

/**
 * on_close - frees buffers
 * return: void
 */
void on_close(uv_handle_t *handle)
{
	uv_tcp_t *client = (uv_tcp_t *)handle;
	http_parser_t *parser = (http_parser_t *)client->data;

	if (parser != NULL)
	{
		free(parser);
	}

	free(client);
}
