#include "Camera.h"

#include "../util/Util.h"
#include "../util/Easing.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"

#include <algorithm>

namespace {
	//半分
	constexpr int half = 2;

	//高さの調整用
	constexpr int height_adjustment = 2;

	//カメラのどの位置から見えるようにするか
	constexpr float camera_near = 5.0f;

	//カメラからどこまで離れたところを描画するか
	constexpr float camera_far = 5000.0f;

	//視野角
	constexpr float viewing_angle = 60.0f;

	//カメラが境界線に到達したときスライド移動するスピード
	constexpr float camera_moveX_speed = 10.0f;
	constexpr float camera_moveZ_speed = 40.0f;
	constexpr float camera_moveY_speed = 40.0f;

	//注視点を逸らすときのどのくらい注視点から逸らすのか
	constexpr float add_focus = 30.0f;

	//カメラの初期ポジション
	const VECTOR init_pos = VGet(0, 400, -800);

	//カメラのZ座標が移動する際、ボーダーライン
	constexpr float tracking_Z_borderline = 1000.0f;

	//カメラのY座標が移動する際、ボーダーラインが有効な範囲
	constexpr float border_range = 100.0f;

	//時間を増加させる
	constexpr float add_time = 1.0f;

	//カメラの移動にかかる合計時間
	constexpr float total_time = 90.0f;

	//カメラのギミックの有効範囲
	constexpr float camera_gimmick_range = 1000.0f;

	//追跡してくる割合
	constexpr float traking_rate_x = 0.97f;
	constexpr float traking_rate_y = 0.9f;
	constexpr float traking_rate_z = 0.98f;

	//割合の最大
	constexpr float max_rate = 1.0f;
}

Camera::Camera(const VECTOR& pos, const VECTOR& viewPos)
{
	//ポジションの設定
	pos_ = pos;

	//見る位置の設定
	cameraViewingPos_ = viewPos;

	//経過時間を最大にする
	elapsedTime_ = total_time;
}

Camera::~Camera()
{
}

void Camera::Init(const VECTOR& targetPos)
{
	/////////////// 3D関連の設定 /////////////
	// Zバッファを使用する
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを行う
	SetWriteZBuffer3D(true);

	// ポリゴンの裏面を描画しない
	SetUseBackCulling(true);

	//////////////// カメラの設定 //////////////////
	// カメラからどれだけ離れたところ( Near )から、 どこまで( Far )のものを描画するかを設定
	SetCameraNearFar(camera_near, camera_far);

	//カメラのポジション、見る場所、上の方向を設定
	SetCameraPositionAndTargetAndUpVec(pos_, cameraViewingPos_, upVec_);

	// カメラの視野角を設定(ラジアン)
	float angle = MathUtil::DegreeToRadian(viewing_angle);
	SetupCamera_Perspective(angle);
}

void Camera::Update(const VECTOR& playerPos, const float playerHeight)
{
	//プレイヤーを追跡するカメラの更新
	TrackingCameraUpdate(playerPos, playerHeight);
}

//プレイヤーを追跡するカメラの更新
void Camera::TrackingCameraUpdate(const VECTOR& playerPos, const float playerHeight)
{
	//カメラがプレイヤーを追いかける用にする
	pos_.x = TrackingPosX(playerPos);
	pos_.y = TrackingPosY(playerPos,playerHeight);
	pos_.z = (pos_.z * traking_rate_z) + ((playerPos.z + init_pos.z) * (max_rate - traking_rate_z));

	//プレイヤーがいた位置を見るようにする
	cameraViewingPos_.x = (cameraViewingPos_.x * traking_rate_x) + (playerPos.x * (max_rate - traking_rate_x));
	cameraViewingPos_.y = (cameraViewingPos_.y * traking_rate_y) + ((playerPos.y + playerHeight / half) * (max_rate - traking_rate_y));
	cameraViewingPos_.z = (cameraViewingPos_.z * traking_rate_z) + (playerPos.z * (max_rate - traking_rate_z));

	//カメラの注視点を変更する
	ChangeOfFocus(playerPos, playerHeight);

	SetCameraPositionAndTarget_UpVecY(pos_, cameraViewingPos_);
}

//カメラの注視点を逸らす
void Camera::ChangeOfFocus(const VECTOR& playerPos, const float playerHeight)
{
	//短縮化
	auto& input = InputState::GetInstance();

	GetJoypadDirectInputState(DX_INPUT_PAD1, &input_);

	if (input_.Ry < 0 || input.IsPressed(InputType::UpArrow)) {
		cameraViewingPos_.y = ((cameraViewingPos_.y + add_focus) * traking_rate_y) + ((playerPos.y + playerHeight / half) * (max_rate - traking_rate_y));
	}
	if (input_.Ry > 0 || input.IsPressed(InputType::DownArrow)) {
		cameraViewingPos_.y = ((cameraViewingPos_.y - add_focus) * traking_rate_y) + ((playerPos.y + playerHeight / half) * (max_rate - traking_rate_y));
	}
	if (input_.Rx < 0 || input.IsPressed(InputType::LeftArrow)) {
		cameraViewingPos_.x = ((cameraViewingPos_.x - add_focus) * traking_rate_x) + (playerPos.x * (max_rate - traking_rate_x));
	} 
	if (input_.Rx > 0 || input.IsPressed(InputType::RightArrow)) {
		cameraViewingPos_.x = ((cameraViewingPos_.x + add_focus) * traking_rate_x) + (playerPos.x * (max_rate - traking_rate_x));
	}

	SetCameraPositionAndTarget_UpVecY(pos_, cameraViewingPos_);
}

