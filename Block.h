#pragma once
#include "Actor.h"

class Block : public Actor
{
public:
	Block(class Game* game);
	~Block();

	void UpdateActor(float deltaTime);
};
