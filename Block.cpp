#include "Block.h"
#include "D2DBlockDrawComponent.h"
#include "PhysicsComponent.h"
#include "BoxComponent.h"
#include "Game.h"

Block::Block(Game* game, int width, int height)
	:Actor(game)
	,mWidth(width)
	,mHeight(height)
	,lifeTime(5.0f)
	,isHit(false)
{
	SetScale(1.0f);

	D2DBlockDrawComponent* d2d = new D2DBlockDrawComponent(this, width, height);
	physicsComponent = new PhysicsComponent(this, 101,1.0,250);

	boxComponent = new BoxComponent(this);
	Box myBox(Vector2(-width / 2, -height / 2), Vector2(width / 2, height / 2));
	boxComponent->SetObjectBox(myBox);

	game->AddBlock(this);
}

Block::~Block()
{
	mGame->RemoveBlock(this);
}

void Block::UpdateActor(float deltaTime)
{
	if (isHit) { lifeTime -= deltaTime; }
	if (lifeTime < 0.0f) { SetState(EDead); }

	Vector2 currentPos = GetPosition();
	D2D1_SIZE_F windowSize = mGame->GetRenderTarget()->GetSize();

	if(currentPos.y > windowSize.height)
	{
		physicsComponent->Bound(0.5f);
	}
	
	if (currentPos.x < 0)
	{
		physicsComponent->AddImpact(Vector2(300, 0));
	}
	else if (currentPos.x > windowSize.width)
	{
		physicsComponent->AddImpact(Vector2(-300, 0));
	}

	boxComponent->UpdateWorldBoxTransform();
}

BoxComponent* Block::GetBoxComponent()
{
	return boxComponent;
}

PhysicsComponent* Block::GetPhysicsComponent()
{
	return physicsComponent;
}

bool Block::GetHit()
{
	return isHit;
}

void Block::SetHit(bool hit)
{
	isHit = hit;
}