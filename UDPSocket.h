#pragma once
#pragma comment(lib, "ws2_32.lib")

#include "Component.h"
#include "Math.h"
#include "Actor.h"
#include <vector>
#include <string>
#include <Windows.h>

class UDPSocket : public Component
{
public:
	UDPSocket(class Actor* owner, u_short port);
	~UDPSocket();
	void Update(float deltaTime) override;

	bool Init();
	void UDP_Receive();
	const Vector2& GetPointPosition(int index);

private:
	WSADATA wsaData;
	SOCKET sock;
	struct sockaddr_in addr;
	u_short port;
	std::vector<std::vector<int>> handPoints;
	char buf[2048] = {};
	char delimiter;
	size_t delimiter_length;
};