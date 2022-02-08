#pragma once
#include "Actor.h"

class Block : public Actor
{
public:
	Block(class Game* game, int width, int height);
	~Block();

	void UpdateActor(float deltaTime) override;
	class BoxComponent* GetBoxComponent();
	class PhysicsComponent* GetPhysicsComponent();

	bool GetHit();
	void SetHit(bool hit);

private:
	class PhysicsComponent* physicsComponent;
	class BoxComponent* boxComponent;

	float mWidth, mHeight;
	//��������
	float lifeTime;
	//�v���C���[�̌��ɐڐG������
	bool isHit;
};
