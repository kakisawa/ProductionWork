#include "Player.h"
#include "DxLib.h"
#include "Pad.h"
#include "Camera.h"
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
	constexpr float kAnimChangeRateSpeed = 0.1f;	// アニメーション切り替えにかかる速度
	constexpr float kAnimBlendAdd = 0.5f;			// アニメーションブレンドの増加値
	constexpr float kAnimBlendMaxFrame = 1.0f;		// アニメーション切り替えに掛ける総フレーム数

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
	m_animBlendRate(0),
	m_currentJumpPower(0.0f),
	m_multiAttack(0),
	m_attackKind(AttackKind::kNone),
	m_currentState(State::kIdle),
	m_pos(kInitVec),
	m_move(kInitVec),
	m_targetDir(VGet(0.0f, 0.0f, 0.0f))
{
	// モデル読み込み
	m_model = MV1LoadModel(kModelPlayer);
	assert(m_model != -1);

	// アニメーション状態の初期化
	InitAnim(m_prev);
	InitAnim(m_current);
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
void Player::Update(const Camera& camera)
{
	// パッド入力によって移動パラメータを設定する
	VECTOR	upMoveVec;		// 方向ボタン「↑」を入力をしたときのプレイヤーの移動方向ベクトル
	VECTOR	leftMoveVec;	// 方向ボタン「←」を入力をしたときのプレイヤーの移動方向ベクトル

	// プレイヤーの状態更新
	State prevState = m_currentState;
	
	m_currentState = JumpState();
	m_currentState = AttackState();		// 攻撃

	// 攻撃処理
	Attack();

	Pad::Update();

	m_currentState = MoveValue(camera, upMoveVec, leftMoveVec);		// 移動

	//アニメーション状態の更新
	UpdateAnimState(prevState);

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

void Player::InitAnim(AnimData& anim)
{
	anim.animNo = -1;
	anim.attachNo = -1;
	anim.totalTime = 0.0f;
	anim.elapsedTime = 0.0f;
	anim.isLoop = false;
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

	/*if (state == State::kAttack && m_currentState == State::kAttack && m_attackKind == AttackKind::kNormalAttack2)
	{
		PlayAnim(AnimKind::kAttack2);
	}
	if (state == State::kAttack && m_currentState == State::kAttack && m_attackKind == AttackKind::kNormalAttack3)
	{
		PlayAnim(AnimKind::kAttack3);
	}
	if (state == State::kAttack && m_currentState == State::kAttack && m_attackKind == AttackKind::kNormalAttack4)
	{
		PlayAnim(AnimKind::kAttack4);
	}*/

	// ジャンプ→移動
	if (state == State::kJump && m_currentState == State::kWalk)
	{
		PlayAnim(AnimKind::kWalk);
	}
}

/// <summary>
/// アニメーション処理
/// </summary>
void Player::UpdateAnim()
{
	float total;		// 再生中アニメーションの最大値

	// test 8フレームで切り替え
	//if (m_animBlendRate < kAnimBlendMax)
	{
		// 指定フレームに掛けてアニメーションを変更する
		m_animBlendRate += kAnimChangeRateSpeed;
		if (m_animBlendRate >= kAnimBlendMaxFrame)
		{
			m_animBlendRate = kAnimBlendMaxFrame;
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
			// 攻撃アニメーションが終了し、かつ次の攻撃が入力されていたら
			// そのまま次のアニメーションを行う



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

		// アニメーション変化のフレーム数に応じたブレンド率の設定
		float rate = m_animBlendRate / kAnimBlendMaxFrame;
		if (rate > 1.0f)	rate = 1.0f;

		// アニメーションのブレンド率を設定する
		MV1SetAttachAnimBlendRate(m_model, m_current.animNo, rate);
	}

	// 一つ前に再生していたアニメーションの処理
	if (m_prev.animNo != -1)
	{
		// アニメーションの総時間獲得
		total = MV1GetAttachAnimTotalTime(m_model, m_prev.animNo);

		// アニメーションを進行させる
		m_prev.elapsedTime += kAnimBlendAdd;

		// アニメーションの再生時間をループ
		if (m_prev.animNo > total)
		{
			m_prev.elapsedTime = static_cast<float>(fmod(m_prev.elapsedTime, total));
		}

		// 進めた時間に設定
		MV1SetAttachAnimTime(m_model, m_prev.animNo, m_prev.elapsedTime);

		float rate = m_animBlendRate / kAnimBlendMaxFrame;
		if (rate > 1.0f)	rate = 1.0f;
		// アニメーションのブレンド率を設定する
		MV1SetAttachAnimBlendRate(m_model, m_prev.animNo, kAnimBlendMaxFrame - rate);
	}
}

/// <summary>
/// アニメーションの再生
/// </summary>
/// <param name="animIndex">再生するアニメーションの状態</param>
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
		m_animBlendRate = kAnimBlendMaxFrame;
	}
	else {
		// 切り替えの瞬間は変更前のアニメーションが再生される状態にする
		m_animBlendRate = 0.0f;
	}

}

/// <summary>
/// 移動パラメータの設定
/// </summary>
Player::State Player::MoveValue(const Camera& camera, VECTOR& upMoveVec, VECTOR& leftMoveVec)
{
	State nextState = m_currentState;

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

/// <summary>
/// プレイヤーの攻撃処理
/// </summary>
Player::State Player::AttackState()
{
	State nextState = m_currentState;


	// 現在、連続攻撃の処理をやってる
	if (Pad::IsTrigger(PAD_INPUT_X))
	{
		if (m_isAttack)
		{
			m_nextAttackFlag = true;
		}
		m_isAttack = true;
		//m_isForward = true;
		m_multiAttack++;
		

		nextState = State::kAttack;
	}



	switch (m_multiAttack)
	{
	case 1:
		m_attackKind=AttackKind::kNormalAttack1;
		break;
	case 2:
		m_attackKind = AttackKind::kNormalAttack2;
		break;
	case 3:
		m_attackKind = AttackKind::kNormalAttack3;
		break;
	case 4:
		m_attackKind = AttackKind::kNormalAttack4;
		break;
	default:
		break;
	}

	return nextState;
}

Player::State Player::JumpState()
{
	State nextState = m_currentState;

	// プレイヤーの状態が「ジャンプ」ではなく、且つボタン１が押されていたらジャンプする
	if (nextState != State::kJump && (Pad::IsTrigger(PAD_INPUT_A)))
	{
		m_isJump = true;

		// Ｙ軸方向の速度をセット
		m_currentJumpPower = kJumpPower;

		nextState = State::kJump;
	}

	return nextState;
}

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
			m_gravity += 0.005f;
		}

		// 移動ベクトルのＹ成分をＹ軸方向の速度にする
		m_move.y = m_currentJumpPower;
	}
	else {
		m_isJump = false;
		m_pos.y = 0.0f;
		m_gravity = kGravity;
	}
}