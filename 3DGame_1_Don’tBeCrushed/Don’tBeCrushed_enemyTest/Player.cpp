#include "Player.h"
#include "DxLib.h"

namespace {
	const char* const kModelPlayer = "data/model/Player.mv1";
}

Player::Player() :
	m_speed(0.5f),
	m_playerScele(3.0f),
	m_pos(VGet(0.0f, 1.0f, 0.0f)),
	m_move(VGet(0.0f, 0.0f, 0.0f)),
	m_playerModel(MV1LoadModel(kModelPlayer))		// プレイヤーのロード
{
	MV1SetScale(m_playerModel, VGet(m_playerScele, m_playerScele, m_playerScele));
	MV1SetPosition(m_playerModel, m_pos);
}

Player::~Player()
{
}

void Player::Init()
{
}

void Player::Update()
{
	// 移動量を初期値に戻す
	m_move = VGet(0, 0, 0);

	int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// 移動処理
	if (pad & PAD_INPUT_RIGHT)
	{
		m_move = VAdd(m_move, VGet(m_speed, 0.0f, 0.0f));
	}
	if (pad & PAD_INPUT_LEFT)
	{
		m_move = VAdd(m_move, VGet(-m_speed, 0.0f, 0.0f));
	}
	if (pad & PAD_INPUT_UP)
	{
		m_move = VAdd(m_move, VGet(0.0f, 0.0f, m_speed));
	}
	if (pad & PAD_INPUT_DOWN)
	{
		m_move = VAdd(m_move, VGet(0.0f, 0.0f, -m_speed));
	}

	// 正規化
	if (VSize(m_move) > 0.0f)
	{
		m_move = VNorm(m_move);
		m_move = VScale(m_move, m_speed);
	}

	m_pos = VAdd(m_pos, m_move);			// プレイヤーの位置に移動量を足す

	MV1SetPosition(m_playerModel, m_pos);	// プレイヤーの位置セット
}

void Player::Draw()
{
	MV1DrawModel(m_playerModel);	// プレイヤー描画
}

void Player::End()
{
	MV1DeleteModel(m_playerModel);	// プレイヤーのアンロード
	m_playerModel = -1;
}
