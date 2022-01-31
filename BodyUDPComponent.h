#pragma once
#include "UDPSocket.h"
#include "Math.h"
#include <vector>

class BodyUDPComponent : public UDPSocket
{
public:
	BodyUDPComponent(class Actor* owner, u_short port);

	void UDP_Receive() override;
	const Vector2 GetPointPosition(int index);
	std::vector<std::vector<float>> GetAllPosition();

private:
	std::vector<std::vector<float>> bodyPositions;
};