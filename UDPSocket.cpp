#include "UDPSocket.h"
#include <iostream>

UDPSocket::UDPSocket(Actor* owner, u_short inPort, bool blocking = false)
	:Component(owner)
	,port(inPort)
	,delimiter(',')
	,delimiter_length(std::string::size_type(1))
{
	if (!Init(blocking))
	{
		printf("Error has occured in UDPSocket.\n");
		mOwner->SetState(Actor::State::EPaused);
	}
}

UDPSocket::~UDPSocket()
{}

bool UDPSocket::Init(bool blocking)
{
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed. %d\n", iResult);
		return false;
	}

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
	{
		printf("Invalid socket.");
		return false;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(sock, (struct sockaddr*)&addr, sizeof(addr));

	if (!blocking)
	{
		//recvのブロッキングを解除
		u_long val = 1;
		iResult = ioctlsocket(sock, FIONBIO, &val);
		if (iResult != NO_ERROR)
		{
			printf("ioctlsocket failed with error: %ld\n", iResult);
		}
		else
		{
			printf("Unlock blocking");
		}
	}


	return true;
}

void UDPSocket::Update(float deltaTime)
{
	UDP_Receive();
}

void UDPSocket::UDP_Receive()
{
	memset(buf, 0, sizeof(buf));
	int n = recv(sock, buf, sizeof(buf), 0);

	if (n >= 1)
	{
		printf("%s", buf);
	}
}