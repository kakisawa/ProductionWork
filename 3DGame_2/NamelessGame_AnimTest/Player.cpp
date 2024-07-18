#include "Player.h"
#include "DxLib.h"
#include "PlayerState.h"
#include "Pad.h"
#include "Camera.h"
#include "Time.h"
#include <cassert>
#include <cmath>

// 07/16:連続攻撃処理中

namespace {
	const char* const kModelPlayer = "data/model/RogueHooded.mv1";	// モデルのファイル名

	constexpr float kInitAngle = -DX_PI_F / 2.0f * 90.0f;	// プレイヤーの初期角度*90(向きを反対にする)
	constexpr float kModelSize = 5.0f;			// モデルのサイズ
	constexpr float kSpeed = 0.7f;				// プレイヤー移動速度
	constexpr float kAttackSpeed = 0.5f;		// プレイヤー攻撃時の加速度
	constexpr float	kAngleSpeed = 0.2f;			// 角度変化速度
	constexpr float	kJumpPower = 1.8f;			// ジャンプ力
	constexpr float	kGravity = 0.05f;			// 重力

	// アニメーション関係
	constexpr float kAnimChangeFrame = 8.0f;		// アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeRateSpeed = 0.2f;	// アニメーション切り替えにかかる速度
	constexpr float kAnimBlendAdd = 0.5f;			// アニメーションブレンドの増加値
	constexpr float kAnimBlendMax = 1.0f;			// アニメーションブレンドの最大値

	// 初期化用値
	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// ベクトルの初期化
	constexpr float kInitFloat = 0.0f;				// float値の初期化
}

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player() :
	m_model(-1),
	m_angle(kInitFloat),
	m_gravity(kGravity),
	m_isAttack(false),
	m_nextAttackFlag(false),
	m_isWalk(false),
	m_isJump(false),
	//m_isForward(false),
	m_animBlendRate(kAnimBlendMax),
	m_currentJumpPower(0.0f),
	m_multiAttack(0),
	m_currentState(State::kIdle),
	m_pos(kInitVec),
	m_move(kInitVec),
	m_targetDir(VGet(0.0f, 0.0f, 0.0f)),
	m_animSpeed(0),
	m_animChangeFrameTotal(0),
	m_pAttackStanTime(std::make_shared<Time>(6))
{
	// モデル読み込み
	m_model = MV1LoadModel(kModelPlayer);
	assert(m_model != -1);


	// アニメーション状態の初期化
	InitAnim(m_current);
	InitAnim(m_prev);
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
	//PlayAnim(AnimKind::kIdle);									// プレイヤーの初期アニメーション
	SetAnim(m_animData.kIdle, false, true);
}

