#include "runtime/core/base/log.h"

#include "network.h"

PCL_ENGINE_NS_OPEN

#define LOG_UV_ERR(MSG, ERR) LOG_ERROR(MSG + "{}-{}", ERR, uv_err_name(ERR))
#define UV_CHECK_RC(RC, MSG) \
    if(RC) {\
        LOG_UV_ERR(MSG + " error ", RC);\
        return RC;\
    }\
    else {\
        LOG_DEBUG(MSG + " success")\
    }

void free_write_req(uv_write_t *req) {
    write_req_t *wr = (write_req_t*) req;
    free(wr->buf.base);
    free(wr);
}

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = (char*) malloc(suggested_size);
    buf->len = suggested_size;
}

void echo_write(uv_write_t *req, int status) {
    LOG_DEBUG("echo write");
    if(status) {
        LOG_ERROR("write error: {}-{}", status, uv_strerror(status));
        return;
    }
    free_write_req(req);
}

void on_close(uv_handle_t* handle) {
    LOG_DEBUG("close");
    free(handle);
}

void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    LOG_DEBUG("echo read");
    if(nread > 0) {
        write_req_t *req = (write_req_t*) malloc(sizeof(write_req_t));
        req->buf = uv_buf_init(buf->base, nread);
        LOG_DEBUG("read: {}, nread: {}", std::string(buf->base, nread), nread);
        uv_buf_t send;
        send.base = (char*) ("send");
        send.len = 4;
        uv_write((uv_write_t*) req, client, &send, 1, echo_write);
        return;
    }

    if(nread < 0 && nread != UV_EOF) {
        LOG_UV_ERR("read error", nread);
        free(buf->base);
    }
    
    uv_close((uv_handle_t*) client, on_close);
}

void on_new_connection(uv_stream_t *server, int status) {
    LOG_DEBUG("on new conn");
    if(status < 0) {
        LOG_ERROR("new connection error: {}", uv_strerror(status));
        return;
    }

    uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    SOCKET client_sock = client->socket;
    uv_tcp_init(server->loop, client);

    if(uv_accept(server, (uv_stream_t*) client) == 0) {
        uv_read_start((uv_stream_t*) client, alloc_buffer, echo_read);
    }
    else {
        uv_close((uv_handle_t*) client, on_close);
    }
}

int start(NetNode* net_node) {
    int rc = 0;
    uv_loop_t* loop = uv_default_loop();
    rc = uv_tcp_init(loop, &net_node->server);
    UV_CHECK_RC(rc, "init")

    sockaddr_in addr;
    rc = uv_ip4_addr("0.0.0.0", net_node->listen_port, &addr);
    UV_CHECK_RC(rc, "addr")

    rc = uv_tcp_bind(&net_node->server, (const struct sockaddr*) &addr, 0);
    UV_CHECK_RC(rc, "bind")
    
    rc = uv_listen((uv_stream_t*) &net_node->server, 128, &on_new_connection);
    UV_CHECK_RC(rc, "listen")

    return uv_run(loop, UV_RUN_DEFAULT);
}

PCL_ENGINE_NS_CLOSE