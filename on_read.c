#include "main.h"

/**
 * on_read - reads the passed buffer, and parse the HTTP request
 * @client: pointer to client socket
 * @nread: number of bytes read
 * @buf: pointer to buffer containing data to be read
 * return: void
 */
void on_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf)
{
	file_request_t *file_request;
	http_parser_t *parser;
	size_t parsed;

	if (nread < 0)
	{
		if (nread != UV_EOF)
		{
			fprintf(stderr, "Error on read: %s\n",
				uv_strerror(nread));
		}
		uv_close((uv_handle_t *)client, on_close);
		free(buf->base);
		return;
	}

	parser = (http_parser_t *)client->data;
	parsed = http_parser_execute(parser, &settings,
		       		     buf->base, nread);

	if (parsed != nread)
	{
		fprintf(stderr, "Error parsing HTTP request\n");
		uv_close(client, on_close);
		free(buf->base);
		return;
	}

	if (parser->method == HTTP_GET)
	{
		file_request = malloc(sizeof(file_request_t));
		file_request->client = (uv_tcp_t *)client;
		snprintf(file_request->path, 1024, "%s", parser->path);
		serve_file(file_request);
	}

	free(buf->base);
}
