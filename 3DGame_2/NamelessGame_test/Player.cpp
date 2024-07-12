#include "Player.h"
#include "DxLib.h"
#include "Pad.h"
#include <cassert>
#include <cmath>

namespace {
	const char* const kModelPlayer = "data/model/RogueHooded.mv1";	// モデルのファイル名

	constexpr float kInitAngle = -DX_PI_F / 2.0f * 90.0f;	// プレイヤーの初期角度*90(向きを反対にする)
	constexpr float kModelSize = 5.0f;						// モデルのサイズ
	constexpr float kSpeed = 0.5f;							// プレイヤー移動速度

	// アニメーション関係
	constexpr float kAnimChangeFrame = 8.0f;				// アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;	// アニメーション切り替えにかかる速度
	constexpr float kAnimBlendAdd = 0.5f;					// アニメーションブレンドの増加値
	constexpr float kAnimBlendMax = 1.0f;					// アニメーションブレンドの最大値
	
	// 初期化用値
	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// ベクトルの初期化
	constexpr float kInitFloat = 0.0f;				// float値の初期化
}

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player() :
	m_model		(-1),
	m_angle		(kInitFloat),
	m_isAttack	(false),
	m_isWalk	(false),
	m_currentAnimNo(-1),
	m_currentAnimCount(kInitFloat),
	m_prevAnimNo(-1),
	m_prevAnimCount(kInitFloat),
	m_animBlendRate(kAnimBlendMax),
	m_currentState(State::kIdle),
	m_pos(kInitVec),
	m_move(kInitVec)
{
	// モデル読み込み
	m_model = MV1LoadModel(kModelPlayer);
	assert(m_model != -1);

}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
}

/// <summary>
/// 初期化
/// </summary>
void Player::Init()
{
	// プレイヤー初期設定
	MV1SetScale(m_model, VGet(kModelSize, kModelSize, kModelSize));	// プレイヤーの初期サイズ
	MV1SetRotationXYZ(m_model, VGet(0.0f, kInitAngle, 0.0f));		// プレイヤーの初期角度
	MV1SetPosition(m_model, m_pos);									// プレイヤーの初期位置	
	PlayAnim(AnimKind::kIdle);										// プレイヤーの初期アニメーション
}

