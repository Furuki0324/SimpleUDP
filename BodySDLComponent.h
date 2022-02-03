#pragma once
#include "SDLComponent.h"

class BodySDLComponent : public SDLComponent
{
public:
	BodySDLComponent(class Body* owner, class Game* game, int drawOrder = 100);
	void Draw(SDL_Renderer* renderer) override;
	void DrawLine(SDL_Renderer* renderer, std::vector<std::vector<float>> position);

private:
	class Body* mBodyPtr;
	std::vector<std::vector<float>> bodyPositions;
	class Game* mGame;
};