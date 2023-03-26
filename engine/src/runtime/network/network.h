#pragma once
#include <uv.h>

namespace Piccolo
{

namespace Network {

typedef struct {
    uv_tcp_t server;
    uv_loop_t* loop;
    int listen_port;
} NetNode;

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

int start(NetNode *net_node);

} // namespace network
} // namespace piccolo