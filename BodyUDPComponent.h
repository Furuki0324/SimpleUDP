#pragma once
#include "UDPSocket.h"
#include "SharedStruct.h"
//#include "Math.h"
//#include <vector>

class BodyUDPComponent : public UDPSocket
{
public:
	BodyUDPComponent(class Actor* owner, u_short port, int updateOrder = 1);

	void UDP_Receive() override;
	Vector2 GetPointPosition(int index);
	const std::vector<std::vector<float>>& GetAllPosition();
	const std::vector<MediaPipeData>& GetMediaPipeData();

private:
	//[0] = ID, [1] = X, [2] = Y, [3] = Z
	std::vector<std::vector<float>> bodyPositions;
	std::vector<MediaPipeData> mpData;
};