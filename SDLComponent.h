#pragma once

#include <SDL.h>
#include "Component.h"
#include "Actor.h"
#include "Game.h"

class SDLComponent : public Component
{
public:
	SDLComponent(class Actor* owner, int drawOrder = 100);
	~SDLComponent();

	virtual void Draw(SDL_Renderer* renderer);

	int GetDrawOrder() const { return mDrawOrder; }

private:
	int mDrawOrder;
};