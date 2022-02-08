#pragma once
#include "Component.h"
#include "Math.h"

//このコンポーネントは最後に更新されるように順序を設定することを推奨します
class PhysicsComponent : public Component
{
public:
	PhysicsComponent(class Actor* owner, int updateOrder = 100, float mass = 1.0, float gravity = 9.8, bool activate = true);

	void Update(float deltaTime) override;

	void Move(float deltaTime);

	//物体に衝撃を加える
	void AddImpact(const Vector2& impact);
	//物体の進行方向を指定する
	void SetDirection(const Vector2& direction);
	void SetGravityAcceleration(const float& acceleration);
	float GetGravityAcceleration() const { return mGravityAcceleration; }
	void SetGravityState(bool state);
	bool GetGravityState() const { return mIsGravityActive; }

private:
	bool mIsGravityActive;
	//アクターの質量
	float mMass;
	//アクターの進行方向
	Vector2 mDirection;
	//重力加速度
	float mGravityAcceleration;
};