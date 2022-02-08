#pragma once
#include "D2DDrawComponent.h"

class D2DBlockDrawComponent : public D2DDrawComponent
{
public:
	D2DBlockDrawComponent(Actor* owner, int width, int height, int drawOrder = 100);

	void Draw(ID2D1HwndRenderTarget* renderer) override;

private:
	int mWidth, mHeight;
};