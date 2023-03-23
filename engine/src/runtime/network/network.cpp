#include <WS2tcpip.h>

#include "runtime/core/base/macro.h"
#include "runtime/core/base/log.h"

#include "network.h"

namespace Piccolo {

void NetNode::init() {
    LOG_DEBUG("net node init");

    DWORD iResult = WSAStartup(MAKEWORD(2,2), &wsaData);

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(17000);
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

    // 初始化 Socket
    unsigned long ul = 1;
    serverSocket = WSASocketW(AF_INET, SOCK_STREAM, 0, nullptr, 0, WSA_FLAG_OVERLAPPED);
    if (INVALID_SOCKET == serverSocket) {
        DWORD err = WSAGetLastError();
        LOG_SYS_ERR("FAILED TO CREATE SERVER SOCKET:", err);
        closesocket(serverSocket);
        exit(err);
    }
    // 设置为非阻塞 IO
    if (SOCKET_ERROR == ioctlsocket(serverSocket, FIONBIO, &ul)) {
        perror("FAILED TO SET NONBLOCKING SOCKET");
        closesocket(serverSocket);
        exit(-2);
    }
    if (SOCKET_ERROR == bind(serverSocket, (const struct sockaddr *) &address, sizeof(address))) {
    perror("FAILED TO BIND ADDRESS");
        closesocket(serverSocket);
        exit(-3);
    }
    if (SOCKET_ERROR == listen(serverSocket, SOMAXCONN)) {
        perror("FAILED TO LISTEN SOCKET");
        closesocket(serverSocket);
        exit(-4);
    }

    isShutdown = false;
    hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, NumberOfThreads);
    if (INVALID_HANDLE_VALUE == hIOCP) {
        perror("FAILED TO CREATE IOCP HANDLE");
        closesocket(serverSocket);
        exit(-5);
    }
}

void NetNode::accept1() {
    while (!isShutdown) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (INVALID_SOCKET == clientSocket) continue;

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
        auto err = WSAGetLastError();
        if (SOCKET_ERROR == rt && ERROR_IO_PENDING != err) {

            shutdown(clientSocket, SD_BOTH);
            closesocket(clientSocket);
            delete ioContext;
        }
    }
}

void NetNode::event() {
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

        if (lpNumberOfBytesTransferred == -1) break;

        if (lpNumberOfBytesTransferred == 0) continue;

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
                    setbuf(stdout, nullptr);
                    puts(ioContext->buffer);
                    fflush(stdout);
                    closesocket(ioContext->socket);
                    delete ioContext;
                    ioContext = nullptr;
                }
                break;
            }
            case IOType::Write: {
                break;
            }
        }
    }
}

void NetNode::update() {
    LOG_DEBUG("update");
    
}

}