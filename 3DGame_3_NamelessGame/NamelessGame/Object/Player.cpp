#include "Player.h"
#include "../Camera.h"
#include "../Pad.h"
#include "../LoadCsv.h"
#include <cmath>
#include <cassert>

namespace {
	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// Vector値初期価値
	constexpr float kInitFloat = 0.0f;				// float値初期化

	const char* kModelFilePath = "Data/Model/PlayerModel.mv1";
}

Player::Player() :
	inputX(0),
	inputY(0)
{
	// プレイヤー外部データ読み込み
	LoadCsv::GetInstance().LoadData(m_chara, "player");
	// モデルの読み込み
	m_model = MV1LoadModel(kModelFilePath);
	assert(m_model != -1);

	// 座標初期値
	m_pos = VGet(m_chara.initPosX, m_chara.initPosY, m_chara.initPosZ);
	MV1SetScale(m_model, VGet(m_chara.modelSize, m_chara.modelSize, m_chara.modelSize));

	m_status.situation.isMoving = false;
}

Player::~Player()
{
}

void Player::Init()
{
	ModelBase::Init();

	// アニメーションの設定
	SetAnimation(static_cast<int>(PlayerAnim::Idle), true, false);
}

void Player::Update(const Camera& camera)
{
	Move(camera);
	Angle();
	
	ModelBase::Update();		// アニメーションの更新
}

void Player::Draw()
{
	ModelBase::Draw();
	
	DrawFormatString(0, 300, 0xffffff, "m_move.x/y/z=%.2f/%.2f/%.2f", m_move.x, m_move.y, m_move.z);

	DrawFormatString(0, 340, 0xffffff, "m_targetDir=%.2f", m_targetDir);
	DrawFormatString(0, 360, 0xffffff, "inputX=%d", inputX);
	DrawFormatString(0, 380, 0xffffff, "inputY=%d", inputY);
}

void Player::Move(const Camera& camera)
{
	// カメラの向きベクトルを取得
	VECTOR cameraForwardVec = VSub(camera.GetTarget(), camera.GetPosition());
	cameraForwardVec.y = 0.0f; // 水平成分のみ考慮する
	cameraForwardVec = VNorm(cameraForwardVec); // 正規化

	// カメラの右方向ベクトルを取得（forwardベクトルとY軸上ベクトルの外積で算出）
	VECTOR cameraRightVec = VCross(cameraForwardVec, VGet(0.0f, 1.0f, 0.0f));

	// 入力の初期化
	m_move = kInitVec;
	inputX = inputY = 0;

	// 入力取得
	GetJoypadAnalogInput(&inputX, &inputY, DX_INPUT_KEY_PAD1);

	// カメラ基準でプレイヤーの移動ベクトルを設定
	m_move = VScale(cameraForwardVec, static_cast<float>(-inputY));  // 前後移動
	m_move = VAdd(m_move, VScale(cameraRightVec, static_cast<float>(-inputX)));  // 左右移動

	// 正規化と移動速度の適用
	if (VSize(m_move) > 0.0f) {
		m_move = VNorm(m_move); // 正規化
		m_targetDir = m_move;  // 目標方向を保存
		m_move = VScale(m_move, m_chara.walkSpeed); // 移動速度を適用
	}

	m_pos = VAdd(m_pos, m_move);
	
	// 移動処理の更新
	MoveUpdate();	
}

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
		difference -= m_chara.rotaSpeed;
		if (difference < 0.0f)
		{
			difference = 0.0f;
		}
	}
	else
	{
		// 差がマイナスの場合は足す
		difference += m_chara.rotaSpeed;
		if (difference > 0.0f)
		{
			difference = 0.0f;
		}
	}

	// モデルの角度を更新
	m_angle = targetAngle - difference;
	MV1SetRotationXYZ(m_model, VGet(0.0f, m_angle + DX_PI_F, 0.0f));
}

void Player::MoveUpdate()
{
	float movingSpeed = std::max(m_move.x, m_move.z);

	m_status.situation.isMoving = false;

	if (movingSpeed != 0.0f) {
		m_status.situation.isMoving = true;

		// アニメーションの変更
		ChangeAnimNo(PlayerAnim::Run, true, m_animChangeTime.Idle);
	}
	else
	{
		ChangeAnimIdle();
	}
}

void Player::UseItem()
{
}

void Player::ChangeAnimNo(const PlayerAnim anim, const bool isAnimLoop, const int changeTime)
{
	m_status.animNo = static_cast<int>(anim);
	m_status.isLoop = isAnimLoop;
	ChangeAnimation(m_status.animNo, m_status.isLoop, false, changeTime);
}

void Player::ChangeAnimIdle()
{
	// 待機アニメーションに変更する
	if (!m_status.situation.isMoving) {
		ChangeAnimNo(PlayerAnim::Idle, true, m_animChangeTime.Run);
	}
}
