#pragma once
#include "D2DDrawComponent.h"

class BladeD2DDrawComponent : public D2DDrawComponent
{
public:
	BladeD2DDrawComponent(class Blade* owner, int drawOrder = 100);
	void Draw(ID2D1HwndRenderTarget* renderer) override;
	
private:
	class Blade* mBladePtr;
};