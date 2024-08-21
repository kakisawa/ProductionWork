#include "Player.h"
#include "DxLib.h"
#include "Model.h"
#include "Pad.h"
#include "Camera.h"
#include "PlayerState.h"
#include "GameMap.h"
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


	// 初期化用値
	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// ベクトルの初期化
	constexpr float kInitFloat = 0.0f;				// float値の初期化
}

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player() :
	m_angle(kInitFloat),
	m_gravity(kGravity),
	m_isAttack(false),
	m_nextAttackFlag(false),
	m_isFirstAttack(false),
	m_isWalk(false),
	m_isJump(false),
	m_jumpPower(0.0f),
	m_multiAttack(0),
	m_pos(kInitVec),
	m_move(kInitVec),
	m_targetDir(VGet(0.0f, 0.0f, 0.0f))
{
	//モデルインスタンス作成
	m_pModel = std::make_shared<Model>(kModelPlayer);
	// アイドル状態のアニメーションを再生させる
	m_pModel->SetAnim(m_animData.kIdle, false, true);

	//ステイトクラスのインスタンス生成
	m_pState = std::make_shared<PlayerState>();

	m_pState->AddState([=] { IdleStateUpdate(); }, [=] { IdleStateInit(); }, PlayerState::State::kIdle);
	m_pState->AddState([=] { WalkStateUpdate(); }, [=] { WalkStateInit(); }, PlayerState::State::kWalk);
	m_pState->AddState([=] { JumpStateUpdate(); }, [=] { JumpStateInit(); }, PlayerState::State::kJump);
	m_pState->AddState([=] { AttackStateUpdate(); }, [=] { AttackStateInit(); }, PlayerState::State::kAttack);

	//初期ステイトセット
	m_pState->SetState(PlayerState::State::kIdle);	//ステイトセット(最初はIdle状態)
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
void Player::Init(std::shared_ptr<GameMap> pMap)
{
	// プレイヤー初期設定
	m_pModel->SetSize(VGet(kModelSize, kModelSize, kModelSize));
	m_pModel->SetRota(VGet(0.0f, kInitAngle, 0.0f));
	m_pModel->SetPos(m_pos);

	mp.leftBack = pMap->GetMapLeftBack();
	mp.rightFront = pMap->GetMapRightFront();
}


/// <summary>
/// 更新
/// </summary>
void Player::Update(const Camera& camera)
{
	// パッド入力によって移動パラメータを設定する
	VECTOR	upMoveVec;		// 方向ボタン「↑」を入力をしたときのプレイヤーの移動方向ベクトル
	VECTOR	leftMoveVec;	// 方向ボタン「←」を入力をしたときのプレイヤーの移動方向ベクトル

	// ステイトの更新
	m_pState->Update();

	// プレイヤーの状態更新
	// 攻撃処理
	Attack();
	// 移動処理
	OldMoveValue(camera, upMoveVec, leftMoveVec);

	// モデルのアップデート
	m_pModel->Update();

	// プレイヤーの移動方向にモデルの方向を近づける
	Angle();

	// プレイヤーの座標更新
	Move(m_move);
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw()
{
	m_pModel->Draw();

	DrawFormatString(0, 200, 0xffffff, "State=%d", m_pState->GetState());
	DrawFormatString(0, 220, 0xffffff, "m_isWalk=%d", m_isWalk);
	DrawFormatString(0, 240, 0xffffff, "m_pos.x,y,z=%.2f,=%.2f,=%.2f", m_pos.x, m_pos.y, m_pos.z);
	DrawFormatString(0, 260, 0xffffff, "m_move.x,y,z=%.2f,=%.2f,=%.2f", m_move.x, m_move.y, m_move.z);

	DrawFormatString(0, 280, 0xffffff, "m_angle=%.2f", m_angle);
}

/// <summary>
/// 終了
/// </summary>
void Player::End()
{
}


void Player::JumpStateInit()
{
	m_isJump = true;
	m_jumpPower = kJumpPower;
}

void Player::AttackStateInit()
{
	m_isAttack = true;
	m_multiAttack = 0;
	m_nextAttackFlag = false;
	m_isFirstAttack = true;
}

void Player::IdleStateUpdate()
{
	// アニメーションを待機モーションに変更
	m_pModel->ChangeAnim(m_animData.kIdle, true, false, 0.5f);
}

void Player::WalkStateUpdate()
{
	// アニメーションを歩きモーションに変更
	m_pModel->ChangeAnim(m_animData.kWalk, true, false, 0.5f);
}

void Player::JumpStateUpdate()
{
	// アニメーションをジャンプモーションに変更
	m_pModel->ChangeAnim(m_animData.kJump, false, false, 1.0f);
}

void Player::AttackStateUpdate()
{
	// アニメーション変更
	switch (m_multiAttack)
	{
	case 0:
		m_pModel->ChangeAnim(m_animData.kAttack1, false, false, 1.0f);
		break;
	case 1:
		m_pModel->ChangeAnim(m_animData.kAttack2, false, false, 1.0f);
		break;
	case 2:
		m_pModel->ChangeAnim(m_animData.kAttack3, false, false, 1.0f);
		break;
	case 3:
		m_pModel->ChangeAnim(m_animData.kAttack4, false, false, 1.0f);
		break;
	default:
		break;
	}

	
	if (Pad::IsTrigger(PAD_INPUT_X) && !m_nextAttackFlag)
	{	
		if (!m_isFirstAttack)
		{
			m_nextAttackFlag = true;
		}
		m_isFirstAttack = false;
	}

	// アニメーションが終わった段階で次の攻撃フラグがたっていなかったら
	if (m_pModel->IsAnimEnd() && !m_nextAttackFlag)
	{
		m_isAttack = false;
		m_multiAttack = 0;
		m_pState->EndState();
	}

	// アニメーションが終わった段階で次の攻撃フラグがたっていたら
	if (m_pModel->IsAnimEnd() && m_nextAttackFlag)
	{
		// 硬直時間を入れるならここ
		{
			m_nextAttackFlag = false;
			m_multiAttack++;
		}
	}
}

/// <summary>
/// 移動パラメータの設定
/// </summary>
void Player::OldMoveValue(const Camera& camera, VECTOR& upMoveVec, VECTOR& leftMoveVec)
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
			m_targetDir = m_move;
			m_move = VScale(m_move, kSpeed);
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

	// プレイヤーの位置に移動量を足す
	m_pos = VAdd(m_pos, m_move);


	// プレイヤーが画面外に出ないようする処理
	if (m_pos.x < mp.leftBack.x)
	{
		m_pos.x -= m_move.x;		// 左
	}
	if (m_pos.x > mp.rightFront.x)
	{
		m_pos.x -= m_move.x;		// 右
	}
	if (m_pos.z < mp.rightFront.z)
	{
		m_pos.z -= m_move.z;		// 前
	}
	if (m_pos.z > mp.leftBack.z)
	{
		m_pos.z -= m_move.z;		// 奥
	}

	// プレイヤーの位置セット
	MV1SetPosition(m_pModel->GetModel(), m_pos);
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
	MV1SetRotationXYZ(m_pModel->GetModel(), VGet(0.0f, m_angle + DX_PI_F, 0.0f));
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
		if (m_isJump)
		{
			// Ｙ軸方向の速度を重力分減算する
			m_jumpPower -= m_gravity;
			m_gravity += 0.005f;
		}

		// 移動ベクトルのＹ成分をＹ軸方向の速度にする
		m_move.y = m_jumpPower;
	}
	else {
		m_isJump = false;
		m_pos.y = 0.0f;
		m_gravity = kGravity;
		m_pState->EndState();
	}
}