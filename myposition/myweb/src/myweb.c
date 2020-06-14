#include <kore/kore.h>
#include <kore/http.h>
#include <linux/fs.h>
#include <stdio.h>
#include <string.h> //strncpy() 
#include <unistd.h>	//sleep(s)/usleep(us)
#include "assets.h"

int page(struct http_request *);
int page_ws_connect(struct http_request *);
void websocket_connect(struct connection *);
void websocket_disconnect(struct connection *);
void websocket_message(struct connection *, u_int8_t, void *, size_t);

/* Called whenever we get a new websocket connection. */
void websocket_connect(struct connection *c)
{
	kore_log(LOG_NOTICE, "%p: connected", c);
}

void websocket_message(struct connection *c, u_int8_t op, void *data, size_t len)
{
	kore_log(LOG_NOTICE, "%p:got the message from client: %s", c, data);

	size_t mylen;
	struct kore_buf *buf;
	u_int8_t *mydata;

	char buffer[64];
	int count;
	int fd;
	//sending interval 0.2s / total 500 times
	for (int i = 0; i < 500; i++)
	{
		buf = kore_buf_alloc(128);
		fd = open("/dev/position", 0);
		read(fd, buffer, 32);
		kore_buf_appendf(buf, "(X,Y,Z) = (%s) \n", buffer); //like snprintf
		close(fd);
		mydata = kore_buf_release(buf, &mylen);

		kore_websocket_send(c, op, mydata, mylen); //send position
		kore_log(LOG_NOTICE, "%p:send the message to client: %s", c, mydata);

		kore_free(mydata);
		usleep(200000); //200000us=200ms=0.2s
	}
}

void websocket_disconnect(struct connection *c)
{
	kore_log(LOG_NOTICE, "%p: disconnecting", c);
}

int page(struct http_request *req)
{
	http_response_header(req, "content-type", "text/html");
	http_response(req, 200, asset_index_html, asset_len_index_html);

	return (KORE_RESULT_OK);
}
//handshake
int page_ws_connect(struct http_request *req)
{
	/* Perform the websocket handshake, passing our callbacks. */
	kore_websocket_handshake(req, "websocket_connect",
							 "websocket_message", "websocket_disconnect");

	return (KORE_RESULT_OK);
}
