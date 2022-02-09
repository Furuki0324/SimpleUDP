#include "D2DBlockDrawComponent.h"
#include "Actor.h"
#include "Block.h"

D2DBlockDrawComponent::D2DBlockDrawComponent(Block* owner, int width, int height, int drawOrder)
	:D2DDrawComponent(owner)
	,mWidth(width)
	,mHeight(height)
	,m_pOwnerBlock(owner)
{

}

void D2DBlockDrawComponent::Draw(ID2D1HwndRenderTarget* renderer)
{
	Vector2 position = mOwner->GetPosition();
	bool isHit = m_pOwnerBlock->GetHit();

	ID2D1SolidColorBrush* brush = nullptr;

	if (isHit)
	{
		renderer->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Blue),
			&brush
		);
	}
	else
	{
		renderer->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Red),
			&brush
		);
	}


	D2D1_SIZE_F rtSize = renderer->GetSize();

	renderer->FillRectangle(
		D2D1::RectF(
			position.x - mWidth / 2,
			position.y - mHeight / 2,
			position.x + mWidth / 2,
			position.y + mHeight / 2
		),
		brush
	);
}