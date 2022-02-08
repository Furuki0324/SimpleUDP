#include "Game.h"
#include "Actor.h"
#include "Hand.h"
#include "Body.h"
#include "Block.h"
#include "D2DSampleBox.h"
#include "SpriteComponent.h"
#include "D2DDrawComponent.h"
#include "SDLComponent.h"
#include "SDL_image.h"
#include <iostream>
#include <wincodec.h>

const unsigned int window_width = 640;
const unsigned int window_height = 480;

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

template <class T> void Game::SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = nullptr;
	}
}

Game::Game()
	:mWindow(nullptr)
	,mRenderer(nullptr)
	,pD2DFactory(nullptr)
	,mIsRunning(true)
	,mUpdatingActors(false)
{
	mWindowSize.x = window_width;
	mWindowSize.y = window_height;
}

bool Game::InitializeSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Failed to initialize IMG: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow(
		"MediaPipe Simple Game",
		100,
		100,
		mWindowSize.x,
		mWindowSize.y,
		0
	);

	if (mWindow == nullptr)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED
	);

	if (mRenderer == nullptr)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	mTicksCount = SDL_GetTicks();

	return true;
}

bool Game::InitializeDirect2D()
{
	//ここからはDirectX2Dの起動
	HINSTANCE hInst = GetModuleHandle(nullptr);
	WNDCLASSEX w = {};
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)MainWndProc;
	w.lpszClassName = _T("Direct2D");
	w.hInstance = GetModuleHandle(0);
	RegisterClassEx(&w);

	RECT wrc = { 0,0,window_width,window_height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	hwnd = CreateWindow(
		w.lpszClassName,
		_T("Direct2D Test"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom,
		nullptr,
		nullptr,
		w.hInstance,
		nullptr);

	/*
	* ID2DFactoryの生成
	*/
	HRESULT hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&pD2DFactory
	);
	if (FAILED(hr))
	{
		printf("D2D1CreateFactory was failed.\n");
		return false;
	}

	/*
	* IDWriteFactoryの生成
	*/
	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(pWriteFactory),
		reinterpret_cast<IUnknown**>(&pWriteFactory)
	);
	if (FAILED(hr))
	{
		printf("DWriteCreateFactory was failed.\n");
		return false;
	}

	/*
	* DirectWrite text formatの生成
	*/
	hr = pWriteFactory->CreateTextFormat(
		L"F66筆めいげつ",
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		24.0f,
		L"ja-jp",
		&pTextFormat
	);
	if (FAILED(hr))
	{
		printf("CreateTextFormat was failed.\n");
		return false;
	}

	pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	/*
	* ID2D1HwndRenderTargetの生成
	*/
	hr = pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			hwnd,
			D2D1::SizeU(
				wrc.right - wrc.left,
				wrc.bottom - wrc.top)
		),
		&pRT
	);
	if (FAILED(hr))
	{
		printf("CreateHwndRenderTarget was failed.\n");
		return false;
	}

	/*
	* IWICImagingFactoryの生成
	*/
	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<void**>(&pWICFactory));
	if (FAILED(hr))
	{
		printf("CoCreateInstance was failed.\n");
		return false;
	}

	return true;
}

void Game::Shutdown()
{
	IMG_Quit();

	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();

	SafeRelease(&pRT);
	SafeRelease(&pD2DFactory);

	std::cout << "Shutdown" << std::endl;
}

void Game::RunLoop()
{
	LoadData();
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		//GenerateOutput();
		GenerateOutputWithDirect2D();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	const bool escapeState = GetKeyState(VK_ESCAPE) & 0x8000;
	if (state[SDL_SCANCODE_ESCAPE] || escapeState)
	{
		mIsRunning = false;
	}
}

void Game::UpdateGame()
{
	//指定したフレーム数に到達するまで待機
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	//全てのアクターを更新
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;


	//保留中のアクターをmActorsに移動。次の更新タイミングで更新されるようにする
	for (auto pending : mPendingActors)
	{
		mActors.push_back(pending);
	}
	mPendingActors.clear();	//保留中のものを全てmActorsに移動させたため内容をクリア

	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.push_back(actor);
		}
	}

	for (auto actor : deadActors)
	{
		delete actor;
	}
}

void Game::GenerateOutput()
{
	//背景色を設定
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	SDL_RenderClear(mRenderer);

	//登録済みのスプライトを更新して描画
	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}

	//SDLの描画処理
	for (auto sdl : mSDLs)
	{
		sdl->Draw(mRenderer);
	}

	SDL_RenderPresent(mRenderer);
}

