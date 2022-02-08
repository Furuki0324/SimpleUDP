#pragma once
#include "Collision.h"
#include "Component.h"

class CircleComponent : public Component
{
public:
	CircleComponent(class Actor* owner, int drawOrder = 100);

	void SetCircle(const Circle& circle);
	const Circle& GetWorldCircle();

	void UpdateWorldCircleTransform();
	void UpdateWorldCircleTransform(const Vector2& pos);

private:
	Circle mCircle;
	Circle mWorldCircle;
};