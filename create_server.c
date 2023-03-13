#include "main.h"

/**
 * create_server - a function that creates a server
 * @loop: the event loop instance to initialize with server
 * return: created server
 */
uv_tcp_t *create_server(uv_loop_t *loop)
{
	struct sockaddr_in addr;

	uv_tcp_t *server = malloc(sizeof(uv_tcp_t));
	uv_tcp_init(loop, server);

	uv_ip4_addr("0.0.0.0", 8080, &addr);
	uv_tcp_bind(server, (const struct sockaddr *)&addr, 0);
	return server;
}
