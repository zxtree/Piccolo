#pragma once

#include <vector>

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

        void init();

        void update();

        // void registerSelf(NetNodeInfo remote);

        // void send(NetNodeInfo dst, Message msg);

        // void recv(Message msg);
    };
}