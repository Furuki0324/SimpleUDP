#include "PhysicsComponent.h"
#include "Actor.h"

PhysicsComponent::PhysicsComponent(Actor* owner, int updateOrder, float mass, float gravity, bool activate)
	:Component(owner, updateOrder)
	,mMass(mass)
	,mIsGravityActive(activate)
	,mGravityAcceleration(gravity)
{

}

void PhysicsComponent::Update(float deltaTime)
{
	Move(deltaTime);
}

void PhysicsComponent::SetGravityAcceleration(float acceleration)
{
	mGravityAcceleration = acceleration;
}

void PhysicsComponent::SetGravityState(bool state)
{
	mIsGravityActive = state;
}

void PhysicsComponent::AddImpact(const Vector2& impact)
{
	mDirection += impact;
}

void PhysicsComponent::SetDirection(const Vector2& direction)
{
	mDirection = direction;
}


void PhysicsComponent::Move(float deltaTime)
{
	Vector2 ownerPos = mOwner->GetPosition();

	if (mIsGravityActive) mDirection.y += mGravityAcceleration * deltaTime;

	ownerPos.x += mDirection.x * deltaTime;
	ownerPos.y += mDirection.y * deltaTime;

	mOwner->SetPosition(ownerPos);
}