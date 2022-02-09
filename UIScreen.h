#pragma once


class UIScreen
{
public:
	UIScreen(class Game* game);
	~UIScreen();

	virtual void Update(float deltaTime);
	virtual void Draw(struct ID2D1HwndRenderTarget* renderer, struct IDWriteFactory* factory);

protected:
	class Game* mGame;
};