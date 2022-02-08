#pragma once
#include "D2DDrawComponent.h"
#include "Math.h"

class BodyD2DDrawComponent : public D2DDrawComponent
{
public:
	BodyD2DDrawComponent(class Body* owner, int drawOrder = 100);
	void Draw(ID2D1HwndRenderTarget* renderer) override;
	void DrawLine(ID2D1HwndRenderTarget* renderer);

private:
	std::vector<std::vector<float>> bodyPositions;
	class BodyUDPComponent* mBodyUDP;

	Vector2 rightHandPosition, leftHandPosition;
	Vector2 rightHandVector, leftHandVector;
};