/// <summary>
/// 更新
/// </summary>
void Player::Update(const Camera& camera)
{
	Pad::Update();

	// パッド入力によって移動パラメータを設定する
	VECTOR	upMoveVec;		// 方向ボタン「↑」を入力をしたときのプレイヤーの移動方向ベクトル
	VECTOR	leftMoveVec;	// 方向ボタン「←」を入力をしたときのプレイヤーの移動方向ベクトル

	// プレイヤーの状態更新
	State prevState = m_currentState;
	m_currentState = JumpState();
	m_currentState = AttackState();		// 攻撃
	
	// 本来モデルクラス内に入ってる
	{
		m_animSpeed++;
		if (m_animSpeed >= m_animChangeFrameTotal)
		{
			// アニメーション関係(調整中)
			UpdateAnimation(m_prev, kAnimBlendAdd);
			UpdateAnimation(m_current, kAnimBlendAdd);
			m_animSpeed = 0;
		}

		m_animChangeFrame++;
		if (m_animChangeFrame > m_animChangeFrameTotal)
		{
			m_animChangeFrame = m_animChangeFrameTotal;
		}
		// アニメーションのブレンド率を設定
		UpdateAnimBlendRate();
	}


	

	// 攻撃処理
	Attack();

	m_currentState = MoveValue(camera, upMoveVec, leftMoveVec);		// 移動

	//アニメーション状態の更新
	//UpdateAnimState(prevState);

	// プレイヤーの移動方向にモデルの方向を近づける
	Angle();

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
/// (一応)自分で書いたやつ
//void Player::UpdateAnimState(State state)
{
	// 待機→移動
	if (state == State::kIdle && m_currentState == State::kWalk)
	{
		PlayAnim(AnimKind::kWalk);
	}
	// 待機→攻撃
	if (state == State::kIdle && m_currentState == State::kAttack)
	{
		PlayAnim(AnimKind::kAttack1);
	}
	// 待機→ジャンプ
	if (state == State::kIdle && m_currentState == State::kJump)
	{
		PlayAnim(AnimKind::kJump);
	}

	// 移動→待機
	if (state == State::kWalk && m_currentState == State::kIdle)
	{
		PlayAnim(AnimKind::kIdle);
	}
	// 移動→攻撃
	if (state == State::kWalk && m_currentState == State::kAttack)
	{
		PlayAnim(AnimKind::kAttack1);
	}
	// 移動→ジャンプ
	if (state == State::kWalk && m_currentState == State::kJump)
	{
		PlayAnim(AnimKind::kJump);
	}

	// 攻撃→待機
	if (state == State::kAttack && m_currentState == State::kIdle)
	{
		PlayAnim(AnimKind::kIdle);
	}
	// 攻撃→移動
	if (state == State::kAttack && m_currentState == State::kWalk)
	{
		PlayAnim(AnimKind::kWalk);
	}
	// 攻撃→ジャンプ
	if (state == State::kAttack && m_currentState == State::kJump)
	{
		PlayAnim(AnimKind::kJump);
	}

	// ジャンプ→移動
	if (state == State::kJump && m_currentState == State::kWalk)
	{
		PlayAnim(AnimKind::kWalk);
	}
}

/// <summary>
/// アニメーション処理
/// </summary>
/// (一応)自分で書いたやつ
void Player::UpdateAnim()
{
	//float total;		// 再生中アニメーションの最大値

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
	if (m_current.animNo != -1)
	{
		// 現在再生中のアニメーションの総カウントを取得する
		m_current.totalTime = MV1GetAttachAnimTotalTime(m_model, m_current.animNo);
		// アニメーションを進行させる
		m_current.elapsedTime += kAnimBlendAdd;	// アニメーションを進める

		if (m_current.elapsedTime > m_current.totalTime)
		{
			// 攻撃アニメーションが終了したら待機に移行
			if (m_isAttack)
			{
				m_isAttack = false;
				m_currentState = State::kIdle;
				PlayAnim(AnimKind::kIdle);
			}

			// ジャンプアニメーションが終了したら待機に移行
			if (!m_isJump)
			{
				m_currentState = State::kIdle;
				PlayAnim(AnimKind::kIdle);
			}

			m_current.elapsedTime = static_cast<float>(fmod(m_current.elapsedTime, m_current.totalTime));
		}

		// 進めた時間に設定
		MV1SetAttachAnimTime(m_model, m_current.animNo, m_current.elapsedTime);
		// アニメーションのブレンド率を設定する
		MV1SetAttachAnimBlendRate(m_model, m_current.animNo, m_animBlendRate);
	}

	// 一つ前に再生していたアニメーションの処理
	if (m_prev.animNo != -1)
	{
		// アニメーションの総時間獲得
		m_prev.totalTime = MV1GetAttachAnimTotalTime(m_model, m_prev.animNo);

		// アニメーションを進行させる
		m_prev.elapsedTime += kAnimBlendAdd;

		// アニメーションの再生時間をループ
		if (m_prev.animNo > m_prev.totalTime)
		{
			m_prev.elapsedTime = static_cast<float>(fmod(m_prev.elapsedTime, m_prev.totalTime));
		}

		// 進めた時間に設定
		MV1SetAttachAnimTime(m_model, m_prev.animNo, m_prev.elapsedTime);
		// アニメーションのブレンド率を設定する
		MV1SetAttachAnimBlendRate(m_model, m_prev.animNo, kAnimBlendMax - m_animBlendRate);
	}
}

/// <summary>
/// アニメーションの再生
/// </summary>
/// <param name="animIndex">再生するアニメーションの状態</param>
/// (一応)自分で書いたやつ
void Player::PlayAnim(AnimKind animIndex)
{
	// 更に古いアニメーションがアタッチされている場合はこの時点で削除しておく
	if (m_prev.animNo != -1)
	{
		MV1DetachAnim(m_model, m_prev.animNo);
		m_prev.animNo = -1;
	}

	// 現在再生中の待機アニメーションは変更前のアニメーション扱いに
	m_prev.animNo = m_current.animNo;
	m_prev.elapsedTime = m_current.elapsedTime;

	// 変更後のアニメーションとして攻撃アニメーションを改めて設定する
	m_current.animNo = MV1AttachAnim(m_model, static_cast<int>(animIndex), -1, false);
	m_current.elapsedTime = 0.0f;

	// ブレンド率はPrevが有効でない場合、1.0fにする
	if (m_prev.animNo == -1)
	{
		m_animBlendRate = kAnimBlendMax;
	}
	else {
		// 切り替えの瞬間は変更前のアニメーションが再生される状態にする
		m_animBlendRate = 0.0f;
	}

}

void Player::EndJumpState()
{
	m_isJump = false;

	m_pos.y = 0.0f;
	m_gravity = kGravity;

	//m_upPower = 0;
}

void Player::GravityUpdate()
{
	m_gravity += 0.005f;
}

void Player::InitAnim(AnimData& anim)
{
	anim.animNo = -1;
	anim.attachNo = -1;
	anim.totalTime = 0.0f;
	anim.elapsedTime = 0.0f;
	anim.isLoop = false;
}

void Player::SetAnim(int animNo, bool loop, bool isForceChange)
{
	if (!isForceChange)
	{
		// 再生中のアニメーションは再生しない
		if (m_current.animNo == animNo) return;
	}

	// 以前のアニメーションが残っていたら終了
	if (m_prev.animNo != -1)
	{
		MV1DetachAnim(m_model, m_prev.animNo);
		InitAnim(m_prev);
	}
	if (m_current.animNo != -1)
	{
		MV1DetachAnim(m_model, m_prev.animNo);
		InitAnim(m_current);
	}

	
	// 新しいアニメーションの設定
	m_current.animNo = animNo;
	m_current.attachNo = MV1AttachAnim(m_model, m_current.animNo, -1, false);
	m_current.totalTime = MV1GetAttachAnimTotalTime(m_model, m_current.animNo);
	m_current.isLoop = loop;

	// 変更に掛けるフレーム数を覚えておく
	m_animChangeFrameTotal = 1;
	m_animChangeFrame = 1;
}

void Player::ChangeAnimation(int animNo, bool Loop, bool isForceChange, int changeFrame)
{
	// 変更に掛けるフレーム数の記憶
	if (!isForceChange)
	{
		// 既に再生中のアニメーションは再生しない
		if (m_current.animNo == animNo)	return;
	}
	
	// 以前のアニメーションが残っていたら終了
	if (m_prev.animNo != -1)
	{
		MV1DetachAnim(m_model, m_prev.animNo);
		InitAnim(m_prev);
	}

	// 現在再生中のアニメーションは変更前のアニメーション扱いに
	m_prev = m_current;

	// 新しくアニメーションを設定する
	// 新しいアニメーションの設定
	m_current.animNo = animNo;
	m_current.attachNo = MV1AttachAnim(m_model, m_current.animNo, -1, false);
	m_current.totalTime = MV1GetAttachAnimTotalTime(m_model, m_current.animNo);
	m_current.isLoop = Loop;


	// 変更に掛けるフレーム数を覚えておく
	m_animChangeFrameTotal = changeFrame;
	m_animChangeFrame = 0;

	// アニメーションのブレンド率を設定する
	UpdateAnimBlendRate();
}

bool Player::IsAnimEnd()
{
	// Loopアニメの場合はfalseを返す
	if (m_current.isLoop)return false;

	float time = MV1GetAttachAnimTime(m_model, m_current.attachNo);
	if (time >= m_current.totalTime)
	{
		return true;
	}

	return false;
}

void Player::UpdateAnimation(AnimData anim, float dt)
{
	if (anim.attachNo == -1)	return;

	// アニメーションの更新
	anim.elapsedTime= MV1GetAttachAnimTime(m_model, anim.attachNo);
	anim.elapsedTime += dt;

	if (anim.elapsedTime >= anim.totalTime)
	{
		if (anim.isLoop)
		{
			// アニメーションのループ
			anim.elapsedTime -= anim.totalTime;
		}
		else
		{
			anim.elapsedTime = anim.totalTime;
		}

		// 進めた時間に設定
		MV1SetAttachAnimTime(m_model, anim.attachNo, anim.elapsedTime);
	}
}

void Player::UpdateAnimBlendRate()
{
	// ブレンド率はPrevが有効でない場合、1.0fにする
	if (m_prev.animNo == -1)
	{
		m_animBlendRate = kAnimBlendMax;
	}
	else {
		// 切り替えの瞬間は変更前のアニメーションが再生される状態にする
		m_animBlendRate = 0.0f;
	}
}


void Player::JumpStateInit()
{
	m_isJump = true;
}

void Player::AttackStateInit()
{
	m_isAttack = true;
	m_multiAttack = 0;
	m_isNextAttack = false;
}

void Player::IdleStateUpdate()
{
	ChangeAnimation(m_animData.kIdle, true, false, kStateAnimSpeed::Idle);
}

void Player::WalkStateUpdate()
{
	ChangeAnimation(m_animData.kWalk, true, false, kStateAnimSpeed::Walk);
}

void Player::JumpStateUpdate()
{
	ChangeAnimation(m_animData.kJump, true, false, kStateAnimSpeed::Jump);
}

void Player::AttackStateUpdate()
{
	switch (m_multiAttack)
	{
	case0:
		ChangeAnimation(m_animData.kAttack1, true, false, kStateAnimSpeed::Attack);
		break;
	case1:
		ChangeAnimation(m_animData.kAttack2, true, false, kStateAnimSpeed::Attack);
		break;
	case2:
		ChangeAnimation(m_animData.kAttack3, true, false, kStateAnimSpeed::Attack);
		break;
	case3:
		ChangeAnimation(m_animData.kAttack4, true, false, kStateAnimSpeed::Attack);
		break;
	default:
		break;
	}

	// 攻撃ボタンが押された時
	if (Pad::IsPress(PAD_INPUT_X) && !m_nextAttackFlag)
	{
		m_nextAttackFlag = true;
	}
	
	// アニメーションが終わった段階で次の攻撃フラグが経っていなかったら
	if (IsAnimEnd() && !m_nextAttackFlag)
	{
		m_isAttack = false;
		m_pAttackStanTime->Reset();
		m_multiAttack = 0;
		m_pState->EndState();
	}


	// アニメーションが終わった段階で次の攻撃フラグが立っていたら
	if (IsAnimEnd() && !m_nextAttackFlag)
	{
		// 攻撃硬直時間が0より小さくなったら
		if (m_pAttackStanTime->Update())
		{
			
			m_pAttackStanTime->Reset();
			m_nextAttackFlag = false;
			m_multiAttack++;
		}
	}
}

/// <summary>
/// 移動パラメータの設定
/// </summary>
VECTOR Player::MoveValue(const Camera& camera, VECTOR& upMoveVec, VECTOR& leftMoveVec)
{
	// プレイヤーの移動方向のベクトルを算出
	// 方向ボタン「↑」を押したときのプレイヤーの移動ベクトルはカメラの視線方向からＹ成分を抜いたもの
	upMoveVec = VSub(camera.GetTarget(), camera.GetPosition());
	upMoveVec.y = 0.0f;

	// 方向ボタン「←」を押したときのプレイヤーの移動ベクトルは上を押したときの方向ベクトルとＹ軸のプラス方向のベクトルに垂直な方向
	leftMoveVec = VCross(upMoveVec, VGet(0.0f, kSpeed, 0.0f));

	// 移動値を初期値に戻す
	m_move = VGet(0.0f, 0.0f, 0.0f);

	// 移動したか(true:移動した)
	bool isPressMove = false;

	// 移動処理
	if (!m_isAttack)
	{
		if (Pad::IsPress(PAD_INPUT_RIGHT))						// 右方向
		{
			m_move = VAdd(m_move, VScale(leftMoveVec, -1.0f));
			isPressMove = true;
		}
		if (Pad::IsPress(PAD_INPUT_LEFT))						// 左方向
		{
			m_move = VAdd(m_move, leftMoveVec);
			isPressMove = true;
		}
		if (Pad::IsPress(PAD_INPUT_UP))							// 前方向
		{
			m_move = VAdd(m_move, upMoveVec);
			isPressMove = true;
		}
		if (Pad::IsPress(PAD_INPUT_DOWN))						// 後ろ方向
		{
			m_move = VAdd(m_move, VScale(upMoveVec, -1.0f));
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

			m_targetDir = VNorm(m_move);
			m_move = VScale(m_targetDir, kSpeed);

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

	//ジャンプ処理
	Jump();
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

	//// 攻撃時の前進速度を足す
	//m_move = VAdd(m_move, m_moveAttack);

	// プレイヤーの位置に移動量を足す
	m_pos = VAdd(m_pos, m_move);

	// プレイヤーの位置セット
	MV1SetPosition(m_model, m_pos);
}

/// <summary>
/// プレイヤーの角度処理
/// </summary>
void Player::Angle()
{
	// プレイヤーの移動方向にモデルの方向を近づける
	float targetAngle;		// 目標角度
	float difference;		// 目標角度と現在の角度の差

	// 目標の方向ベクトルから角度値を算出する
	targetAngle = static_cast<float>(atan2(m_targetDir.x, m_targetDir.z));

	// 目標の角度と現在の角度との差を割り出す
	difference = targetAngle - m_angle;

	// 差の角度が180度以上になっていたら修正する
	if (difference < -DX_PI_F)
	{
		difference += DX_TWO_PI_F;
	}
	else if (difference > DX_PI_F)
	{
		difference -= DX_TWO_PI_F;
	}

	// 角度の差が0に近づける
	if (difference > 0.0f)
	{
		// 差がプラスの場合は引く
		difference -= kAngleSpeed;
		if (difference < 0.0f)
		{
			difference = 0.0f;
		}
	}
	else
	{
		// 差がマイナスの場合は足す
		difference += kAngleSpeed;
		if (difference > 0.0f)
		{
			difference = 0.0f;
		}
	}

	// モデルの角度を更新
	m_angle = targetAngle - difference;
	MV1SetRotationXYZ(m_model, VGet(0.0f, m_angle + DX_PI_F, 0.0f));
}

///// <summary>
///// プレイヤーの攻撃処理
///// </summary>
//Player::State Player::AttackState()
//{
//	State nextState = m_currentState;
//
//
//	// 現在、連続攻撃の処理をやってる
//
//
//	/**/
//
//	
//
//
//	return nextState;
//}
//
//Player::State Player::JumpState()
//{
//	State nextState = m_currentState;
//
//	// プレイヤーの状態が「ジャンプ」ではなく、且つボタン１が押されていたらジャンプする
//	if (nextState != State::kJump && (Pad::IsPress(PAD_INPUT_A)))
//	{
//		m_isJump = true;
//
//		// Ｙ軸方向の速度をセット
//		m_currentJumpPower = kJumpPower;
//
//		nextState = State::kJump;
//	}
//
//	return nextState;
//}

void Player::Attack()
{
	if (!m_isAttack)	return;

	/*if (m_isForward)
	{
		m_moveAttack = VAdd(m_moveAttack, VGet(0.0f, 0.0f, kAttackSpeed));
		m_isForward = false;
	}*/

	

}

void Player::Jump()
{
	if (!m_isJump)	return;

	if (m_pos.y >= 0.0f)
	{
		// ジャンプ状態なら重力適用
		if (m_currentState == State::kJump)
		{
			// Ｙ軸方向の速度を重力分減算する
			m_currentJumpPower -= m_gravity;
			GravityUpdate();
		}

		// 移動ベクトルのＹ成分をＹ軸方向の速度にする
		m_move.y = m_currentJumpPower;
	}
	else {
		EndJumpState();
	}
}