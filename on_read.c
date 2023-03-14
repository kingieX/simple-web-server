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
	http_parser *parser = malloc(sizeof(http_parser));
	size_t parsed;
	http_parser_settings settings;
	parser_data_t *data = malloc(sizeof(parser_data_t));
	data->url = NULL;

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

	http_parser_init(parser, HTTP_REQUEST);
	parser->data = client;
	http_parser_settings_init(&settings);
	settings.on_url = on_url;
	parsed = http_parser_execute(parser, &settings,
		       		     buf->base, nread);
	if (parsed != nread)
	{
		fprintf(stderr, "Error parsing HTTP request\n");
		uv_close((uv_handle_t *)client, on_close);
		free(buf->base);
		return;
	}

	if (parser->method == HTTP_GET)
	{
		file_request = malloc(sizeof(file_request_t));
		file_request->client = (uv_tcp_t *)client;
		snprintf(file_request->path, 1024, "%s", ((parser_data_t *)parser->data)->url);
		extract_http_request_path(file_request);
	}

	free(data->url);
	free(data);
	free(parser);
	free(buf->base);

}

/**
 * on_url - extracts request path and its length into struct
 * @parser: the parser containing request
 * @at: pointer to buffer containing path
 * @length: size of the buffer
 * return: 0 on success
 */
int on_url(http_parser* parser, const char* at, size_t length)
{
	parser_data_t *data = (parser_data_t *)parser->data;
	data->url = (char*)malloc(length + 1);
	snprintf(data->url, length + 1, "%s", at);
	return 0;
}
