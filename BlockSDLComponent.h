#pragma once
#include "SDLComponent.h"

class BlockSDLComponent : public SDLComponent
{
public:
	BlockSDLComponent(class Block* owner, float width, float height, int drawOrder = 100);

	void Draw(SDL_Renderer* renderer) override;

private:
	float mWidth;
	float mHeight;
};