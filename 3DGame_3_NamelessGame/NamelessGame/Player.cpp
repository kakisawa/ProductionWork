#include "Player.h"

Player::Player() :
	model(-1)
{
	model = MV1LoadModel("Data/Model/PlayerModel.mv1");
}

Player::~Player()
{
	MV1DeleteModel(model);
}

void Player::Init()
{
	pos = VGet(300.0f, 250.0f, -100.0f);
	MV1SetPosition(model, pos);
	MV1SetScale(model, VGet(0.5f, 0.5f, 0.5f));
}

void Player::Update()
{
}

void Player::Draw()
{
	MV1DrawModel(model);
}
