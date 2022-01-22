#include "UDPSocket.h"
#include <iostream>

UDPSocket::UDPSocket(Actor* owner, u_short inPort)
	:Component(owner)
	,port(inPort)
	,delimiter(',')
	,delimiter_length(std::string::size_type(1))
{
	if (!Init())
	{
		printf("Error has occured in UDPSocket.\n");
		mOwner->SetState(Actor::State::EPaused);
	}
}

UDPSocket::~UDPSocket()
{}

bool UDPSocket::Init()
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

	//recv�̃u���b�L���O������
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

	return true;
}

void UDPSocket::Update(float deltaTime)
{
	//recv�̃u���b�L���O������
	u_long val = 1;
	int iResult = ioctlsocket(sock, FIONBIO, &val);
	UDP_Receive();
}

void UDPSocket::UDP_Receive()
{
	memset(buf, 0, sizeof(buf));
	int n = recv(sock, buf, sizeof(buf), 0);

	if (n >= 1)
	{
		handPoints.clear();

		std::string string(buf);
		auto offset = std::string::size_type(0);

		//21�ӏ��̍��W���X�g���쐬
		while (handPoints.size() < 21)
		{
			//Mediapipe�̎��ʔԍ�(0~20)��X,Y���W���܂Ƃ߂����X�g���쐬
			std::vector<int> point;
			std::vector<std::string> strings;
			while (point.size() < 3)
			{
				auto pos = string.find(delimiter, offset);
				std::string s = "000";
				if (pos != std::string::npos)
				{
					//delimiter�Ŏw�肵�������ŋ�؂�
					s = string.substr(offset, pos - offset);

					//��؂���string�ɐ����ȊO�̋L���������Ă�����폜����
					size_t c;
					while ((c = s.find_first_of("[]")) != std::string::npos)
					{
						s.erase(c, 1);
					}
				}
				else if (handPoints.size() == 20)
				{
					//�Ō�̍��ځi21�Ԗڂ�[2]�j�̓R���}���������ߓ���
					s = string.substr(offset);

					size_t c;
					while ((c = s.find_first_of("[]")) != std::string::npos)
					{
						s.erase(c, 1);
					}
				}
				int i = std::stoi(s);
				point.push_back(i);
				strings.push_back(s);

				offset = pos + delimiter_length;
			}
			printf("[0]%d [1]%d [2]%d\n", point[0], point[1], point[2]);
			//printf("[0]%s [1]%s [2]%s\n", strings[0].c_str(), strings[1].c_str(), strings[2].c_str());

			handPoints.push_back(point);
		}

	}

}

const Vector2& UDPSocket::GetPointPosition(int index)
{
	Vector2 vec;

	if (handPoints.empty())
	{
		vec.x = 0;
		vec.y = 0;
		return vec;
	}

	vec.x = handPoints[index][1];
	vec.y = handPoints[index][2];

	return vec;
}