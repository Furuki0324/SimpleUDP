#include "Game.h"
#include "Actor.h"
#include "Body.h"
#include "Block.h"
#include "Blade.h"
#include "GimmickGenerator.h"
#include "D2DSampleBox.h"
#include "D2DDrawComponent.h"
#include "UIScreen.h"
#include "HUD.h"
#include <iostream>
#include <wincodec.h>
#include <xaudio2.h>

#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

const unsigned int window_width = 960;
const unsigned int window_height = 720;
const float MIN_FRAME_TIME = 1.0f / 60;		//60FPS
const float TIME_LIMIT = 65.0f;				//��������

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

HRESULT Game::FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) return S_FALSE;

	}

	return S_OK;
}

HRESULT Game::ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}

Game::Game()
	:pD2DFactory(nullptr)
	,mIsRunning(true)
	,mUpdatingActors(false)
	,currentScene(Title)
	,score(0)
	,frameTime(0.0f)
	,timeLimit(TIME_LIMIT)
{
}

void Game::Shutdown()
{
	UnloadData();

	SafeRelease(&pD2DFactory);
	SafeRelease(&pRT);
	SafeRelease(&pWriteFactory);

	std::cout << "Shutdown" << std::endl;
}

bool Game::InitializeDirect2D()
{
	//�t���[�����[�g�Ǘ��p�̕ϐ����擾
	BOOL query = QueryPerformanceFrequency(&timeFreq);
	QueryPerformanceCounter(&timeBefore);

	//���������DirectX2D�̋N��
	//HRESULT hr = CoInitialize(NULL);
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		printf("CoInitializeEx was failed.\n");
		ShowErrorMessage(hr);
		return false;
	}

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

	/*
	* IXAudio2�̐���
	*/
	hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if (FAILED(hr))
	{
		printf("XAudio2Create was failed.\n");
		ShowErrorMessage(hr);
		return false;
	}

	/*
	* IXAudio2MasteringVoice�̐���
	*/
	hr = pXAudio2->CreateMasteringVoice(&pMasterVoice);
	if (FAILED(hr))
	{
		printf("CreateMasteringVoice was failed.");
		ShowErrorMessage(hr);
		return false;
	}

	CoUninitialize();

	return true;
}

void Game::PlayAudio(LPCTSTR wavFilePath, UINT32 loop, bool isBGM)
{
	HRESULT hr = S_OK;

	WAVEFORMATEXTENSIBLE wfx = { 0 };
	XAUDIO2_BUFFER buffer = { 0 };

	HANDLE hFile = CreateFile(
		wavFilePath,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		ShowErrorMessage(hr);
	}
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		ShowErrorMessage(hr);
	}

	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
	DWORD filetype;
	ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
	if (filetype != fourccWAVE)
	{
		printf("INVALID FILE TYPE\n");
	}

	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	BYTE* pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

	buffer.AudioBytes = dwChunkSize;
	buffer.pAudioData = pDataBuffer;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = loop;

	IXAudio2SourceVoice* pSourceVoice;
	hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx);
	if (FAILED(hr))
	{
		printf("CreateSourceVoice was failed.\n");
		ShowErrorMessage(hr);
	}

	hr = pSourceVoice->SubmitSourceBuffer(&buffer);
	if (FAILED(hr))
	{
		printf("SubmitSourceBuffer was failed.\n");
		ShowErrorMessage(hr);
	}

	hr = pSourceVoice->Start(0);
	if (FAILED(hr))
	{
		printf("Start was failed.\n");
		ShowErrorMessage(hr);
	}

	//�g�����킩��Ȃ�����ǁABGM�̃|�C���^�[�͊m�ۂ��Ă���
	if (isBGM)
	{
		mBGMSource = pSourceVoice;
	}
}

void Game::RunLoop()
{
	PlayAudio(_T("Media\\������ �t�B�[���h01.wav"), XAUDIO2_LOOP_INFINITE, true);
	LoadData();
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutputWithDirect2D();
	}
}

void Game::SetScene(Scene newScene)
{
	currentScene = newScene;
	switch (currentScene)
	{
	case Title:
		break;

	case MainGame:
		score = 0;
		timeLimit = TIME_LIMIT;
		break;
		
	case Result:
		PlayAudio(_T("Media\\������ ���ʉ� �����|�C���g12.wav"));
		break;
	}
}

Game::Scene Game::GetScene() const
{
	return currentScene;
}

int Game::GetTimeLimit()
{
	return timeLimit;
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
	//�w�肵��FPS���ێ�����
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

	if (currentScene == MainGame)
	{
		timeLimit -= deltaTime;
		if (timeLimit < 0.0f)
		{
			SetScene(Result);
		}
	}

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

	for (auto ui : mUIs)
	{
		ui->Update(deltaTime);
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

	for (auto ui : mUIs)
	{
		ui->Draw(pRT, pWriteFactory);
	}

	pRT->EndDraw();
	ShowWindow(hwnd, SW_SHOW);
}

void Game::AddScore(const int value)
{
	score += value;

	if (score < 0) { score = 0; }
}

const int Game::GetScore()
{
	return score;
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

void Game::AddBlade(Blade* blade)
{
	mBlades.push_back(blade);
}

void Game::RemoveBlade(Blade* blade)
{
	auto iter = std::find(mBlades.begin(), mBlades.end(), blade);
	mBlades.erase(iter);
}

std::vector<Blade*>& Game::GetBlades()
{
	return mBlades;
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

void Game::AddUI(UIScreen* ui)
{
	mUIs.push_back(ui);
}

void Game::RemoveUI(UIScreen* ui)
{
	auto iter = std::find(mUIs.begin(), mUIs.end(), ui);
	mUIs.erase(iter);
}

//�N�����ɕK�v�ȃI�u�W�F�N�g��z�u
void Game::LoadData()
{
	Actor* a = nullptr;
	UIScreen* ui = nullptr;

	a = new Body(this, 50001);
	a = new GimmickGenerator(this);

	ui = new HUD(this);
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

	while (!mUIs.empty())
	{
		delete mUIs.back();
	}
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