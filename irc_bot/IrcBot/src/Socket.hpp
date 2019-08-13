#pragma once

#include <iostream>
#include <sstream>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib,"WS2_32")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define closesocket(s) close(s)
#define close(s)
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#endif

class Socket
{
private:
	uint64_t _socket;

	bool _connected;

public:
    bool init();

    bool connect(char const* host, int port);
    void disconnect();

    bool isConnected() { return _connected; };

    bool send(char const* data);
    std::string receive();
};
