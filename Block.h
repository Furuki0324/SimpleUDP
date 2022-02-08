#pragma once
#include "Actor.h"
#include "Collision.h"

class Block : public Actor
{
public:
	Block(class Game* game, int width, int height);
	~Block();

	void UpdateActor(float deltaTime) override;

	const std::vector<Vector2>& GetBlockCorners();

private:
	class PhysicsComponent* physicsComponent;

	float mWidth, mHeight;
	//‘±ŠÔ
	float lifeTime;
	//ƒvƒŒƒCƒ„[‚ÌŒ‚ÉÚG‚µ‚½‚©
	bool isHit;
};
