#pragma once
#include "SDLComponent.h"

class YarnSDLComponent : public SDLComponent
{
public:
	YarnSDLComponent(class Yarn* owner, int drawOrder = 100);
	void Draw(SDL_Renderer* renderer) override;
	void DrawLine(SDL_Renderer* renderer, std::vector<float> position);

private:
	class Yarn* mYarnPtr;
};