#include "Block.h"
#include "BlockSDLComponent.h"
#include "D2DBlockDrawComponent.h"
#include "PhysicsComponent.h"
#include "Game.h"

Block::Block(Game* game, int width, int height)
	:Actor(game)
	,mWidth(width)
	,mHeight(height)
	,lifeTime(2.0f)
{
	BlockSDLComponent* sdl = new BlockSDLComponent(this, width, height);
	D2DBlockDrawComponent* d2d = new D2DBlockDrawComponent(this, width, height);
	physicsComponent = new PhysicsComponent(this, 101,1.0,150);
	physicsComponent->AddImpact(Vector2(50, 0));
}

Block::~Block()
{
	if (!isHit)
	{

	}
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

	lifeTime -= deltaTime;
	if (lifeTime < 0.0f)
	{
		SetState(EDead);
	}
}

const std::vector<Vector2>& Block::GetBlockCorners()
{
	std::vector<Vector2> vecs;
	Vector2 currentPos = GetPosition();

	vecs.push_back(Vector2(currentPos.x - mWidth / 2, currentPos.y - mHeight / 2));
	vecs.push_back(Vector2(currentPos.x + mWidth / 2, currentPos.y - mHeight / 2));
	vecs.push_back(Vector2(currentPos.x + mWidth / 2, currentPos.y + mHeight / 2));
	vecs.push_back(Vector2(currentPos.x - mWidth / 2, currentPos.y + mHeight / 2));

	return vecs;
}