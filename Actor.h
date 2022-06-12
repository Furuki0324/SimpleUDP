#pragma once
//#include <vector>
//#include "Math.h"

class Actor
{
public:
	enum State
	{
		EActive,
		EPaused,
		EDead
	};

	struct Location 
	{
		float X;
		float Y;
	};

	Actor(class Game* game);
	virtual ~Actor();

	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	virtual void UpdateActor(float deltaTime);

	const Vector2& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	float GetScale() const { return mScale; }
	void SetScale(const float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(const float rot) { mRotation = rot; }

	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() { return mGame; }

	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

protected:
	class Game* mGame;

private:
	State mState;
	Vector2 mPosition;
	float mRotation;
	float mScale;
	std::vector<class Component*> mComponents;
};