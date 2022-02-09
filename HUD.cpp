#include "HUD.h"
#include "Game.h"	//this include d2d1.h and dwrite.h

HUD::HUD(Game* game)
	:UIScreen(game)
{

}

void HUD::Update(float deltaTime)
{

}

void HUD::Draw(ID2D1HwndRenderTarget* renderer, IDWriteFactory* factory)
{
	int score = mGame->GetScore();
	std::wstring text = L"“¾“_:" + std::to_wstring(score);
	//int time;

	ID2D1SolidColorBrush* brush = nullptr;
	renderer->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black),
		&brush
	);

	IDWriteTextFormat* format = nullptr;
	HRESULT hr = factory->CreateTextFormat(
		L"F66•M‚ß‚¢‚°‚Â",
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		40.0f,
		L"ja-jp",
		&format
	);
	if (FAILED(hr))
	{
		printf("CreateTextFormat was failed in HUD.\n");
		mGame->ShowErrorMessage(hr);
		return;
	}

	format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	renderer->DrawTextW(
		text.c_str(),
		text.size(),
		format,
		D2D1::RectF(50.0f, 50.0f, 350.0f, 75.0f),
		brush
	);

	mGame->SafeRelease(&brush);
	mGame->SafeRelease(&format);
}