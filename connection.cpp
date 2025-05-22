#include "connection.h"
#include <cstring>
#include <stdexcept>
#include <string>
#ifndef _WIN32
#include <sys/socket.h>
#include <unistd.h>
#define closesocket close
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#else
#pragma comment(lib, "ws2_32.lib")
#endif

connection::connection(bool& servMode, char* ipa) {
    this->servMode = servMode;
    this->sock = socket(AF_INET, SOCK_STREAM, 0);

    this->servaddr.sin_family = AF_INET;
    this->servaddr.sin_port = htons(MULTI_PORT);

    if (servMode) {
        this->servaddr.sin_addr.s_addr = INADDR_ANY;
        if (bind(this->sock, (sockaddr*)&this->servaddr, sizeof(this->servaddr)) == SOCKET_ERROR) {
#ifndef _WIN32
            int error = WSAGetLastError();
#endif
            closesocket(this->sock);
            throw error;
        }

        if (listen(this->sock, SOMAXCONN) == SOCKET_ERROR) {
            closesocket(this->sock);
            throw 1;
        }

        sockaddr_in clientAddr;
#ifdef _WIN32
        int clientAddrLen = sizeof(clientAddr);
#else
        socklen_t clientAddrLen = sizeof(clientAddr);
#endif
        this->clientSock = accept(this->sock, (sockaddr*)&clientAddr, &clientAddrLen);

        if (this->clientSock == INVALID_SOCKET) {
            closesocket(this->sock);
            throw 1;
        }
    } else {
        inet_pton(AF_INET, ipa, &servaddr.sin_addr);
        if (connect(this->sock, (sockaddr*)&this->servaddr, sizeof(this->servaddr)) == SOCKET_ERROR) {
            closesocket(this->sock);
            throw 1;
        }
    }
}

connection::~connection() {
    if (this->servMode) {
        if (this->clientSock != INVALID_SOCKET) {
            closesocket(this->clientSock);
        }
    }
    if (this->sock != INVALID_SOCKET) {
        closesocket(this->sock);
    }
}

void connection::getSignal(char*& buffer) const {
    recv(this->servMode ? clientSock : sock, buffer, 2, 0);
}

void connection::sendSignal(char*& buffer) const {
    send(this->servMode ? clientSock : sock, buffer, 2, 0);
}

bool connection::isServ() const {
    return this->servMode;
}
