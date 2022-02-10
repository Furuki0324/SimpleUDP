#pragma once
#pragma comment(lib, "WindowsCodecs.lib")
#pragma comment(lib, "Xaudio2.lib")

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
	
	enum Scene{Title, MainGame, Result};

	Game();
	bool InitializeDirect2D();
	void Shutdown();
	void RunLoop();
	void SetScene(Scene newScene);
	Scene GetScene() const;
	int GetTimeLimit();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddBlock(class Block* block);
	void RemoveBlock(class Block* block);
	std::vector<class Block*>& GetBlocks();

	void AddBlade(class Blade* blade);
	void RemoveBlade(class Blade* blade);
	std::vector<class Blade*>& GetBlades();

	void AddD2D(class D2DDrawComponent* d2d);
	void RemoveD2D(class D2DDrawComponent* d2d);

	void AddUI(class UIScreen* ui);
	void RemoveUI(class UIScreen* ui);

	void AddScore(const int score);
	const int GetScore();

	const ID2D1HwndRenderTarget* GetRenderTarget();
	void PlayAudio(LPCTSTR wavFilePath, UINT32 loop = 0, bool isBGM = false);

	HRESULT LoadBitmapFromFile(
		ID2D1RenderTarget* pRenderTarget,
		struct IWICImagingFactory* pIWICFactory,
		PCWSTR uri,
		UINT destinationWidth,
		UINT destinationHeight,
		ID2D1Bitmap** ppBitmap
	);

	HRESULT FindChunk(
		HANDLE hFile,
		DWORD fourcc,
		DWORD& dwChunkSize,
		DWORD& d2ChunkDataPosition
	);

	HRESULT ReadChunkData(
		HANDLE hFile,
		void* buffer,
		DWORD bufferSize,
		DWORD bufferoffset
	);

	void ShowErrorMessage(const HRESULT& err);

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutputWithDirect2D();

	void LoadData();
	void UnloadData();

private:
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	std::vector<class D2DDrawComponent*> mD2Ds;
	std::vector<class Block*> mBlocks;
	std::vector<class Blade*> mBlades;
	std::vector<class UIScreen*> mUIs;
	struct IXAudio2SourceVoice* mBGMSource;
	bool mUpdatingActors;

	Scene currentScene;
	int score;
	bool mIsRunning;
	float frameTime;
	float timeLimit;
	LARGE_INTEGER timeFreq;
	LARGE_INTEGER timeBefore;
	LARGE_INTEGER timeNow;
	MSG msg;

	//Direct2DŠÖ˜A
	struct IWICImagingFactory* pWICFactory;
	HWND hwnd;
	ID2D1Factory* pD2DFactory;
	IDWriteFactory* pWriteFactory;
	ID2D1HwndRenderTarget* pRT;
	struct IXAudio2* pXAudio2;
	struct IXAudio2MasteringVoice* pMasterVoice;
};