void Game::GenerateOutputWithDirect2D()
{
	pRT->BeginDraw();

	pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));
	for (auto d2d : mD2Ds)
	{
		d2d->Draw(pRT);
	}

	ID2D1SolidColorBrush* brush = nullptr;
	pRT->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black),
		&brush
	);
	std::wstring text(L"試し書き");
	pRT->DrawTextW(
		text.c_str(),
		text.size(),
		pTextFormat,
		D2D1::RectF(100.0f, 100.0f, 400.0f, 400.0f),
		brush
	);
	
	pRT->EndDraw();
	ShowWindow(hwnd, SW_SHOW);

	SafeRelease(&brush);
}

void Game::AddActor(Actor* actor)
{
	if (mUpdatingActors)
	{
		//既存のアクターがアップデートを終了するまで待機列に追加
		mPendingActors.push_back(actor);
	}
	else
	{
		mActors.push_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	//最初に待機列に対象のアクターが居ないか検索
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		//リストの最後に移動
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	//次にアップデート対象の列に居ないか検索
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		//リストの最後に移動
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

/*
* スプライトを追加。SpriteComponentのコンストラクタ―から実行される。
*/
void Game::AddSprite(SpriteComponent* sprite)
{
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();

	for (; iter != mSprites.end(); ++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	mSprites.insert(iter, sprite);
}

/*
* スプライトを削除。SpriteComponentのデストラクターから実行される。
*/
void Game::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;
	auto iter = mTextures.find(fileName);

	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log("Failed to load texture file: %s", fileName.c_str());
			return nullptr;
		}

		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return nullptr;
		}

		mTextures.emplace(fileName.c_str(), tex);
	}

	return tex;
}

/*
* SDLを追加する。SDLComponentのコンストラクタ―から実行される。
*/
void Game::AddSDL(SDLComponent* sdl)
{
	mSDLs.push_back(sdl);
}

/*
* SDLを削除する。SDLComponentのデストラクターから実行される。
*/
void Game::RemoveSDL(SDLComponent* sdl)
{
	auto iter = std::find(mSDLs.begin(), mSDLs.end(), sdl);
	mSDLs.erase(iter);
}

void Game::AddD2D(D2DDrawComponent* d2d)
{
	int myDrawOrder = d2d->GetDrawOrder();
	auto iter = mD2Ds.begin();

	for (; iter != mD2Ds.end(); ++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	mD2Ds.insert(iter, d2d);
}

void Game::RemoveD2D(D2DDrawComponent* d2d)
{
	auto iter = std::find(mD2Ds.begin(), mD2Ds.end(), d2d);
	mD2Ds.erase(iter);
}

//起動時に必要なオブジェクトを配置
void Game::LoadData()
{
	Actor* a = nullptr;
	/*mRightHand = new Hand(this, 50001, true);
	mRightHand->SetPosition(Vector2(500.0f, 200.0f));

	mLeftHand = new Hand(this, 50002, false);
	mLeftHand->SetPosition(Vector2(140.0f, 200.0f));

	mHandManager = new HandManager(this, mRightHand, mLeftHand);*/

	a = new Body(this, 50001);

	a = new Block(this, 100, 20);
	a->SetPosition(Vector2(0, 0));
}

void Game::UnloadData()
{
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	for (auto i : mTextures)
	{
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();
}

Vector2 Game::GetWindowSize()
{
	Vector2 size;
	size.x = window_width;
	size.y = window_height;
	return size;
}

HRESULT Game::LoadBitmapFromFile(
	ID2D1RenderTarget* pRenderTarget,
	IWICImagingFactory* pIWICFactory,
	PCWSTR uri,
	UINT destinationWidth,
	UINT destinationHeight,
	ID2D1Bitmap** ppBitmap
)
{
	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pSource = NULL;
	IWICStream* pStream = NULL;
	IWICFormatConverter* pConverter = NULL;
	IWICBitmapScaler* pScaler = NULL;

	HRESULT hr = pWICFactory->CreateDecoderFromFilename(
		uri,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);

	if (SUCCEEDED(hr))
	{
		hr = pDecoder->GetFrame(0, &pSource);

		if (SUCCEEDED(hr))
		{
			hr = pWICFactory->CreateFormatConverter(&pConverter);
		}

		if (SUCCEEDED(hr))
		{
			hr = pConverter->Initialize(
				pSource,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeMedianCut
			);
		}

		if (SUCCEEDED(hr))
		{
			hr = pRT->CreateBitmapFromWicBitmap(
				pConverter,
				NULL,
				ppBitmap
			);
		}
	}

	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pStream);
	SafeRelease(&pConverter);
	SafeRelease(&pScaler);

	return hr;
}