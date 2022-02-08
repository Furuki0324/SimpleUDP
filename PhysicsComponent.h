#pragma once
#include "Component.h"
#include "Math.h"

//���̃R���|�[�l���g�͍Ō�ɍX�V�����悤�ɏ�����ݒ肷�邱�Ƃ𐄏����܂�
class PhysicsComponent : public Component
{
public:
	PhysicsComponent(class Actor* owner, int updateOrder = 100, float mass = 1.0, float gravity = 9.8, bool activate = true);

	void Update(float deltaTime) override;

	void Move(float deltaTime);

	//���̂ɏՌ���������
	void AddImpact(const Vector2& impact);
	//���̂̐i�s�������w�肷��
	void SetDirection(const Vector2& direction);
	void SetGravityAcceleration(const float& acceleration);
	float GetGravityAcceleration() const { return mGravityAcceleration; }
	void SetGravityState(bool state);
	bool GetGravityState() const { return mIsGravityActive; }

private:
	bool mIsGravityActive;
	//�A�N�^�[�̎���
	float mMass;
	//�A�N�^�[�̐i�s����
	Vector2 mDirection;
	//�d�͉����x
	float mGravityAcceleration;
};