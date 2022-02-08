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
	//���������DirectX2D�̋N��
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
	* ID2DFactory�̐���
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
	* IDWriteFactory�̐���
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
	* DirectWrite text format�̐���
	*/
	hr = pWriteFactory->CreateTextFormat(
		L"F66�M�߂�����",
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
	* ID2D1HwndRenderTarget�̐���
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
	* IWICImagingFactory�̐���
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
	//�w�肵���t���[�����ɓ��B����܂őҋ@
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	//�S�ẴA�N�^�[���X�V
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;


	//�ۗ����̃A�N�^�[��mActors�Ɉړ��B���̍X�V�^�C�~���O�ōX�V�����悤�ɂ���
	for (auto pending : mPendingActors)
	{
		mActors.push_back(pending);
	}
	mPendingActors.clear();	//�ۗ����̂��̂�S��mActors�Ɉړ����������ߓ��e���N���A

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
	//�w�i�F��ݒ�
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	SDL_RenderClear(mRenderer);

	//�o�^�ς݂̃X�v���C�g���X�V���ĕ`��
	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}

	//SDL�̕`�揈��
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
	std::wstring text(L"��������");
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
		//�����̃A�N�^�[���A�b�v�f�[�g���I������܂őҋ@��ɒǉ�
		mPendingActors.push_back(actor);
	}
	else
	{
		mActors.push_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	//�ŏ��ɑҋ@��ɑΏۂ̃A�N�^�[�����Ȃ�������
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		//���X�g�̍Ō�Ɉړ�
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	//���ɃA�b�v�f�[�g�Ώۂ̗�ɋ��Ȃ�������
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		//���X�g�̍Ō�Ɉړ�
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

/*
* �X�v���C�g��ǉ��BSpriteComponent�̃R���X�g���N�^�\������s�����B
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
* �X�v���C�g���폜�BSpriteComponent�̃f�X�g���N�^�[������s�����B
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
* SDL��ǉ�����BSDLComponent�̃R���X�g���N�^�\������s�����B
*/
void Game::AddSDL(SDLComponent* sdl)
{
	mSDLs.push_back(sdl);
}

/*
* SDL���폜����BSDLComponent�̃f�X�g���N�^�[������s�����B
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

//�N�����ɕK�v�ȃI�u�W�F�N�g��z�u
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