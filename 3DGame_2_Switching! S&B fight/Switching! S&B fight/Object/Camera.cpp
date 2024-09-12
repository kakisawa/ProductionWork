#include "DxLib.h"
#include "Camera.h"
#include "Player/Player.h"
#include <algorithm>
#include <cmath>

namespace {

	// カメラ情報
	constexpr float kCameraHeight = 15.0f;	// カメラの注視点
	constexpr float kCameraNear = 1.0f;		// カメラ手前クリップ距離
	constexpr float kCameraFar = 10000.0f;	// カメラ最奥クリップ距離
	constexpr float kDist = 30.0f;			// カメラからプレイヤーまでの距離
	constexpr float kAngle = 0.05f;			// カメラを動かす角度

	constexpr float kInitAngleH = -DX_PI_F / 2.0f;	// カメラの初期平行角度
	constexpr float kInitAngleV = -0.3f;	// カメラの初期垂直角度
	constexpr float kMinAngleV = DX_PI_F * 0.5f - 1.5f;		// 最小の垂直角度
	constexpr float kMaxAngleV = -DX_PI_F * 0.5f + 0.6f;	// 最大の垂直角度
	
	
	float cameraAddAngle=0.0f;
}

Camera::Camera():
	m_pos(VGet(0.0f, 0.0f, 0.0f)),
	m_targetPos(VGet(0.0f, 0.0f, 0.0f)),
	m_angleH(kInitAngleH),
	m_angleV(kInitAngleV)
{
	
}

void Camera::Init()
{
	SetCameraNearFar(kCameraNear, kCameraFar);
}

void Camera::Update(const Player& player)
{
	int Pad = GetJoypadInputState(DX_INPUT_PAD1);

	
	//入力状態初期化
	input.Rx = 0;
	input.Ry = 0;


	// 入力状態を取得
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	if (input.Rx < 0.0f)			// 右スティックを右に倒したら右回転する
	{
		m_angleH -= kAngle;
	}
	if (input.Rx > 0.0f)			// 右スティックを左に倒したら左回転する
	{
		m_angleH += kAngle;
	}
	if(input.Ry>0.0f)				// 右スティックを下に倒したら上方向に回る
	{
		m_angleV -= kAngle;
		// ある一定角度以上にならないようにする
		m_angleV = (std::max)(m_angleV, kMaxAngleV);

	}
	if (input.Ry < 0.0f)			// 右スティックを上に倒したら下方向に回る
	{
		m_angleV += kAngle;
		// ある一定角度以下にならないようにする
		m_angleV = (std::min)(kMinAngleV, m_angleV);
	}

	// カメラの注視点を設定
	m_targetPos = VAdd(player.GetPos(), VGet(0.0f, kCameraHeight, 0.0f));
	// カメラ位置補正
	FixCameraPos();

	// カメラの情報をライブラリのカメラに反映させる
	SetCameraPositionAndTarget_UpVecY(m_pos, m_targetPos);

	ChangeLightTypeDir(VGet(20.0f, -50.0f, 0.0f));

#ifdef _DEBUG
	DrawFormatString(0, 160, 0xffffff, "Camera:m_pos.x/y/z=%.2f/%.2f/%.2f", m_pos.x, m_pos.y, m_pos.z);

	DrawFormatString(0, 180, 0xffffff, "Player:m_pos.x/y/z=%.2f/%.2f/%.2f",
		player.GetPos().x, player.GetPos().y, player.GetPos().z);
#endif // DEBUG
}

void Camera::FixCameraPos()
{
	// 水平方向の回転
	auto rotY = MGetRotY(m_angleH);
	// 垂直方向の回転
	auto rotZ = MGetRotZ(m_angleV);

	// X軸にカメラからプレイヤーまでの距離分伸びたベクトルを垂直方向に回転する(Z軸回転)
	m_pos = VTransform(VGet(-kDist, 0.0f, 0.0f), rotZ);
	// 水平方向(Y軸回転)に回転する
	m_pos = VTransform(m_pos, rotY);

	// 注視点の座標を足す
	m_pos = VAdd(m_pos, m_targetPos);
}
