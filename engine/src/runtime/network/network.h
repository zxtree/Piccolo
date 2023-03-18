#pragma once

#include <vector>

#include "kcp/ikcp.h"

namespace Piccolo
{
    struct NetNodeInfo
    {
        int addr;
    };

    struct Message
    {
        int data;
    };

    class NetNode
    {
    public:
        // NetNodeInfo self_info;

        // std::vector<NetNode> node_list;

        ikcpcb* m_kcpcb;

        void init();

        // void registerSelf(NetNodeInfo remote);

        // void send(NetNodeInfo dst, Message msg);

        // void recv(Message msg);
    };
}