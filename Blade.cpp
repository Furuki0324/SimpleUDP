#include "Blade.h"
#include "BladeD2DDrawComponent.h"
#include "BoxComponent.h"
#include "PhysicsComponent.h"
#include "Game.h"

Blade::Blade(Game* game)
	:Actor(game)
	,lifeTime(3.0f)
	,isHit(false)
	,isVertical(false)
{
	BladeD2DDrawComponent* d2d = new BladeD2DDrawComponent(this);
	boxComponent = new BoxComponent(this);
	physicsComponent = new PhysicsComponent(this, 100, 1.0f, 9.8f, false);

	game->AddBlade(this);
}

Blade::~Blade()
{
	if (!isHit)
	{
		mGame->AddScore(200);
	}

	mGame->RemoveBlade(this);
}

void Blade::UpdateActor(float deltaTime)
{
	lifeTime -= deltaTime;
	if (lifeTime < 0.0f)
	{
		SetState(EDead);
	}

	boxComponent->UpdateWorldBoxTransform();
}

const Vector2& Blade::GetBoxSize()
{
	return boxSize;
}

void Blade::SetBoxSize(const Vector2& size)
{
	boxSize = size;
}

bool Blade::GetHit()
{
	return isHit;
}

void Blade::SetHit(bool hit)
{
	if (!isHit && hit)
	{
		mGame->PlayAudio(_T("Media\\–‚‰¤° í“¬17.wav"));
		mGame->AddScore(-100);
		isHit = hit;
	}
}

bool Blade::GetIsVertical()
{
	return isVertical;
}

void Blade::SetIsVertical(bool vertical)
{
	isVertical = vertical;
}

BoxComponent* Blade::GetBoxComponent()
{
	return boxComponent;
}

PhysicsComponent* Blade::GetPhysicsComponent()
{
	return physicsComponent;
}

HorizontalBlade::HorizontalBlade(Game* game, float scale)
	:Blade(game)
{
	SetBoxSize(Vector2(100.0f, 10.0f));
	SetIsVertical(false);
	SetScale(scale);
	Box myBox(Vector2(-GetBoxSize().x / 2, -GetBoxSize().y / 2), Vector2(GetBoxSize().x / 2, GetBoxSize().y / 2));
	boxComponent->SetObjectBox(myBox);
}

VerticalBlade::VerticalBlade(Game* game, float scale)
	:Blade(game)
{
	SetBoxSize(Vector2(10.0f, 100.0f));
	SetIsVertical(true);
	SetScale(scale);
	Box myBox(Vector2(-GetBoxSize().x / 2, -GetBoxSize().y / 2), Vector2(GetBoxSize().x / 2, GetBoxSize().y / 2));
	boxComponent->SetObjectBox(myBox);
}