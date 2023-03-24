#pragma once
#include <WinSock2.h>
#include <atomic>
#include <vector>
#include <thread>

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

enum class IOType {
    Read,
    Write
};

#define MaxBufferSize 512

struct IOContext {
    OVERLAPPED overlapped{};
    WSABUF wsaBuf{MaxBufferSize, buffer};
    CHAR buffer[MaxBufferSize]{};
    IOType type{};
    SOCKET socket = INVALID_SOCKET;
    DWORD nBytes = 0;
};

class NetNode
{
public:
    // NetNodeInfo self_info;

    // std::vector<NetNode> node_list;

    WSAData wsaData;
    std::atomic_bool isShutdown = false;
    SOCKET serverSocket;
    HANDLE hIOCP;
    DWORD NumberOfThreads = 1;
    std::vector<std::thread> threadGroup;

    int init();

    void accept1();

    void event();

    void update();

    // void registerSelf(NetNodeInfo remote);

    // void send(NetNodeInfo dst, Message msg);

    // void recv(Message msg);
};

} // namespace end