#pragma once

#include "Actor.h"
#include "HandManager.h"
#include "Game.h"
#include "YarnSDLComponent.h"

class Yarn : public Actor
{
public:
	Yarn(Game* game, HandManager* hm);
	~Yarn();

	void UpdateActor(float deltaTime);

	HandManager* GetHandManager() const { return hm; }

private:
	YarnSDLComponent* mSDL;
	HandManager* hm;
};