#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include "runtime/network/network.h"
//#include "runtime/core/base/log.h"

#define DEFAULT_BUFLEN 512
//#define DEFAULT_PORT "17000"


using namespace Piccolo::Engine;

int main() {
    NetNode node;
    node.listen_port = 17000;
    
    start(&node);
    return 0;
}