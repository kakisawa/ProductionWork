#include "Camera.h"

#include "../util/Util.h"
#include "../util/Easing.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"

#include <algorithm>

namespace {
	//����
	constexpr int half = 2;

	//�����̒����p
	constexpr int height_adjustment = 2;

	//�J�����̂ǂ̈ʒu���猩����悤�ɂ��邩
	constexpr float camera_near = 5.0f;

	//�J��������ǂ��܂ŗ��ꂽ�Ƃ����`�悷�邩
	constexpr float camera_far = 5000.0f;

	//����p
	constexpr float viewing_angle = 60.0f;

	//�J���������E���ɓ��B�����Ƃ��X���C�h�ړ�����X�s�[�h
	constexpr float camera_moveX_speed = 10.0f;
	constexpr float camera_moveZ_speed = 40.0f;
	constexpr float camera_moveY_speed = 40.0f;

	//�����_����炷�Ƃ��̂ǂ̂��炢�����_�����炷�̂�
	constexpr float add_focus = 30.0f;

	//�J�����̏����|�W�V����
	const VECTOR init_pos = VGet(0, 400, -800);

	//�J������Z���W���ړ�����ہA�{�[�_�[���C��
	constexpr float tracking_Z_borderline = 1000.0f;

	//�J������Y���W���ړ�����ہA�{�[�_�[���C�����L���Ȕ͈�
	constexpr float border_range = 100.0f;

	//���Ԃ𑝉�������
	constexpr float add_time = 1.0f;

	//�J�����̈ړ��ɂ����鍇�v����
	constexpr float total_time = 90.0f;

	//�J�����̃M�~�b�N�̗L���͈�
	constexpr float camera_gimmick_range = 1000.0f;

	//�ǐՂ��Ă��銄��
	constexpr float traking_rate_x = 0.97f;
	constexpr float traking_rate_y = 0.9f;
	constexpr float traking_rate_z = 0.98f;

	//�����̍ő�
	constexpr float max_rate = 1.0f;
}

Camera::Camera(const VECTOR& pos, const VECTOR& viewPos)
{
	//�|�W�V�����̐ݒ�
	pos_ = pos;

	//����ʒu�̐ݒ�
	cameraViewingPos_ = viewPos;

	//�o�ߎ��Ԃ��ő�ɂ���
	elapsedTime_ = total_time;
}

Camera::~Camera()
{
}

void Camera::Init(const VECTOR& targetPos)
{
	/////////////// 3D�֘A�̐ݒ� /////////////
	// Z�o�b�t�@���g�p����
	SetUseZBuffer3D(true);

	// Z�o�b�t�@�ւ̏������݂��s��
	SetWriteZBuffer3D(true);

	// �|���S���̗��ʂ�`�悵�Ȃ�
	SetUseBackCulling(true);

	//////////////// �J�����̐ݒ� //////////////////
	// �J��������ǂꂾ�����ꂽ�Ƃ���( Near )����A �ǂ��܂�( Far )�̂��̂�`�悷�邩��ݒ�
	SetCameraNearFar(camera_near, camera_far);

	//�J�����̃|�W�V�����A����ꏊ�A��̕�����ݒ�
	SetCameraPositionAndTargetAndUpVec(pos_, cameraViewingPos_, upVec_);

	// �J�����̎���p��ݒ�(���W�A��)
	float angle = MathUtil::DegreeToRadian(viewing_angle);
	SetupCamera_Perspective(angle);
}

void Camera::Update(const VECTOR& playerPos, const float playerHeight)
{
	//�v���C���[��ǐՂ���J�����̍X�V
	TrackingCameraUpdate(playerPos, playerHeight);
}

//�v���C���[��ǐՂ���J�����̍X�V
void Camera::TrackingCameraUpdate(const VECTOR& playerPos, const float playerHeight)
{
	//�J�������v���C���[��ǂ�������p�ɂ���
	pos_.x = TrackingPosX(playerPos);
	pos_.y = TrackingPosY(playerPos,playerHeight);
	pos_.z = (pos_.z * traking_rate_z) + ((playerPos.z + init_pos.z) * (max_rate - traking_rate_z));

	//�v���C���[�������ʒu������悤�ɂ���
	cameraViewingPos_.x = (cameraViewingPos_.x * traking_rate_x) + (playerPos.x * (max_rate - traking_rate_x));
	cameraViewingPos_.y = (cameraViewingPos_.y * traking_rate_y) + ((playerPos.y + playerHeight / half) * (max_rate - traking_rate_y));
	cameraViewingPos_.z = (cameraViewingPos_.z * traking_rate_z) + (playerPos.z * (max_rate - traking_rate_z));

	//�J�����̒����_��ύX����
	ChangeOfFocus(playerPos, playerHeight);

	SetCameraPositionAndTarget_UpVecY(pos_, cameraViewingPos_);
}

//�J�����̒����_����炷
void Camera::ChangeOfFocus(const VECTOR& playerPos, const float playerHeight)
{
	//�Z�k��
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
	//���Ԃ̍X�V
	elapsedTime_ = (std::min)(elapsedTime_ + add_time, total_time);

	//�J�����̍��W�ړ�
	pos_.x = Easing::InOutCubic(elapsedTime_, total_time, cameraTargetPos_.x, pos_.x);
	pos_.y = Easing::InOutCubic(elapsedTime_, total_time, cameraTargetPos_.y, pos_.y);
	pos_.z = Easing::InOutCubic(elapsedTime_, total_time, cameraTargetPos_.z, pos_.z);

	//�J�����̌���ʒu�̈ړ�
	cameraViewingPos_.x = Easing::InOutCubic(elapsedTime_, total_time, cameraTargetViewPos_.x, cameraViewingPos_.x);
	cameraViewingPos_.y = Easing::InOutCubic(elapsedTime_, total_time, cameraTargetViewPos_.y, cameraViewingPos_.y);
	cameraViewingPos_.z = Easing::InOutCubic(elapsedTime_, total_time, cameraTargetViewPos_.z, cameraViewingPos_.z);

	//�J�����̏������ψڂ�����
	upVec_.x = Easing::InOutCubic(elapsedTime_, total_time, targetUpVec_.x, upVec_.x);
	upVec_.y = Easing::InOutCubic(elapsedTime_, total_time, targetUpVec_.y, upVec_.y);
	upVec_.z = Easing::InOutCubic(elapsedTime_, total_time, targetUpVec_.z, upVec_.z);

	//�J�����̏��̍X�V
	SetCameraPositionAndTargetAndUpVec(pos_, cameraViewingPos_, upVec_);

	if (elapsedTime_ >= total_time) {
		isMoving_ = false;
	}
	else {
		isMoving_ = true;
	}
}

//�J�������ڕW�Ƃ�����W�ƌ���ʒu��ݒ肷��
void Camera::SetCameraTargetPosAndView(const VECTOR& targetPos, const VECTOR& targetViewPos, const VECTOR& upVec)
{
	//�ڕW�ʒu�̐ݒ�
	cameraTargetPos_ = targetPos;

	//����ʒu�̐ݒ�
	cameraTargetViewPos_ = targetViewPos;

	//�J�����̏����
	targetUpVec_ = upVec;
}

//�v���C���[��X���W��ǐ�
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

//�v���C���[��Y���W��ǐ�
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