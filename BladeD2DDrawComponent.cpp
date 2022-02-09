#include "BladeD2DDrawComponent.h"
#include "Blade.h"

BladeD2DDrawComponent::BladeD2DDrawComponent(Blade* owner, int drawOrder)
	:D2DDrawComponent(owner, drawOrder)
	,mBladePtr(owner)
{

}

void BladeD2DDrawComponent::Draw(ID2D1HwndRenderTarget* renderer)
{
	ID2D1SolidColorBrush* brush = nullptr;
	renderer->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::DarkRed),
		&brush
	);

	if (mBladePtr->GetIsVertical())
	{
		//Œxü‚Ì•`‰æ
		renderer->DrawLine(
			D2D1::Point2F(mOwner->GetPosition().x, 0.0f),
			D2D1::Point2F(mOwner->GetPosition().x, renderer->GetSize().height),
			brush
		);
	}
	else
	{
		//Œxü‚Ì•`‰æ
		renderer->DrawLine(
			D2D1::Point2F(0.0f, mOwner->GetPosition().y),
			D2D1::Point2F(renderer->GetSize().width, mOwner->GetPosition().y),
			brush
		);
	}
	//–{‘Ì
	renderer->FillRectangle(
		D2D1::RectF(
			mOwner->GetPosition().x - mBladePtr->GetBoxSize().x / 2,
			mOwner->GetPosition().y - mBladePtr->GetBoxSize().y / 2,
			mOwner->GetPosition().x + mBladePtr->GetBoxSize().x / 2,
			mOwner->GetPosition().y + mBladePtr->GetBoxSize().y / 2
		),
		brush
	);
}