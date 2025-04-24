#pragma once
#define MULTI_PORT 0x10B2
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif

class connection {
	bool servMode;
#ifdef _WIN32
	SOCKET sock, clientSock;
#else
	int sock, clientSock;
#endif
	sockaddr_in servaddr, clientaddr;
public:
	connection(bool&, char*);
	~connection();
	void getSignal(char*&) const;
	void sendSignal(char*&) const;
	bool isServ() const;
};

