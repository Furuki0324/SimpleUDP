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
	physicsComponent = new PhysicsComponent(this, 101);
	physicsComponent->AddImpact(Vector2(50, -50));
}

void Block::UpdateActor(float deltaTime)
{
	Vector2 currentPos = GetPosition();
	Vector2 windowSize = mGame->GetWindowSize();

	if(currentPos.y > windowSize.y + 100)
	{
		currentPos.y = -50;
		SetPosition(currentPos);
	}
	
	if (currentPos.x < -50)
	{
		currentPos.x = windowSize.x + 50;
		SetPosition(currentPos);
	}
	else if (currentPos.x > windowSize.x + 50)
	{
		currentPos.x = -50;
		SetPosition(currentPos);
	}
}

Block::~Block()
{

}