void Camera::EasingMoveCamera()
{
	//時間の更新
	elapsedTime_ = (std::min)(elapsedTime_ + add_time, total_time);

	//カメラの座標移動
	pos_.x = Easing::InOutCubic(elapsedTime_, total_time, cameraTargetPos_.x, pos_.x);
	pos_.y = Easing::InOutCubic(elapsedTime_, total_time, cameraTargetPos_.y, pos_.y);
	pos_.z = Easing::InOutCubic(elapsedTime_, total_time, cameraTargetPos_.z, pos_.z);

	//カメラの見る位置の移動
	cameraViewingPos_.x = Easing::InOutCubic(elapsedTime_, total_time, cameraTargetViewPos_.x, cameraViewingPos_.x);
	cameraViewingPos_.y = Easing::InOutCubic(elapsedTime_, total_time, cameraTargetViewPos_.y, cameraViewingPos_.y);
	cameraViewingPos_.z = Easing::InOutCubic(elapsedTime_, total_time, cameraTargetViewPos_.z, cameraViewingPos_.z);

	//カメラの上方向を変移させる
	upVec_.x = Easing::InOutCubic(elapsedTime_, total_time, targetUpVec_.x, upVec_.x);
	upVec_.y = Easing::InOutCubic(elapsedTime_, total_time, targetUpVec_.y, upVec_.y);
	upVec_.z = Easing::InOutCubic(elapsedTime_, total_time, targetUpVec_.z, upVec_.z);

	//カメラの情報の更新
	SetCameraPositionAndTargetAndUpVec(pos_, cameraViewingPos_, upVec_);

	if (elapsedTime_ >= total_time) {
		isMoving_ = false;
	}
	else {
		isMoving_ = true;
	}
}

//カメラが目標とする座標と見る位置を設定する
void Camera::SetCameraTargetPosAndView(const VECTOR& targetPos, const VECTOR& targetViewPos, const VECTOR& upVec)
{
	//目標位置の設定
	cameraTargetPos_ = targetPos;

	//見る位置の設定
	cameraTargetViewPos_ = targetViewPos;

	//カメラの上方向
	targetUpVec_ = upVec;
}

//プレイヤーのX座標を追跡
float Camera::TrackingPosX(const VECTOR& playerPos)
{

	float boderlinePosX = ExternalFile::GetInstance().GetCameraGimmickInfo(playerPos, "TrackingBorderlineX").pos.x;
	float gimmickPosX = ExternalFile::GetInstance().GetCameraGimmickInfo(playerPos, "TrackingX").pos.x;
	float distance = 0.0f;

	if (playerPos.x < gimmickPosX && playerPos.x > boderlinePosX - border_range) 
	{
		distance = gimmickPosX - pos_.x;
		moveVecX_ = distance / camera_moveY_speed;
		moveVecX_ = moveVecX_ * traking_rate_x;

		return pos_.x + moveVecX_;
	}

	return (pos_.x * traking_rate_x) + (playerPos.x * (max_rate - traking_rate_x));
}

//プレイヤーのY座標を追跡
float Camera::TrackingPosY(const VECTOR& playerPos, const  float playerHeight)
{

	VECTOR boderlinePos = ExternalFile::GetInstance().GetCameraGimmickInfo(playerPos, "TrackingBorderlineY").pos;
	float gimmickPosY = ExternalFile::GetInstance().GetCameraGimmickInfo(playerPos, "TrackingY").pos.y;
	float distance = 0.0f;
	float playerHeadPosY = playerPos.y + playerHeight;

	distance = boderlinePos.x - playerPos.x;

	distance = (std::max)(distance, -distance);

	if (distance < camera_gimmick_range)
	{
		if (playerHeadPosY < boderlinePos.y + border_range && playerHeadPosY > boderlinePos.y - border_range)
		{
			distance = gimmickPosY - pos_.y;
			moveVecY_ = distance / camera_moveY_speed;
			moveVecY_ = moveVecY_ * traking_rate_y;

			return pos_.y + moveVecY_;
		}
	}
	
	return (pos_.y * traking_rate_y + ((playerHeadPosY + playerHeight * height_adjustment) * (max_rate - traking_rate_y)));
}