#include "Player.h"
#include "../Camera.h"
#include "../Pad.h"
#include "../LoadCsv.h"
#include <cmath>

namespace {
	const VECTOR InitVec = VGet(0.0f, 0.0f, 0.0f);	// Vector値初期価値
	constexpr float InitFloat = 0.0f;				// float値初期化

	const char* kModelFilePath = "Data/Model/PlayerModel.mv1";
}

Player::Player() :
	model(-1),
	m_angle(InitFloat),
	m_pos(InitVec),
	m_move(InitVec),
	m_targetDir(InitVec)
{
	// プレイヤー外部データ読み込み
	LoadCsv::GetInstance().LoadData(m_chara);
	// モデルの読み込み
	model = MV1LoadModel(kModelFilePath);

	// 座標初期値
	m_pos = VGet(m_chara.initPosX, m_chara.initPosY, m_chara.initPosZ);
}

Player::~Player()
{
	MV1DeleteModel(model);
}

void Player::Init()
{
	MV1SetPosition(model, m_pos);
	MV1SetScale(model, VGet(m_chara.modelSize, m_chara.modelSize, m_chara.modelSize));
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

	DrawFormatString(0, 340, 0xffffff, "m_targetDir=%.2f", m_targetDir);
	DrawFormatString(0, 360, 0xffffff, "inputX=%d", inputX);
	DrawFormatString(0, 380, 0xffffff, "inputY=%d", inputY);
}

void Player::Move(const Camera& camera)
{
	//{
	//	VECTOR upMoveVec = VSub(camera.GetTarget(), camera.GetPosition());
	//	upMoveVec.y = InitFloat;
	//	VECTOR leftMoveVec = VCross(upMoveVec, VGet(0.0f, m_chara.walkSpeed, 0.0f));
	//	// 移動量の初期化
	//	m_move = InitVec;
	//	inputX, inputY = 0;
	//	GetJoypadAnalogInput(&inputX, &inputY, DX_INPUT_KEY_PAD1);
	//	m_move = VGet(inputX, 0.0f, -inputY);
	//	if (inputX > 0.0f)					// 右方向
	//	{
	//		m_move = VAdd(m_move, VScale(leftMoveVec, -1.0f));
	//	}
	//	if (inputX < 0.0f)						// 左方向
	//	{
	//		m_move = VAdd(m_move, leftMoveVec);
	//	}
	//	if (inputY < 0.0f)							// 前方向
	//	{
	//		m_move = VAdd(m_move, upMoveVec);
	//	}
	//	if (inputY > 0.0f)						// 後ろ方向
	//	{
	//		m_move = VAdd(m_move, VScale(upMoveVec, -1.0f));
	//	}
	//	float len = VSize(m_move);
	//	// ベクトルの長さを0.0～1.0の割合に変換する
	//	float rate = len / 1000.0f;
	//	// 正規化
	//	if (VSize(m_move) > 0.0f) {
	//		m_move = VNorm(m_move);
	//		m_targetDir = m_move;
	//		m_move = VScale(m_move, m_chara.walkSpeed);
	//	}
	//}

	//if (Pad::IsPress(PAD_INPUT_RIGHT))						// 右方向
	//{
	//	m_move = VAdd(m_move, VScale(leftMoveVec, -1.0f));
	//}
	//if (Pad::IsPress(PAD_INPUT_LEFT))						// 左方向
	//{
	//	m_move = VAdd(m_move, leftMoveVec);
	//}
	//if (Pad::IsPress(PAD_INPUT_UP))							// 前方向
	//{
	//	m_move = VAdd(m_move, upMoveVec);
	//}
	//if (Pad::IsPress(PAD_INPUT_DOWN))						// 後ろ方向
	//{
	//	m_move = VAdd(m_move, VScale(upMoveVec, -1.0f));
	//}
	//// 正規化
	//if (VSize(m_move) > 0.0f) {
	//	m_move = VNorm(m_move);
	//	m_targetDir = m_move;
	//	m_move = VScale(m_move, m_chara.walkSpeed);
	//}


	// カメラの向きベクトルを取得
	VECTOR cameraForwardVec = VSub(camera.GetTarget(), camera.GetPosition());
	cameraForwardVec.y = 0.0f; // 水平成分のみ考慮する
	cameraForwardVec = VNorm(cameraForwardVec); // 正規化

	// カメラの右方向ベクトルを取得（forwardベクトルとY軸上ベクトルの外積で算出）
	VECTOR cameraRightVec = VCross(cameraForwardVec, VGet(0.0f, 1.0f, 0.0f));

	// 入力の初期化
	m_move = InitVec;
	inputX = inputY = 0;

	// 入力取得
	GetJoypadAnalogInput(&inputX, &inputY, DX_INPUT_KEY_PAD1);

	// カメラ基準でプレイヤーの移動ベクトルを設定
	m_move = VScale(cameraForwardVec, -inputY);  // 前後移動
	m_move = VAdd(m_move, VScale(cameraRightVec, -inputX));  // 左右移動

	// 正規化と移動速度の適用
	if (VSize(m_move) > 0.0f) {
		m_move = VNorm(m_move); // 正規化
		m_targetDir = m_move;  // 目標方向を保存
		m_move = VScale(m_move, m_chara.walkSpeed); // 移動速度を適用
	}
}

void Player::leftStick(VECTOR moveUp, VECTOR moveLeft)
{


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
	MV1SetRotationXYZ(model, VGet(0.0f, m_angle + DX_PI_F, 0.0f));
}
