#include "runtime/core/base/log.h"

#include "network.h"
#include <WS2tcpip.h>

namespace Piccolo {

int NetNode::init() {
    LOG_DEBUG("net node init");

    DWORD iResult = WSAStartup(MAKEWORD(2,2), &wsaData);

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(17000);
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

    unsigned long ul = 1;
    serverSocket = WSASocketW(AF_INET, SOCK_STREAM, 0, nullptr, 0, WSA_FLAG_OVERLAPPED);
    if (INVALID_SOCKET == serverSocket) {
        LOG_ERROR("FAILED TO CREATE SERVER SOCKET:");
        closesocket(serverSocket);
        return WSAGetLastError();
    }
    LOG_DEBUG("create serversocket");
    
    if (SOCKET_ERROR == ioctlsocket(serverSocket, FIONBIO, &ul)) {
        LOG_ERROR("FAILED TO SET NONBLOCKING SOCKET");
        closesocket(serverSocket);
        return WSAGetLastError();
    }
    LOG_DEBUG("set nonblocking socket");

    if (SOCKET_ERROR == bind(serverSocket, (const struct sockaddr *) &address, sizeof(address))) {
        LOG_ERROR("FAILED TO BIND ADDRESS");
        closesocket(serverSocket);
        return WSAGetLastError();
    }
    LOG_DEBUG("bind succ");

    if (SOCKET_ERROR == listen(serverSocket, SOMAXCONN)) {
        LOG_ERROR("FAILED TO LISTEN SOCKET");
        closesocket(serverSocket);
        return WSAGetLastError();
    }
    LOG_DEBUG("listen succ");

    hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, NumberOfThreads);
    if (INVALID_HANDLE_VALUE == hIOCP) {
        LOG_ERROR("FAILED TO CREATE IOCP HANDLE");
        closesocket(serverSocket);
        return WSAGetLastError();
    }
    LOG_DEBUG("create complete port");

    for (size_t i = 0; i < NumberOfThreads; i++) {
        threadGroup.emplace_back(std::thread(&NetNode::event, this));
    }

    void *lpCompletionKey = nullptr;
    std::thread acceptThread = std::thread(&NetNode::accept1, this);
    // getchar();
    // isShutdown = true;

    for (size_t i = 0; i < NumberOfThreads; i++) {
        PostQueuedCompletionStatus(hIOCP, -1, (ULONG_PTR) lpCompletionKey, nullptr);
    }
    acceptThread.join();
    for (auto &thread: threadGroup) {
        thread.join();
    }

    WSACleanup();
    return 0;
}

void NetNode::accept1() {
    while (!isShutdown) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);

        if (INVALID_SOCKET == clientSocket) continue;
        LOG_DEBUG("accept");
        
        unsigned long ul = 1;
        if (SOCKET_ERROR == ioctlsocket(clientSocket, FIONBIO, &ul)) {
            shutdown(clientSocket, SD_BOTH);
            closesocket(clientSocket);
            continue;
        }

        if (nullptr == CreateIoCompletionPort((HANDLE) clientSocket, hIOCP, 0, 0)) {
            shutdown(clientSocket, SD_BOTH);
            closesocket(clientSocket);
            continue;
        }

        DWORD nBytes = MaxBufferSize;
        DWORD dwFlags = 0;
        auto ioContext = new IOContext;
        ioContext->socket = clientSocket;
        ioContext->type = IOType::Read;

        auto rt = WSARecv(clientSocket, &ioContext->wsaBuf, 1, &nBytes, &dwFlags, &ioContext->overlapped, nullptr);
        LOG_DEBUG("accept recv");
        auto err = WSAGetLastError();
        if (SOCKET_ERROR == rt && ERROR_IO_PENDING != err) {

            //shutdown(clientSocket, SD_BOTH);
            //closesocket(clientSocket);
            delete ioContext;
        }
    }
}

void NetNode::event() {
    LOG_DEBUG("event");
    IOContext *ioContext = nullptr;
    DWORD lpNumberOfBytesTransferred = 0;
    void *lpCompletionKey = nullptr;

    DWORD dwFlags = 0;
    DWORD nBytes = MaxBufferSize;

    while (true) {
        BOOL bRt = GetQueuedCompletionStatus(
                hIOCP,
                &lpNumberOfBytesTransferred,
                (PULONG_PTR) &lpCompletionKey,
                (LPOVERLAPPED *) &ioContext,
                INFINITE);

        if (!bRt) continue;
        
        LOG_DEBUG("lb number of bytes transferred: {}", lpNumberOfBytesTransferred);

        if (lpNumberOfBytesTransferred == -1) {
            LOG_DEBUG("lb number of bytes transferred = -1");
            continue;;
        }

        if (lpNumberOfBytesTransferred == 0) continue;
        
        LOG_DEBUG("event get queued complete port2");
        ioContext->nBytes = lpNumberOfBytesTransferred;
        switch (ioContext->type) {
            case IOType::Read: {
                int nRt = WSARecv(
                        ioContext->socket,
                        &ioContext->wsaBuf,
                        1,
                        &nBytes,
                        &dwFlags,
                        &(ioContext->overlapped),
                        nullptr);
                auto e = WSAGetLastError();
                if (SOCKET_ERROR == nRt && e != WSAGetLastError()) {
                    closesocket(ioContext->socket);
                    delete ioContext;
                    ioContext = nullptr;
                } else {
                    LOG_DEBUG("event wsarecv");
                    LOG_INFO("event recv buf: {}", ioContext->buffer);
                    strcat(ioContext->buffer, "send");
                    int rc = WSASend(ioContext->socket, &ioContext->wsaBuf, 1, &lpNumberOfBytesTransferred, dwFlags, &(ioContext->overlapped), NULL);
                    if(rc == SOCKET_ERROR) {
                        LOG_ERROR("event send error: {}", WSAGetLastError());
                    }
                    LOG_DEBUG("event send: {}", ioContext->buffer);
                    closesocket(ioContext->socket);
                    delete ioContext;
                    ioContext = nullptr;
                }
                break;
            }
            case IOType::Write: {
                LOG_ERROR("event write");
                break;
            }
        }
    }
    LOG_ERROR("event end");
}

void NetNode::update() {
    LOG_DEBUG("update");
    
}

}