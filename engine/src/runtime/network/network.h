// network node
// author: zxtree @2023-04-09
#pragma once

#include "piccolodef.h"

#include <uv.h>

PCL_ENGINE_NS_OPEN

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

PCL_ENGINE_NS_CLOSE