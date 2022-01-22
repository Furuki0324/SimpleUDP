#include "Game.h"
#include "Actor.h"
#include "Hand.h"
#include "Yarn.h"
#include "SpriteComponent.h"
#include "SDL_image.h"
#include <iostream>

Game::Game()
	:mWindow(nullptr)
	,mRenderer(nullptr)
	,mIsRunning(true)
	,mUpdatingActors(false)
{

}

bool Game::Initialize()
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
		640,
		480,
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

	//int iResult;
	//iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	//if (iResult != 0)
	//{
	//	std::cout << "Failed to WSAStartup: " << iResult << std::endl;
	//	return false;
	//}

	LoadData();

	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::Shutdown()
{
	IMG_Quit();

	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
	std::cout << "Shutdown" << std::endl;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
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
	if (state[SDL_SCANCODE_ESCAPE])
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

	SDL_RenderPresent(mRenderer);
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

void Game::LoadData()
{
	mRightHand = new Hand(this, 50001, true);
	mRightHand->SetPosition(Vector2(500.0f, 200.0f));

	mLeftHand = new Hand(this, 50002, false);
	mLeftHand->SetPosition(Vector2(140.0f, 200.0f));

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