#pragma once
#pragma comment(lib, "WindowsCodecs.lib")
#pragma comment(lib, "DirectXTex.lib")
#pragma comment(lib, "Dwrite.lib")

#include "Math.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <tchar.h>
#include <Windows.h>
#include <dwrite.h>
#include <d2d1.h>

class Game
{
public:
	template <class T> void SafeRelease(T** ppT);

	Game();
	bool InitializeDirect2D();
	void Shutdown();
	void RunLoop();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddBlock(class Block* block);
	void RemoveBlock(class Block* block);
	std::vector<class Block*>& GetBlocks();

	void AddD2D(class D2DDrawComponent* d2d);
	void RemoveD2D(class D2DDrawComponent* d2d);

	const ID2D1HwndRenderTarget* GetRenderTarget();
	Vector2 GetWindowSize();

	HRESULT LoadBitmapFromFile(
		ID2D1RenderTarget* pRenderTarget,
		struct IWICImagingFactory* pIWICFactory,
		PCWSTR uri,
		UINT destinationWidth,
		UINT destinationHeight,
		ID2D1Bitmap** ppBitmap
	);

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutputWithDirect2D();

	void LoadData();
	void UnloadData();

	void ShowErrorMessage(const HRESULT& err);

private:
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	std::vector<class D2DDrawComponent*> mD2Ds;
	std::vector<class Block*> mBlocks;
	bool mUpdatingActors;


	bool mIsRunning;
	float frameTime;
	float timeLimit;
	LARGE_INTEGER timeFreq;
	LARGE_INTEGER timeBefore;
	LARGE_INTEGER timeNow;
	MSG msg;

	Vector2 mWindowSize;

	//Direct2DŠÖ˜A
	struct IWICImagingFactory* pWICFactory;
	HWND hwnd;
	ID2D1Factory* pD2DFactory;
	IDWriteFactory* pWriteFactory;
	IDWriteTextFormat* pTextFormat;
	ID2D1HwndRenderTarget* pRT;
};
