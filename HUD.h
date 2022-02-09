#pragma once
#include "UIScreen.h"

class HUD : public UIScreen
{
public:
	HUD(class Game* game);
	void Update(float deltaTime) override;
	void Draw(struct ID2D1HwndRenderTarget* renderer, struct IDWriteFactory* factory) override;
};