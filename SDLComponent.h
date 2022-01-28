#pragma once

#include <SDL.h>
#include "Component.h"
#include "Actor.h"
#include "Game.h"

class SDLComponent : public Component
{
public:
	SDLComponent(class Actor* owner, class Yarn* yarn, int drawOrder = 100);

	void Draw(SDL_Renderer* renderer);
	void DrawLine(SDL_Renderer* renderer, float x1, float y1, float x2, float y2);
	void DrawLine(SDL_Renderer* renderer, std::vector<float> handPoint);

	int GetDrawOrder() const { return mDrawOrder; }

private:
	int mDrawOrder;
	class Yarn* mYarnPtr;
};