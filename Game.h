#pragma once
#include <SDL.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <Windows.h>

class Game
{
public:
	Game();
	bool Initialize();
	void Shutdown();
	void RunLoop();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	void AddSDL(class SDLComponent* sdl);
	void RemoveSDL(class SDLComponent* sdl);

	SDL_Texture* GetTexture(const std::string& fileName);

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	void LoadData();
	void UnloadData();

private:
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	std::vector<class SpriteComponent*> mSprites;
	std::vector<class SDLComponent*> mSDLs;
	bool mUpdatingActors;

	std::unordered_map<std::string, SDL_Texture*> mTextures;

	class Hand* mRightHand;
	class Hand* mLeftHand;
	class HandManager* mHandManager;

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;

	WSADATA wsaData;
};