/// <summary>
/// 更新
/// </summary>
void Player::Update()
{
	Pad::Update();

	// プレイヤーの状態更新
	State prevState = m_currentState;
	m_currentState = MoveValue();		// 移動
	//m_currentState = Attack();		// 攻撃

	//アニメーション状態の更新
	UpdateAnimState(prevState);
	// アニメーション処理の更新
	UpdateAnim();

	// プレイヤーの座標更新
	Move(m_move);
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw()
{
	// モデルの描画
	MV1DrawModel(m_model);
}

/// <summary>
/// 終了
/// </summary>
void Player::End()
{
	// モデルの削除
	MV1DeleteModel(m_model);
}

/// <summary>
/// アニメーション状態の更新
/// </summary>
/// <param name="state">現在の状態</param>
void Player::UpdateAnimState(State state)
{
	// 待機→移動
	if (state == State::kIdle && m_currentState == State::kWalk)
	{
		PlayAnim(AnimKind::kWalk);
	}
	//// 待機→攻撃
	//if (state == State::kIdle && m_currentState == State::kAttack)
	//{
	//	PlayAnim(AnimKind::kAttack);
	//}
	// 移動→待機
	if (state == State::kWalk&& m_currentState == State::kIdle)
	{
		PlayAnim(AnimKind::kIdle);
	}
	//// 移動→攻撃
	//if (state == State::kWalk&& m_currentState == State::kAttack)
	//{
	//	PlayAnim(AnimKind::kAttack);
	//}
	//// 攻撃→待機
	//if (state == State::kAttack&& m_currentState == State::kIdle)
	//{
	//	PlayAnim(AnimKind::kIdle);
	//}
	//// 攻撃→移動
	//if (state == State::kAttack && m_currentState == State::kWalk)
	//{
	//	PlayAnim(AnimKind::kWalk);
	//}
}

/// <summary>
/// アニメーション処理
/// </summary>
void Player::UpdateAnim()
{
	float total;		// 再生中アニメーションの最大値

	// test 8フレームで切り替え
	if (m_animBlendRate < kAnimBlendMax)
	{
		m_animBlendRate += kAnimChangeRateSpeed;
		if (m_animBlendRate >= kAnimBlendMax)
		{
			m_animBlendRate = kAnimBlendMax;
		}
	}

	// 現在再生中のアニメーションの処理
	if (m_currentAnimNo != -1)
	{
		// 現在再生中のアニメーションの総カウントを取得する
		total = MV1GetAttachAnimTotalTime(m_model, m_currentAnimNo);
		// アニメーションを進行させる
		m_currentAnimCount += kAnimBlendAdd;	// アニメーションを進める

		if(m_currentAnimCount > total)
		{
			// 攻撃アニメーションが終了したら待機に移行
			if (m_isAttack)
			{
				m_isAttack = false;
				m_currentState = State::kIdle;
				PlayAnim(AnimKind::kIdle);
			}

			m_currentAnimCount = static_cast<float>(fmod(m_currentAnimCount, total));
		}

		// 進めた時間に設定
		MV1SetAttachAnimTime(m_model, m_currentAnimNo, m_currentAnimCount);
		// アニメーションのブレンド率を設定する
		MV1SetAttachAnimBlendRate(m_model, m_currentAnimNo, m_animBlendRate);
	}

	// 一つ前に再生していたアニメーションの処理
	if (m_prevAnimNo != -1)
	{
		// アニメーションの総時間獲得
		total=MV1GetAttachAnimTotalTime(m_model, m_prevAnimNo);
		
		// アニメーションを進行させる
		m_prevAnimCount += kAnimBlendAdd;

		// アニメーションの再生時間をループ
		if (m_prevAnimNo > total)
		{
			m_prevAnimCount = static_cast<float>(fmod(m_prevAnimCount, total));
		}

		// 進めた時間に設定
		MV1SetAttachAnimTime(m_model, m_prevAnimNo, m_prevAnimCount);
		// アニメーションのブレンド率を設定する
		MV1SetAttachAnimBlendRate(m_model, m_prevAnimNo, kAnimBlendMax - m_animBlendRate);
		// 変更後のアニメーション0%
		//MV1SetAttachAnimBlendRate(m_model, m_prevAnimNo, m_animBlendRate);
	}
}

/// <summary>
/// アニメーションの再生
/// </summary>
/// <param name="animIndex">再生するアニメーションの状態</param>
void Player::PlayAnim(AnimKind animIndex)
{
	// 更に古いアニメーションがアタッチされている場合はこの時点で削除しておく
	if (m_prevAnimNo != -1)
	{
		MV1DetachAnim(m_model, m_prevAnimNo);
		m_prevAnimNo = -1;
	}

	// 現在再生中の待機アニメーションは変更前のアニメーション扱いに
	m_prevAnimNo = m_currentAnimNo;
	m_prevAnimCount = m_currentAnimCount;

	// 変更後のアニメーションとして攻撃アニメーションを改めて設定する
	m_currentAnimNo = MV1AttachAnim(m_model, static_cast<int>(animIndex), -1, false);
	m_currentAnimCount = 0.0f;

	// ブレンド率はPrevが有効でない場合、1.0fにする
	if (m_prevAnimNo == 1)
	{
		m_animBlendRate = kAnimBlendMax;
	}
	else {
		// 切り替えの瞬間は変更前のアニメーションが再生される状態にする
		m_animBlendRate = 0.0f;
	}
	
}

/// <summary>
/// 移動パラメータの設定
/// </summary>
Player::State Player::MoveValue()
{
	State nextState = m_currentState;

	// 移動値を初期値に戻す
	m_move = VGet(0.0f, 0.0f, 0.0f);

	// 移動したか(true:移動した)
	bool isPressMove = false;

	if (!m_isAttack)
	{

		if (Pad::IsPress(PAD_INPUT_RIGHT))						// 右方向
		{
			m_move = VAdd(m_move, VGet(kSpeed, 0.0f, 0.0f));
			isPressMove = true;
		}
		if (Pad::IsPress(PAD_INPUT_LEFT))						// 左方向
		{
			m_move = VAdd(m_move, VGet(-kSpeed, 0.0f, 0.0f));
			isPressMove = true;
		}
		if (Pad::IsPress(PAD_INPUT_UP))							// 前方向
		{
			m_move = VAdd(m_move, VGet(0.0f, 0.0f, kSpeed));
			isPressMove = true;
		}
		if (Pad::IsPress(PAD_INPUT_DOWN))						// 後ろ方向
		{
			m_move = VAdd(m_move, VGet(0.0f, 0.0f, -kSpeed));
			isPressMove = true;
		}

		// 正規化
		if (VSize(m_move) > 0.0f)
		{
			m_move = VNorm(m_move);
			m_move = VScale(m_move, kSpeed);
		}

		if (isPressMove)
		{
			// 待機状態の場合
			if (m_currentState == State::kIdle)
			{
				nextState = State::kWalk;
			}

			
		}
		else	// 移動しない場合
		{
			if (m_currentState == State::kWalk)
			{
				// 待機状態にする
				nextState = State::kIdle;
			}
		}
	}

	return nextState;
}

/// <summary>
/// プレイヤーの移動処理
/// </summary>
/// <param name="MoveVector">移動量</param>
void Player::Move(const VECTOR& MoveVector)
{
	if (fabs(MoveVector.x) > 0.0f || fabs(MoveVector.z) > 0.0f)
	{
		m_isWalk = true;
	}
	else
	{
		m_isWalk = false;
	}

	// プレイヤーの位置に移動量を足す
	m_pos = VAdd(m_pos, m_move);

	// プレイヤーの位置セット
	MV1SetPosition(m_model, m_pos);
}

/// <summary>
/// プレイヤーの攻撃処理
/// </summary>
Player::State Player::Attack()
{
	return State();
}
