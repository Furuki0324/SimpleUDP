#pragma once
#pragma comment(lib, "ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Windows.h>
#include <WinSock2.h>
#include <string>


class simple_udp {


public:
	simple_udp(std::string address, int port) 
	{
		sock = socket(AF_INET, SOCK_DGRAM, 17);		//17 is the UDP protocol(IPPROTO_UDP).
		addr.sin_family = AF_INET;
		addr.sin_addr.S_un.S_addr = inet_addr(address.c_str());
		addr.sin_port = htons(port);
	}

	~simple_udp()
	{
		closesocket(sock);
	}

	void udp_send(std::string word)
	{
		sendto(sock, word.c_str(), word.length(), 0, (struct sockaddr*)&addr, sizeof(addr));
	}

	void udp_bind()
	{
		bind(sock, (struct sockaddr*)&addr, sizeof(addr));
	}

	std::string udp_recv()
	{
#define BUFFER_MAX 400
		char buf[BUFFER_MAX];
		memset(buf, 0, sizeof(buf));
		recv(sock, buf, sizeof(buf), MSG_PEEK);
		return std::string(buf);
	}

	void udp_recv(char* buf, int size)
	{
		memset(buf, 0, size);
		recv(sock, buf, size, MSG_PEEK);	//MSG_PEEK means that it will copy the incoming data into buffer, but will not remove the incoming data.
	}


public:
	WSAData wsaData;

private:
	SOCKET sock;
	sockaddr_in addr;
};