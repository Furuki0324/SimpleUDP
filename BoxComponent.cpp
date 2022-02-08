#include "BoxComponent.h"
#include "Actor.h"

BoxComponent::BoxComponent(Actor* owner, int updateOrder)
	:Component(owner, updateOrder)
	,mBox(Vector2::Zero, Vector2::Zero)
	,mWorldBox(Vector2::Zero, Vector2::Zero)
{

}

void BoxComponent::SetObjectBox(const Box& box)
{
	mBox = box;
}

const Box& BoxComponent::GetWorldBox()
{
	return mWorldBox;
}

void BoxComponent::UpdateWorldBoxTransform()
{
	mWorldBox = mBox;

	//�X�P�[���ϊ�
	mWorldBox.mMin *= mOwner->GetScale();
	mWorldBox.mMax *= mOwner->GetScale();

	//���W�ϊ�
	mWorldBox.mMin += mOwner->GetPosition();
	mWorldBox.mMax += mOwner->GetPosition();
}