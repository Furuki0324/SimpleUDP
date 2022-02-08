#include "Game.h"
#include "Actor.h"
#include "Body.h"
#include "Block.h"
#include "GimmickGenerator.h"
#include "D2DSampleBox.h"
#include "D2DDrawComponent.h"
#include <iostream>
#include <wincodec.h>

const unsigned int window_width = 960;
const unsigned int window_height = 720;
const float MIN_FRAME_TIME = 1.0f / 144;

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
	:pD2DFactory(nullptr)
	,mIsRunning(true)
	,mUpdatingActors(false)
	,frameTime(0.0f)
	,timeLimit(10.0f)
{
}

void Game::Shutdown()
{
	SafeRelease(&pD2DFactory);
	SafeRelease(&pRT);
	SafeRelease(&pWriteFactory);
	SafeRelease(&pTextFormat);

	std::cout << "Shutdown" << std::endl;
}

bool Game::InitializeDirect2D()
{
	//�t���[�����[�g�Ǘ��p�̕ϐ����擾
	BOOL query = QueryPerformanceFrequency(&timeFreq);
	QueryPerformanceCounter(&timeBefore);

	//���������DirectX2D�̋N��
	HRESULT hr = CoInitialize(NULL);

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
	hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&pD2DFactory
	);
	if (FAILED(hr))
	{
		printf("D2D1CreateFactory was failed.\n");
		ShowErrorMessage(hr);
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
		ShowErrorMessage(hr);
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
		ShowErrorMessage(hr);
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
		ShowErrorMessage(hr);
		return false;
	}

	/*
	* IWICImagingFactory�̐���
	*/
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pWICFactory)
	);
	if (FAILED(hr))
	{
		printf("CoCreateInstance was failed.\n");
		ShowErrorMessage(hr);
		return false;
	}

	CoUninitialize();

	return true;
}



void Game::RunLoop()
{
	LoadData();
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutputWithDirect2D();
	}
}

void Game::ProcessInput()
{
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			printf("WM_QUIT\n");
			mIsRunning = false;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	const bool escapeState = GetKeyState(VK_ESCAPE) & 0x8000;
	if (escapeState)
	{
		mIsRunning = false;
	}
}

void Game::UpdateGame()
{
	//�w�肵���t���[�����ɓ��B����܂őҋ@
	do
	{
		QueryPerformanceCounter(&timeNow);
		frameTime = (float)(timeNow.QuadPart - timeBefore.QuadPart) / (float)timeFreq.QuadPart;
	} while (frameTime < MIN_FRAME_TIME);


	float deltaTime = frameTime;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	timeBefore = timeNow;
	timeLimit += deltaTime;

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

	std::wstring text = std::to_wstring(timeLimit);

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

void Game::AddBlock(Block* block)
{
	mBlocks.push_back(block);
}

void Game::RemoveBlock(Block* block)
{
	auto iter = std::find(mBlocks.begin(), mBlocks.end(), block);
	mBlocks.erase(iter);
}

std::vector<Block*>& Game::GetBlocks()
{
	return mBlocks;
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

	//a = new Body(this, 50001);
	a = new GimmickGenerator(this);

	a = new Block(this, 100, 20);
	a->SetPosition(Vector2(0, 0));
}

void Game::UnloadData()
{
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	while (!mBlocks.empty())
	{
		delete mBlocks.back();
	}
}

Vector2 Game::GetWindowSize()
{
	Vector2 size;
	size.x = window_width;
	size.y = window_height;
	return size;
}

const ID2D1HwndRenderTarget* Game::GetRenderTarget()
{
	return pRT;
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
	}

	if (SUCCEEDED(hr))
	{
		SafeRelease(&pConverter);
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

	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pStream);
	SafeRelease(&pConverter);
	SafeRelease(&pScaler);

	return hr;
}

void Game::ShowErrorMessage(const HRESULT& err)
{
	std::string text = std::system_category().message(err);
	printf("%s\n", text.c_str());
}