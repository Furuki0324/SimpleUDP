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
	switch (mGame->GetScene())
	{
	case Game::Scene::Title:
		TitleDraw(renderer, factory);
		break;

	case Game::Scene::MainGame:
		MainGameDraw(renderer, factory);
		break;

	case Game::Scene::Result:
		ResultDraw(renderer, factory);
		break;
	}
}

void HUD::TitleDraw(ID2D1HwndRenderTarget* renderer, IDWriteFactory* factory)
{
	std::wstring text = L"拳を突き合わせて開始";

	ID2D1SolidColorBrush* brush = nullptr;
	HRESULT hr = renderer->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black),
		&brush
	);
	if (FAILED(hr))
	{
		mGame->ShowErrorMessage(hr);
		return;
	}

	IDWriteTextFormat* format = nullptr;
	hr = factory->CreateTextFormat(
		L"F66筆めいげつ",
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		60.0f,
		L"ja-jp",
		&format
	);
	if (FAILED(hr))
	{
		mGame->ShowErrorMessage(hr);
		return;
	}

	hr = format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	if (FAILED(hr))
	{
		mGame->ShowErrorMessage(hr);
		return;
	}
	hr = format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	if (FAILED(hr))
	{
		mGame->ShowErrorMessage(hr);
		return;
	}

	renderer->DrawTextW(
		text.c_str(),
		text.size(),
		format,
		D2D1::RectF(0.0f, 550.0f, renderer->GetSize().width, renderer->GetSize().height),
		brush,
		D2D1_DRAW_TEXT_OPTIONS_NO_SNAP
	);
}

void HUD::MainGameDraw(ID2D1HwndRenderTarget* renderer, IDWriteFactory* factory)
{
	int score = mGame->GetScore();
	std::wstring text = L"得点:" + std::to_wstring(score);
	int time = mGame->GetTimeLimit();

	ID2D1SolidColorBrush* brush = nullptr;
	renderer->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black),
		&brush
	);

	IDWriteTextFormat* format = nullptr;
	HRESULT hr = factory->CreateTextFormat(
		L"F66筆めいげつ",
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

	text = L"残り" + std::to_wstring(time) + L"秒";
	renderer->DrawTextW(
		text.c_str(),
		text.size(),
		format,
		D2D1::RectF(50.0f, 100.0f, 350.0f, 125.0f),
		brush
	);
}

void HUD::ResultDraw(ID2D1HwndRenderTarget* renderer, IDWriteFactory* factory)
{
	std::wstring text = L"最終結果";

	ID2D1SolidColorBrush* brush = nullptr;
	renderer->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black),
		&brush
	);

	IDWriteTextFormat* format = nullptr;
	HRESULT hr = factory->CreateTextFormat(
		L"F66筆めいげつ",
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

	format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	renderer->DrawTextW(
		text.c_str(),
		text.size(),
		format,
		D2D1::RectF(0.0f, renderer->GetSize().height * 0.45f, renderer->GetSize().width, renderer->GetSize().height * 0.45f),
		brush
	);

	text = L"拳を突き合わせて再開";
	renderer->DrawTextW(
		text.c_str(),
		text.size(),
		format,
		D2D1::RectF(0, renderer->GetSize().height * 0.8f, renderer->GetSize().width, renderer->GetSize().height * 0.8f),
		brush
	);
	text = L"Escキーで終了";
	renderer->DrawTextW(
		text.c_str(),
		text.size(),
		format,
		D2D1::RectF(0, renderer->GetSize().height * 0.9f, renderer->GetSize().width, renderer->GetSize().height * 0.9f),
		brush
	);

	//スコアを表示する前にフォントサイズを変更
	text = std::to_wstring(mGame->GetScore());
	format = nullptr;
	hr = factory->CreateTextFormat(
		L"F66筆めいげつ",
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		100.0f,
		L"ja-jp",
		&format
	);
	if (FAILED(hr))
	{
		printf("CreateTextFormat was failed in HUD.\n");
		mGame->ShowErrorMessage(hr);
		return;
	}

	format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	renderer->DrawTextW(
		text.c_str(),
		text.size(),
		format,
		D2D1::RectF(0.0f, renderer->GetSize().height * 0.55f, renderer->GetSize().width, renderer->GetSize().height * 0.55f),
		brush
	);
}