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

	//UDPSocket������̍��W���擾�B���������x�����o���Ă��Ȃ�������i�N�����Ȃǁj���W0���擾���܂��B
	Vector2 wristPos = socket->GetPointPosition(0);
	SetPosition(wristPos);

	//�e�w�Ɛl�����w�̋������v���B���l�ȉ��Ȃ畨���܂ގ��̌`�����Ă���Ɣ��f���܂��B
	Vector2 thumbPos = socket->GetPointPosition(4);
	Vector2 indexPos = socket->GetPointPosition(8);
	float TILength = (thumbPos - indexPos).Length();

	isClosed = TILength < 20.0f;
	hsc->SwitchHandSprite(isClosed);
}