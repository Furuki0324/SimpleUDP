#pragma once
#include "Actor.h"
#include "Collision.h"

class Block : public Actor
{
public:
	Block(class Game* game, int width, int height);
	~Block();

	void UpdateActor(float deltaTime) override;

private:
	class BlockSDLComponent* sdlComponent;
	class PhysicsComponent* physicsComponent;
};
