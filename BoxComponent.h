#pragma once
#include "Collision.h"
#include "Component.h"

class BoxComponent : public Component
{
public:
	BoxComponent(class Actor* owner, int updateOrder = 100);

	void SetObjectBox(const Box& box);
	const Box& GetWorldBox();

	void UpdateWorldBoxTransform();

private:
	Box mBox;
	Box mWorldBox;
};