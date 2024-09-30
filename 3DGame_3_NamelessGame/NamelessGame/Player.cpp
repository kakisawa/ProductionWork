#include "Player.h"
#include "Camera.h"
#include "Pad.h"
#include <cmath>

namespace {
	const VECTOR InitVec = VGet(0.0f, 0.0f, 0.0f);
	constexpr float kMove = 0.7f;
	constexpr float	kAngleSpeed =0.15f;			// 角度変化速度
}

Player::Player() :
	model(-1),
	m_angle(0.0f),
	m_pos(InitVec),
	m_move(InitVec),
	m_targetDir(InitVec)
{
	model = MV1LoadModel("Data/Model/PlayerModel.mv1");
}

Player::~Player()
{
	MV1DeleteModel(model);
}

void Player::Init()
{
	m_pos = VGet(0.0f,0.0f,0.0f);
	MV1SetPosition(model, m_pos);
	MV1SetScale(model, VGet(0.5f, 0.5f, 0.5f));
}

void Player::Update(const Camera& camera)
{	
	Move(camera);
	Angle();

	m_pos = VAdd(m_pos, m_move);
	MV1SetPosition(model, m_pos);
}

void Player::Draw()
{
	MV1DrawModel(model);

	DrawFormatString(0, 300, 0xffffff, "m_move.x/y/z=%.2f/%.2f/%.2f", m_move.x, m_move.y, m_move.z);
}

void Player::Move(const Camera& camera)
{
	VECTOR upMoveVec = VSub(camera.GetTarget(), camera.GetPosition());
	upMoveVec.y = 0.0f;

	VECTOR leftMoveVec= VCross(upMoveVec, VGet(0.0f, kMove, 0.0f));

	// 移動量の初期化
	m_move = InitVec;


	if (Pad::IsPress(PAD_INPUT_RIGHT))						// 右方向
	{
		m_move = VAdd(m_move, VScale(leftMoveVec, -1.0f));
	}
	if (Pad::IsPress(PAD_INPUT_LEFT))						// 左方向
	{
		m_move = VAdd(m_move, leftMoveVec);
	}
	if (Pad::IsPress(PAD_INPUT_UP))							// 前方向
	{
		m_move = VAdd(m_move, upMoveVec);
	}
	if (Pad::IsPress(PAD_INPUT_DOWN))						// 後ろ方向
	{
		m_move = VAdd(m_move, VScale(upMoveVec, -1.0f));
	}

	// 正規化
	if (VSize(m_move) > 0.0f){
		m_move = VNorm(m_move);
		m_targetDir = m_move;
		m_move = VScale(m_move, kMove);
	}
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
	MV1SetRotationXYZ(model, VGet(0.0f, m_angle + DX_PI_F, 0.0f));
}
