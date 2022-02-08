#pragma once
#pragma comment(lib, "WindowsCodecs.lib")
#pragma comment(lib, "DirectXTex.lib")
#pragma comment(lib, "Dwrite.lib")

#include "Math.h"
#include <SDL.h>
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
	bool InitializeSDL();
	bool InitializeDirect2D();
	void Shutdown();
	void RunLoop();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	void AddSDL(class SDLComponent* sdl);
	void RemoveSDL(class SDLComponent* sdl);

	void AddD2D(class D2DDrawComponent* d2d);
	void RemoveD2D(class D2DDrawComponent* d2d);

	SDL_Texture* GetTexture(const std::string& fileName);

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
	void GenerateOutput();
	void GenerateOutputWithDirect2D();

	void LoadData();
	void UnloadData();

private:
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	std::vector<class SpriteComponent*> mSprites;
	std::vector<class SDLComponent*> mSDLs;
	std::vector<class D2DDrawComponent*> mD2Ds;
	bool mUpdatingActors;

	std::unordered_map<std::string, SDL_Texture*> mTextures;

	Uint32 mTicksCount;
	bool mIsRunning;
	float frameTime;
	float fps;
	LARGE_INTEGER timeFreq;
	LARGE_INTEGER timeBefore;
	LARGE_INTEGER timeNow;

	//SDLŠÖ˜A
	SDL_Window* mWindow;
	Vector2 mWindowSize;
	int mWindowWidth;
	int mWindowHeight;
	SDL_Renderer* mRenderer;

	//Direct2DŠÖ˜A
	WNDCLASSEX windowClass;
	HWND hwnd;
	ID2D1Factory* pD2DFactory;
	struct IWICImagingFactory* pWICFactory;
	IDWriteFactory* pWriteFactory;
	IDWriteTextFormat* pTextFormat;
	ID2D1HwndRenderTarget* pRT;
};
