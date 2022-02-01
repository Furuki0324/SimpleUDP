#include "Block.h"
#include "BlockSDLComponent.h"
#include "PhysicsComponent.h"
#include "Game.h"

Block::Block(Game* game, int width, int height)
	:Actor(game)
	,sdlComponent(nullptr)
	,physicsComponent(nullptr)
{
	sdlComponent = new BlockSDLComponent(this, width, height);
	physicsComponent = new PhysicsComponent(this, 101,1.0,150);
	physicsComponent->AddImpact(Vector2(50, 0));
}

void Block::UpdateActor(float deltaTime)
{
	Vector2 currentPos = GetPosition();
	Vector2 windowSize = mGame->GetWindowSize();

	if(currentPos.y > windowSize.y)
	{
		physicsComponent->AddImpact(Vector2(0, -300));
	}
	
	if (currentPos.x < 0)
	{
		physicsComponent->AddImpact(Vector2(300, 0));
	}
	else if (currentPos.x > windowSize.x)
	{
		physicsComponent->AddImpact(Vector2(-300, 0));
	}
}

Block::~Block()
{

}