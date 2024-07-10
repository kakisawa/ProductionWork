#include "Player.h"
#include "DxLib.h"
#include <cassert>

namespace {
	const char* const kModelPlayer = "data/model/RogueHooded.mv1";

	constexpr float kInitAngle = -DX_PI_F / 2.0f;	// プレイヤーの初期角度
	constexpr float kModelSize = 5.0f;
}

Player::Player() :
	m_model(-1),
	m_pos(VGet(0.0f, 0.0f, 0.0f))
{
	m_model = MV1LoadModel("data/model/RogueHooded.mv1");
	assert(m_model != -1);

	MV1SetScale(m_model, VGet(kModelSize, kModelSize, kModelSize));
	MV1SetRotationXYZ(m_model, VGet(0.0f,90* kInitAngle,0.0f));
	MV1SetPosition(m_model, m_pos);
}

Player::~Player()
{
}

void Player::Init()
{
}

void Player::Update()
{
	// プレイヤーの位置セット
	MV1SetPosition(m_model, m_pos);
}

void Player::Draw()
{
	MV1DrawModel(m_model);
}

void Player::End()
{
	MV1DeleteModel(m_model);
}
