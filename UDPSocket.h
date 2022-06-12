#pragma once
#pragma comment(lib, "ws2_32.lib")

#include "Component.h"
//#include "Math.h"
#include "Actor.h"
//#include <vector>
//#include <string>
//#include <Windows.h>

class UDPSocket : public Component
{
public:
	UDPSocket(class Actor* owner, u_short port, int updateOrder, bool blocking);
	~UDPSocket();
	void Update(float deltaTime) override;

	bool Init(bool blocking);
	virtual void UDP_Receive();

protected:
	WSADATA wsaData;
	SOCKET sock;
	struct sockaddr_in addr;
	u_short port;

	char buf[2048] = {};
	char delimiter;
	size_t delimiter_length;
};