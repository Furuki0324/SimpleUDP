#pragma once
#include "Component.h"
//#include <vector>
//#include <d2d1.h>

class D2DDrawComponent : public Component
{
public:
	D2DDrawComponent(class Actor* owner, int drawOrder = 100);
	~D2DDrawComponent();

	virtual void Draw(ID2D1HwndRenderTarget* target);

	int GetDrawOrder() const { return mDrawOrder; }

private:
	int mDrawOrder;
};