#include "main.h"

/**
 * main - fires up the server
 * return: 0 on success and 1 on failure
 */
int main(void)
{
	uv_loop_t *loop = uv_default_loop();

	uv_tcp_t *server = create_server(loop);

	int r = uv_listen((uv_stream_t *)server,\
			  DEFAULT_BACKLOG, on_connect);
	if (r)
	{
		fprintf(stderr, "Error listening: %s\n", uv_strerror(r));
		return 1;
	}
	printf("Listening for connection...\n");

	uv_run(loop, UV_RUN_DEFAULT);
	
	uv_loop_close(loop);
	free(loop);

	return 0;
}
