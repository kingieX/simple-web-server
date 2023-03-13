#include "server.h"

/**
 * on_connect - accepts tcp conncetion and reads into allocated buffer
 * @server: pointer to server socket
 * @status: status code of connection
 * return: void
 */ 
void on_connect(uv_stream_t *server, int status)
{
	uv_tcp_t *client;

	if (status < 0)
	{
		fprintf(stderr, "Error on connection: %s\n", uv_strerror(status));
		return;
	}

	client = malloc(sizeof(uv_tcp_t));
	uv_tcp_init(uv_default_loop(), client);
	
	if (uv_accept(server, (uv_stream_t *)client) == 0)
	{
		uv_read_start((uv_stream_t *)client, 
			       alloc_buffer, on_read);
	}
	else
	{
		uv_close((uv_handle_t *)client, on_close);
	}
}
