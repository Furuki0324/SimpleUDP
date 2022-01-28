#pragma once
#include "UDPSocket.h"
#include "Math.h"
#include <vector>

class HandUDPComponent : public UDPSocket
{
public:
	HandUDPComponent(class Actor* owner, u_short port);
	//~HandUDPComponent();

	void UDP_Receive() override;
	const Vector2 GetPointPosition(int index);

protected:

private:
	std::vector<std::vector<float>> handPositions;
};