#pragma once
#include "Actor.h"

class Blade : public Actor
{
public:
	Blade(class Game* game);
	~Blade();
	void UpdateActor(float deltaTime) override;
	class BoxComponent* GetBoxComponent();
	class PhysicsComponent* GetPhysicsComponent();

	const Vector2& GetBoxSize();
	void SetBoxSize(const Vector2& size);

	bool GetHit();
	void SetHit(bool hit);

	bool GetIsVertical();
	void SetIsVertical(bool vertical);

protected:
	Vector2 boxSize;
	class BoxComponent* boxComponent;
	class PhysicsComponent* physicsComponent;
	float lifeTime;
	bool isHit;
	bool isVertical;
};

class HorizontalBlade : public Blade
{
public:
	HorizontalBlade(class Game* game, float scale = 1.0f);
};

class VerticalBlade : public Blade
{
public:
	VerticalBlade(class Game* game, float scale = 1.0f);
};