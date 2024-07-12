#include "Player.h"
#include "DxLib.h"
#include "Pad.h"
#include <cassert>

namespace {
	const char* const kModelPlayer = "data/model/RogueHooded.mv1";	// モデルのファイル名

	constexpr float kInitAngle = -DX_PI_F / 2.0f * 90.0f;	// プレイヤーの初期角度*90(向きを反対にする)
	constexpr float kModelSize = 5.0f;						// モデルのサイズ
	constexpr float kSpeed = 0.5f;							// 移動速度

	// アニメーション番号
	constexpr int kIdleAnimIndex = 1;		// 待機
	constexpr int kWalkAnimIndex = 3;		// 歩く
	constexpr int kAttackAnimIndex = 30;	// 攻撃

	// アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeFrame = 8.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;
	
	const VECTOR kInit = VGet(0.0f, 0.0f, 0.0f);	// ベクトルの初期化
}

Player::Player() :
	m_model(-1),
	m_prevAnimNo(-1),
	m_currentAnimNo(-1),
	m_animBlemdRate(0.0f),
	m_pos(kInit),
	m_move(kInit),
	m_isAttack(false),
	m_isWalk(false)
{
	// モデル読み込み
	m_model = MV1LoadModel(kModelPlayer);
	assert(m_model != -1);

}

Player::~Player()
{
}

void Player::Init()
{
	// 待機アニメーションを設定
	m_currentAnimNo = MV1AttachAnim(m_model, kIdleAnimIndex, -1, false);
	m_prevAnimNo = -1;
	m_animBlemdRate = 1.0f;

	// プレイヤー初期設定
	MV1SetScale(m_model, VGet(kModelSize, kModelSize, kModelSize));	// プレイヤーの初期サイズ
	MV1SetRotationXYZ(m_model, VGet(0.0f, kInitAngle, 0.0f));		// プレイヤーの初期角度
	MV1SetPosition(m_model, m_pos);									// プレイヤーの初期位置
}

void Player::Update()
{
	Pad::Update();

	// アニメーションの切り替え
	if (m_prevAnimNo != -1)
	{
		// test 8フレームで切り替え
		m_animBlemdRate += kAnimChangeRateSpeed;
		if (m_animBlemdRate >= 1.0f) m_animBlemdRate = 1.0f;
		// 変更後のアニメーション割合を設定する
		MV1SetAttachAnimBlendRate(m_model, m_prevAnimNo, 1.0f - m_animBlemdRate);
		MV1SetAttachAnimBlendRate(m_model, m_currentAnimNo, m_animBlemdRate);
	}

	// アニメーションを進める
	bool isLoop = UpdateAnim(m_currentAnimNo);
	UpdateAnim(m_prevAnimNo);

	// 攻撃アニメーション処理
	if(!m_isAttack)
	{
		if (Pad::IsPress(PAD_INPUT_X))
		{
			m_isAttack = true;
			ChangeAnim(kAttackAnimIndex);
		}
		else {
			// 移動処理
			Move();
			/*if(m_isWalk)
			{
				ChangeAnim(kWalkAnimIndex);
			}
			if(isLoop){
				m_isWalk = false;
				ChangeAnim(kIdleAnimIndex);
			}*/
		}
	}
	else
	{
		// 攻撃アニメーションが終了したら待機アニメーションを再生する
		if (isLoop)
		{
			m_isAttack = false;
			ChangeAnim(kIdleAnimIndex);
		}
	}
	
	// プレイヤーの位置セット
	MV1SetPosition(m_model, m_pos);
}

void Player::Draw()
{
	// モデルの描画
	MV1DrawModel(m_model);

	DrawFormatString(0, 40, 0xffffff, "m_isWalk=%d", m_isWalk);

}

void Player::End()
{
	// モデルの削除
	MV1DeleteModel(m_model);
}

void Player::UpdateAnimState(State state)
{
}

void Player::UpdateAnim()
{
}

void Player::PlayAnim(AnimKind animIndex)
{
}

void Player::Move()
{
	// 移動値を初期値に戻す
	m_move = VGet(0.0f, 0.0f, 0.0f);

	if (Pad::IsPress(PAD_INPUT_RIGHT))						// 右方向
	{
		m_move = VAdd(m_move, VGet(kSpeed, 0.0f, 0.0f));
	}
	if (Pad::IsPress(PAD_INPUT_LEFT))						// 左方向
	{
		m_move = VAdd(m_move, VGet(-kSpeed, 0.0f, 0.0f));
	}
	if (Pad::IsPress(PAD_INPUT_UP))							// 前方向
	{
		m_move = VAdd(m_move, VGet(0.0f, 0.0f, kSpeed));
	}
	if (Pad::IsPress(PAD_INPUT_DOWN))						// 後ろ方向
	{
		m_move = VAdd(m_move, VGet(0.0f, 0.0f, -kSpeed));
	}

	// 正規化
	if (VSize(m_move) > 0.0f)
	{
		m_move = VNorm(m_move);
		m_move = VScale(m_move, kSpeed);
	}

	if (m_move.x != kInit.x || m_move.z != kInit.z)
	{
		m_isWalk = true;
	}

	// プレイヤーの位置に移動量を足す
	m_pos = VAdd(m_pos, m_move);
}

bool Player::UpdateAnim(int attachNo)
{
	// アニメーションが設定されていないので終了
	if (attachNo == -1) return false;

	// アニメーションを進行させる
	float now = MV1GetAttachAnimTime(m_model, attachNo);	// 現在の再生カウントを取得
	now += 0.5f;	// アニメーションを進める

	// 現在再生中のアニメーションの総カウントを取得する
	float total = MV1GetAttachAnimTotalTime(m_model, attachNo);
	bool isLoop = false;
	while (now >= total)
	{
		now -= total;
		isLoop = true;
	}
	// 進めた時間に設定
	MV1SetAttachAnimTime(m_model, attachNo, now);

	return isLoop;
}

void Player::ChangeAnim(int animIndex)
{
	// 更に古いアニメーションがアタッチされている場合はこの時点で削除しておく
	if (m_prevAnimNo != -1)
	{
		MV1DetachAnim(m_model, m_prevAnimNo);
	}

	// 現在再生中の待機アニメーションは変更前のアニメーション扱いに
	m_prevAnimNo = m_currentAnimNo;

	// 変更後のアニメーションとして攻撃アニメーションを改めて設定する
	m_currentAnimNo = MV1AttachAnim(m_model, animIndex, -1, false);

	// 切り替えの瞬間は変更前のアニメーションが再生される状態にする
	m_animBlemdRate = 0.0f;

	// 変更前のアニメーション100%
	MV1SetAttachAnimBlendRate(m_model, m_prevAnimNo, 1.0f - m_animBlemdRate);
	// 変更後のアニメーション0%
	MV1SetAttachAnimBlendRate(m_model, m_currentAnimNo, m_animBlemdRate);
}
