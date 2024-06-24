#include "Player.h"
#include "DxLib.h"
#include "Stage.h"
#include <vector>
#include <memory>

namespace {
	const char* const kModelPlayer = "data/model/Player.mv1";
	constexpr float kColSize = 3.5f;	// 当たり判定の幅(正方形)	ベクトルなら4
}

Player::Player() :
	m_speed(0.5f),
	m_playerScele(3.0f),
	m_pos(VGet(0.0f, 1.0f, 0.0f)),
	m_move(VGet(0.0f, 0.0f, 0.0f)),
	m_playerModel(MV1LoadModel(kModelPlayer))// プレイヤーのロード
{
	MV1SetScale(m_playerModel, VGet(m_playerScele, m_playerScele, m_playerScele));
	MV1SetPosition(m_playerModel, m_pos);
}

Player::~Player()
{
}

void Player::Init(std::shared_ptr<Stage> pStage)
{
	sp.leftUp = pStage->GetStageUpperLeft();
	sp.rightDown = pStage->GetStageLowerRight();
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

	// プレイヤーの位置に移動量を足す
	m_pos = VAdd(m_pos, m_move);


	// プレイヤーが画面外に出ないようする処理
	if (m_pos.x - 2 < sp.leftUp.x)
	{
		m_pos.x -=m_move.x ;		// 左
	}
	if (m_pos.x + 2 > sp.rightDown.x)
	{
		m_pos.x -= m_move.x;		// 右
	}
	if (m_pos.z + 2 > sp.rightDown.z)
	{
		m_pos.z -= m_move.z;		// 上
	}
	if (m_pos.z - 2 < sp.leftUp.z)
	{
		m_pos.z -= m_move.z;		// 下
	}
	

	// 当たり判定の更新
	m_colRect.SetLB(m_pos, kColSize, kColSize);
	// プレイヤーの位置セット
	MV1SetPosition(m_playerModel, m_pos);	
}

void Player::Draw()
{
	// 当たり判定描画
	m_colRect.PlayerDraw(0x000000, true);
	// プレイヤー描画
	MV1DrawModel(m_playerModel);

	DrawFormatString(0, 430, 0xffffff, "m_pos.x=%.0f:.z=%.0f", m_pos.x, m_pos.z);
}

void Player::End()
{
	// プレイヤーのアンロード
	MV1DeleteModel(m_playerModel);	
	m_playerModel = -1;
}
