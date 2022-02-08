#include "CircleComponent.h"
#include "Actor.h"

CircleComponent::CircleComponent(Actor* owner, int drawOrder)
	:Component(owner, drawOrder)
	,mCircle(0.0f)
	,mWorldCircle(0.0f)
{

}

void CircleComponent::SetCircle(const Circle& circle)
{
	mCircle = circle;
}

const Circle& CircleComponent::GetWorldCircle()
{
	return mWorldCircle;
}

void CircleComponent::UpdateWorldCircleTransform()
{
	mWorldCircle = mCircle;

	mWorldCircle.mRadius *= mOwner->GetScale();

	mWorldCircle.mCenter += mOwner->GetPosition();
}

void CircleComponent::UpdateWorldCircleTransform(const Vector2& pos)
{
	mWorldCircle = mCircle;
	mWorldCircle.mRadius *= mOwner->GetScale();
	mWorldCircle.mCenter += pos;
}