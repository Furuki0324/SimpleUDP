#include "D2DDrawComponent.h"
#include "Actor.h"
#include "Game.h"

D2DDrawComponent::D2DDrawComponent(Actor* owner, int drawOrder)
	:Component(owner)
	,mDrawOrder(drawOrder)
{
	mOwner->GetGame()->AddD2D(this);
}

D2DDrawComponent::~D2DDrawComponent()
{
	mOwner->GetGame()->RemoveD2D(this);
}

void D2DDrawComponent::Draw(ID2D1HwndRenderTarget* renderer)
{
	ID2D1SolidColorBrush* pBlackBrush = NULL;

	renderer->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black),
		&pBlackBrush
	);

	D2D1_SIZE_F rtSize = renderer->GetSize();

	renderer->DrawRectangle(
		D2D1::RectF(
			0.0f,
			rtSize.height / 2 - 50.0f,
			rtSize.width / 2 + 50.0f,
			rtSize.height / 2 + 50.0f
		),
		pBlackBrush
	);
}