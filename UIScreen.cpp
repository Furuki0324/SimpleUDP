#include "UIScreen.h"
#include "Game.h"	//this include d2d1.h and dwrite.h

UIScreen::UIScreen(Game* game)
	:mGame(game)
{
	game->AddUI(this);
}

UIScreen::~UIScreen()
{
	mGame->RemoveUI(this);
}

void UIScreen::Update(float deltaTime)
{

}

void UIScreen::Draw(ID2D1HwndRenderTarget* renderer, IDWriteFactory* factory)
{

}