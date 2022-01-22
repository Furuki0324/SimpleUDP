#include "Hand.h"
#include "Game.h"

Hand::Hand(Game* game, int inPort, bool right)
	:Actor(game)
	, hsc(nullptr)
	, socket(nullptr)
{
	hsc = new HandSpriteComponent(this);
	std::vector<SDL_Texture*> texs;
	if (right)
	{
		texs = {
		game->GetTexture("Asset/OpenHand_R.png"),
		game->GetTexture("Asset/CloseHand_R.png")
		};
	}
	else
	{
		texs = {
			game->GetTexture("Asset/OpenHand.png"),
			game->GetTexture("Asset/CloseHand.png")
		};
	}

	hsc->SetHandTextures(texs);
	
	socket = new UDPSocket(this, inPort);
}

Hand::~Hand()
{}

UDPSocket* Hand::GetUDPSocket()
{
	return socket;
}

void Hand::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	//UDPSocketから手首の座標を取得。もし手を一度も検出していなかったら（起動時など）座標0を取得します。
	Vector2 wristPos = socket->GetPointPosition(0);
	SetPosition(wristPos);

	Vector2 thumbPos = socket->GetPointPosition(4);
	Vector2 indexPos = socket->GetPointPosition(8);
	float TILength = (thumbPos - indexPos).Length();

	bool closed = TILength < 20.0f;
	hsc->SwitchHandSprite(closed);
}