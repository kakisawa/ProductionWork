#include "Player.h"

#include "../object/Corpse.h"
#include "../object/ObjectManager.h"

#include "../gimmick/Lever.h"
#include "../gimmick/ManualCrank.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"
#include "../util/ModelManager.h"
#include "../util/EffectManager.h"

#include <cmath>
#include <string>
#include <algorithm>

namespace
{
	//����
	constexpr int half = 2;

	//����A�j���[�V�����̑�����炷�t���[����
	constexpr int run_anim_foot_step_frame_type_1 = 6;
	constexpr int run_anim_foot_step_frame_type_2 = 45;

	//�����A�j���[�V�����̑�����炷�t���[����
	constexpr int walk_anim_foot_step_frame_type_1 = 80;
	constexpr int walk_anim_foot_step_frame_type_2 = 160;

	//�����e�̒��_��
	constexpr int shadow_vertex_num = 7;

	//�����e�ō��O�p�`�̐�
	constexpr int shadow_triangle_num = 6;

	//�O�p�`����鏇�Ԃ̐�
	constexpr int number_of_orders_to_form_a_triangle = 18;

	//���̂̐����J�E���g����ő吔
	constexpr int max_death_count = 99;

	//�A�j���[�V������ύX����̂ɂ����鎞��
	constexpr int anim_change_time = 20;

	//�����e�̔��a
	constexpr float shadow_radius = 25.0f;

	//������������͈�
	constexpr float sound_radius = 1500.0f;

	//�����e�̍����␳
	constexpr float correction_chadow_height = 5.0f;

	//�d��
	constexpr float gravity = -0.4f;

	//�v���C���[�̍���
	constexpr float player_hegiht = 130.0f;

	//Z���̈ړ�����
	constexpr float movement_restrictions_Z = -250.0f;

	//3D�T�E���h�͈̔�
	constexpr float sound_range = 1500.0f;

	//�t���[���̖��O
	const char* const frame_name = "hand.R_end";

	//������s���I�u�W�F�N�g�Ƃ̋��e�͈�
	constexpr float permissible_range = 30.0f;

	//90�x
	constexpr float degree_of_90 = 90.0f;

	//���݂̊p�x����ڕW�̊p�x�܂œ������Ƃ�
	//�ő�ŉ��x�܂œ��������Ƃ��o���邩
	constexpr float max_difference_angle = 135.0f;

	//�N�����N���񂷃X�s�[�h�̔{��
	constexpr float crank_rot_speed_rate = 3.0f;

	//�m�b�N�o�b�N��
	constexpr float knockback_rate = 0.96f;

	//�m�b�N�o�b�N�x�N�g���̐؂�̂Ĕ͈�
	constexpr float knockback_vector_truncation_range = 2.0f;

	//�������ʃx�N�g��
	constexpr VECTOR front_vec = { 0,0,-1 };

	//�N�����N�̎�����̍��W
	constexpr VECTOR crank_pos = { 0.0f,1.8f,0.0f };
}

//�R���X�g���N�^
Player::Player(const ObjectInfo& info):updateFunc_(&Player::NormalUpdate),carryUpdateFunc_(&Player::CarryObject)
{
	//�Z�k��
	auto& model = ModelManager::GetInstance();
	auto& file = ExternalFile::GetInstance();

	//�v���C���[���̏�����
	playerInfo_ = ExternalFile::GetInstance().GetPlayerInfo();

	//�v���C���[���f���̐���
	model_ = std::make_shared<Model>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Player)].name), Material::Other);

	//�A�j���[�V�����̐ݒ�
	model_->SetAnimation(static_cast<int>(PlayerAnimType::Idle), true, false);

	//�����|�W�V�����̎擾
	VECTOR startPos = file.GetStartPos(file.GetStartName());

	//�|�W�V�����̐ݒ�
	model_->SetPos(startPos);

	//��]���̐ݒ�
	model_->SetRot(info.rot);

	//�v���C���[�̑傫���̒���
	model_->SetScale(info.scale);

	//�R���W�����t���[���̐ݒ�
	model_->SetUseCollision(true, true, "Character");

	//�|�W�V�����̏�����
	status_.pos = startPos;

	//�g�k���̏�����
	scale_ = info.scale;

	//�W�����v���̏���
	status_.jump.isJump = false;
	status_.jump.jumpVec = 0.0f;
	//�v���C���[�̍����̐ݒ�
	status_.height = player_hegiht;
}

// �f�X�g���N�^
Player::~Player()
{
	ExternalFile::GetInstance().SetStartName("");
}

//�X�V
void Player::Update()
{
	//�v���C���[�̃A�j���[�V�����X�V
	model_->Update();
	
	(this->*updateFunc_)();
}

//�`��
void Player::Draw()
{
	//���f���̕`��
	model_->Draw();

	//�e���ǂ��̕`��
	DrawPolygon3D();
}

//���݂̊p�x����߂��p�x�ɉ��
void Player::RotateAtACloseAngle(float& differenceAngle, float targetAngle, float currentAngle)
{
	//���]�̊p�x
	float oneRevolution = 360.0f;

	if (differenceAngle >= oneRevolution / half)
	{
		differenceAngle = targetAngle - currentAngle - oneRevolution;
	}
	else if (differenceAngle <= -oneRevolution / half)
	{
		differenceAngle = targetAngle - currentAngle + oneRevolution;
	}
}

//�e�ɓ���������m�b�N�o�b�N��ǉ�����
void Player::BulletHitMe(const VECTOR& moveVec)
{
	//�v���C���[�����S���������炽�܂ɂ��m�b�N�o�b�N�𖳌��ɂ���
	if (updateFunc_ == &Player::DeathUpdate) 
	{
		return;
	}

	//�m�b�N�o�b�N��ǉ�����
	status_.moveVec = moveVec;

	//�����o�֐���ύX����
	updateFunc_ = &Player::BulletHitMeUpdate;
}

//�ǂ�ȉ�]��Ԃ��擾����
Player::RotationState Player::WhatRotationState()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//�C���v�b�g���
	bool IsPressedUp = input.IsPressed(InputType::Up);
	bool IsPressedDown = input.IsPressed(InputType::Down);
	bool IsPressedLeft = input.IsPressed(InputType::Left);
	bool IsPressedRight = input.IsPressed(InputType::Right);

	//���͏󋵂ɉ����ĉ�]��Ԃ�Ԃ�
	if (IsPressedUp && IsPressedRight)		return RotationState::UpperRight;
	if (IsPressedUp && IsPressedLeft)		return RotationState::UpperLeft;
	if (IsPressedDown && IsPressedRight)	return RotationState::LowerRight;
	if (IsPressedDown && IsPressedLeft)		return RotationState::LowerLeft;
	if (IsPressedUp)						return RotationState::Up;
	if (IsPressedDown)						return RotationState::Down;
	if (IsPressedLeft)						return RotationState::Left;
	if (IsPressedRight)						return RotationState::Right;

	//���݂̃X�e�[�g��Ԃ�
	return currentRotState_;
}

//�ڕW�̊p�x�ɉ�]���邱�Ƃ��o���邩
bool Player::CanRotation(float rotZ)
{
	//���]�̊p�x
	float oneRevolution = 360.0f;

	//�ڕW�̊p�x�ƌ��݂̊p�x�Ƃ̍�
	float differenceAngle = rotData_[static_cast<int>(currentRotState_)].targetAngle_ - rotZ;

	//���݂̊p�x����߂��p�x�ɉ��
	RotateAtACloseAngle(differenceAngle, rotData_[static_cast<int>(currentRotState_)].targetAngle_, rotZ);

	if (differenceAngle > max_difference_angle)			return false;
	if (rotZ + differenceAngle < 0.0f)					return false;
	if (rotZ + differenceAngle > crank_->GetMaxRotZ())	return false;

	return true;
}

//�|�W�V�����̐ݒ�
void Player::SetPos(const VECTOR& pos)
{
	//�X�e�[�^�X�Ƀ|�W�V������������
	status_.pos = pos;

	//���f���Ƀ|�W�V������ݒ肷��
	model_->SetPos(status_.pos);
}

//�W�����v�̐ݒ�
void Player::SetJumpInfo(bool isJump, float jumpVec)
{
	//�W�����v���Ă��邩�̃t���O��������
	status_.jump.isJump = isJump;

	//�W�����v�x�N�g����ݒ肷��
	status_.jump.jumpVec = jumpVec;

	//�|�W�V�����ɃW�����v�x�N�g���𑫂�
	status_.pos.y += status_.jump.jumpVec;
}

//�����^�Ԏ����o����t���O�Ǝ����^�ԃ��f���̃|�C���^���󂯎��
void Player::SetCarryInfo(const bool isCarry, const std::shared_ptr<ObjectBase>& model)
{
	//�����^�ђ��t���O��ݒ肷��
	status_.situation.isCanBeCarried = isCarry;

	//���̂̃��f���|�C���^���擾
	corpseModelPointer_ = model;
}

//ManualCrank�̃|�C���^��ݒ肷��
void Player::SetCrankPointer(const std::shared_ptr<ManualCrank>& crank)
{
	//���쒆�t���O��true�ɐݒ肷��
	status_.situation.isGimmickCanBeOperated = true;

	//�N�����N�̃|�C���^��ݒ肷��
	crank_ = crank;
}

//���o�[�̃|�C���^��ݒ肷��
void Player::SetLeverPointer(const std::shared_ptr<Lever>& lever)
{
	//���쒆�t���O��true�ɐݒ肷��
	status_.situation.isGimmickCanBeOperated = true;

	//���o�[�̃|�C���^��ݒ肷��
	lever_ = lever;
}

//�n�ʂɕ`�悷��e�̍����Ɠ���ł���I�u�W�F�N�g�̑f�ނ�ݒ肷��
void Player::SetRoundShadowHeightAndMaterial(const float height, const  Material materialType)
{
	//�e�̍�����ݒ肷��
	roundShadowHeight_ = height;

	//�����̃I�u�W�F�N�g�̑f�ނ�ݒ肷��
	materialSteppedOn_ = materialType;
}

//�ʏ펞�̍X�V
void Player::NormalUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

#ifdef _DEBUG
	//�폜�\��
	if (input.IsTriggered(InputType::Creative))
	{
		debugCreativeMode_ = !debugCreativeMode_;
	}
#endif
	
	//�����^�щ\�ȃI�u�W�F�N�g�ɑ΂��ăA�N�V�������N����
	if (input.IsTriggered(InputType::Activate))
	{
		if (status_.situation.isCanBeCarried) 
		{
			(this->*carryUpdateFunc_)();
		}
	}

	//�����K�v�Ƃ���M�~�b�N�ɑ΂��ăA�N�V�������N����
	if (input.IsTriggered(InputType::Activate))
	{
		//�x�N�g����0�ɂ���
		status_.moveVec = VGet(0, 0, 0);

		//�M�~�b�N�𑀍�o����t���O��true�Ȃ��
		//�����o�֐���ύX����
		if (status_.situation.isGimmickCanBeOperated)
		{
			if (crank_ != nullptr)
			{
				//�A�j���[�V�����̕ύX
				ChangeAnimNo(PlayerAnimType::Walk, true, anim_change_time);

				//�N�����N�𓮂�������������
				updateFunc_ = &Player::GoCrankRotationPosition;
			}
			else if (lever_ != nullptr)
			{
				//�A�j���[�V�����̕ύX
				ChangeAnimNo(PlayerAnimType::Walk, true, anim_change_time);

				//���o�[�𓮂�������������
				updateFunc_ = &Player::GoLeverPullPosition;
			}

			return;
		}
	}
	else {
		//�N�����N�𑀍삵�Ȃ��ꍇ�|�C���^�[�����Z�b�g����
		crank_.reset();

		//���o�[�𑀍삵�Ȃ��ꍇ�|�C���^�[�����Z�b�g����
		lever_.reset();
	}

	status_.situation.isGimmickCanBeOperated = false;

	//���̂������^�ђ���
	if (status_.situation.isInTransit)
	{
		//�v���C���[�̉�]�ƂƂ��Ɏ��̂̃��f������]������
		corpseModelPointer_->GetModelPointer()->SetRot(MathUtil::VECTORDegreeToRadian(status_.rot));

		//���̂̃|�W�V��������Ƀv���C���[�̎�̍��W�ɂ���
		VECTOR framePos = model_->GetFrameLocalPosition(frame_name);
		corpseModelPointer_->GetModelPointer()->SetPos(framePos);
	}
	else
	{
		status_.situation.isCanBeCarried = false;
	}

	//�A�j���[�V������ҋ@�ɂ���
	ChangeAnimIdle();

	//�ړ�
	MovingUpdate();

	//������炷
	FootStepSound();

#ifdef _DEBUG
	//�󒆂ɂ���Ƃ�
	//�d�͂��x�N�g���ɑ����ă|�W�V�����ɑ���
	if (!debugCreativeMode_)
	{
		if (status_.jump.isJump)
		{
			status_.jump.jumpVec += gravity;
			status_.pos.y += status_.jump.jumpVec;
			model_->SetPos(status_.pos);
		}
		else 
		{
			status_.jump.jumpVec = 0.0f;
		}
	}
#else
	//�󒆂ɂ���Ƃ�
	//�d�͂��x�N�g���ɑ����ă|�W�V�����ɑ���
	if (status_.jump.isJump) {
		status_.jump.jumpVec += gravity;
		status_.pos.y += status_.jump.jumpVec;
		model_->SetPos(status_.pos);
	}
	else {
		status_.jump.jumpVec = 0.0f;
	}
#endif // _DEBUG

	//�����^�ђ���������
	//�ȍ~�̏������s��Ȃ�
	if (status_.situation.isInTransit)
	{
		return;
	}

#ifdef _DEBUG
	//�����o�֐��|�C���^��jumpUpdate�ɕύX����
	if (!debugCreativeMode_)
	{
		if (input.IsTriggered(InputType::Space))
		{
			if (!status_.jump.isJump)
			{
				SetJumpInfo(true,playerInfo_.jumpPower);
			}
			ChangeAnimNo(PlayerAnimType::Jump, false, anim_change_time);
			updateFunc_ = &Player::JumpUpdate;
			return;
		}
	}
	else {
		if (input.IsPressed(InputType::Space))
		{
			status_.moveVec.y = playerInfo_.jumpPower;
		}
	}
#else
	//�����o�֐��|�C���^��jumpUpdate�ɕύX����
	if (input.IsTriggered(InputType::Space))
	{
		//�W�����v���Ă���t���O��false��������
		//�W�����v�������s��
		if (!status_.jump.isJump) {
			SetJumpInfo(true, playerInfo_.jumpPower);
		}

		//�A�j���[�V�������W�����v�ɕύX����
		ChangeAnimNo(PlayerAnimType::Jump, false, anim_change_time);

		//�����o�֐���ύX����
		updateFunc_ = &Player::JumpUpdate;
		return;
	}
#endif // _DEBUG

	//�����o�֐��|�C���^��DeathUpdate�ɕύX����
	if (input.IsTriggered(InputType::Death))
	{
		//���̂̐����J�E���g����
		deathCount_ = (std::min)(deathCount_ + 1, max_death_count);

		//�ړ��x�N�g����0�ɂ���
		status_.moveVec = VGet(0, 0, 0);

		//�����o�֐���ύX����
		updateFunc_ = &Player::DeathUpdate;
		return;
	}
}

//�ړ��̍X�V
void Player::MovingUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//�ړ��X�s�[�h�̎擾
	float movingSpeed = Move();
	
	if (movingSpeed != 0.0f) 
	{
		if (movingSpeed > playerInfo_.walkSpeed)
		{
			//�A�j���[�V�����̕ύX
			ChangeAnimNo(PlayerAnimType::Run, true, anim_change_time);
		}
		else if (movingSpeed <= playerInfo_.walkSpeed)
		{
			//�A�j���[�V�����̕ύX
			ChangeAnimNo(PlayerAnimType::Walk, true, anim_change_time);
		}
		
	}

	if (VSize(status_.moveVec) == 0.0f) 
	{
		status_.situation.isMoving = false;
		return;
	}

	//�ړ��x�N�g����p�ӂ���
	status_.moveVec = VScale(VNorm(status_.moveVec), movingSpeed);
}

//�ړ�����
float Player::Move()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//�L�[�̉������u�[���^�Ɋi�[
	bool pressedUp = input.IsPressed(InputType::Up);
	bool pressedDown = input.IsPressed(InputType::Down);
	bool pressedLeft = input.IsPressed(InputType::Left);
	bool pressedRight = input.IsPressed(InputType::Right);
	bool pressedShift = input.IsPressed(InputType::Dush);

	status_.moveVec = VGet(0,0,0);

	status_.situation.isMoving = false;
	float movingSpeed = 0.0f;

	if (pressedUp || pressedDown || pressedLeft || pressedRight) 
	{
		movingSpeed = PlayerSpeed(pressedShift);
		status_.situation.isMoving = true;
	}

	//HACK�F�����A���t�@�N�^�����O�K�{
	if (pressedUp) 
	{
		status_.moveVec.z += movingSpeed;
		targetAngle_ = 180.0f;
	}
	if (pressedDown) 
	{
		status_.moveVec.z -= movingSpeed;
		targetAngle_ = 0.0f;
	}
	if (pressedLeft) 
	{
		status_.moveVec.x -= movingSpeed;
		targetAngle_ = 90.0f;
	}
	if (pressedRight) 
	{
		status_.moveVec.x += movingSpeed;
		targetAngle_ = 270.0f;
	}
	if (pressedUp && pressedRight)
	{
		targetAngle_ = 225.0f;
	}
	if (pressedUp && pressedLeft) 
	{
		targetAngle_ = 135.0f;
	}
	if (pressedDown && pressedLeft) 
	{
		targetAngle_ = 45.0f;
	}
	if (pressedDown && pressedRight) 
	{
		targetAngle_ = 315.0f;
	}

	//��]����
	RotationUpdate();

	VECTOR destination = VAdd(status_.pos, status_.moveVec);
	if (destination.z < movement_restrictions_Z) 
	{
		status_.moveVec.z = 0.0f;
	}

	return movingSpeed;
}

//��]�̏���
void Player::RotationUpdate()
{
	//���]�̊p�x
	float oneRevolution = 360.0f;

	//�ڕW�̊p�x���猻�݂̊p�x�������č����o���Ă���
	differenceAngle_ = targetAngle_ - angle_;

	//��Ƀv���C���[���f�������肳�������Ȃ��̂�
	//181�x����-181�x�ȏ�̏ꍇ�A�t���ɂ��Ă�����
	RotateAtACloseAngle(differenceAngle_, targetAngle_, angle_);

	//���炩�ɉ�]�����邽��
	//���݂̊p�x�ɉ�]�X�s�[�h�𑝌������Ă���
	if (differenceAngle_ < 0.0f) 
	{
		status_.rot.y -= playerInfo_.rotSpeed;
		angle_ -= playerInfo_.rotSpeed;
	}
	else if (differenceAngle_ > 0.0f) 
	{
		status_.rot.y += playerInfo_.rotSpeed;
		angle_ += playerInfo_.rotSpeed;
	}

	//360�x�A���������0�x�ɖ߂��悤�ɂ��Ă���
	if (angle_ == oneRevolution || angle_ == -oneRevolution)
	{
		angle_ = 0.0f;
	}
	if (status_.rot.y == oneRevolution || status_.rot.y == -oneRevolution)
	{
		status_.rot.y = 0.0f;
	}

	//���ʂ����f���̉�]���Ƃ��đ���
	model_->SetRot(MathUtil::VECTORDegreeToRadian(status_.rot));
}

//�W�����v
void Player::JumpUpdate()
{
	//�v���C���[�ړ��֐�
	Move();

	//�W�����v����
	//�W�����v�x�N�g����0�ŃW�����v���ł͂Ȃ�������
	//idle��Ԃ̃A�b�v�f�[�g�ɕύX����A�A�j���[�V�������ύX����
	if (!status_.jump.isJump) 
	{
		updateFunc_ = &Player::NormalUpdate;
		return;
	}

	status_.jump.jumpVec += gravity;
	status_.pos.y += status_.jump.jumpVec;
	model_->SetPos(status_.pos);
}

// �v���C���[�̎��̂ɗ^����������֐�
void Player::DeathUpdate()
{
	//�A�j���[�V�����̕ύX
	ChangeAnimNo(PlayerAnimType::Death, false, anim_change_time);

	if (model_->IsAnimEnd()) 
	{
		CorpsePostProsessing();
	}
}

//���̂̌㏈��
void Player::CorpsePostProsessing()
{
	//���̂𐶐�����֐�
	CorpseInfoGenerater();			

	updateFunc_ = &Player::NormalUpdate;
}

// �v���C���[�̎��̂�vector�z��ŊǗ�����֐�
void Player::CorpseInfoGenerater()
{
	//�z�u�f�[�^�̍쐬
	corpseInfo_ = {};

	//���f���̒�����w��̃t���[���̔ԍ����擾����
	int frameNo = MV1SearchFrame(model_->GetModelHandle(), "PlaceToPutTheCorpse");

	//�w��̃t���[���̍��W���擾����
	VECTOR putPos = MV1GetFramePosition(model_->GetModelHandle(), frameNo);

	//�擾�������W�����̂̃|�W�V�����Ƃ���
	corpseInfo_.pos = putPos;

	//�v���C���[�̉�]�l�����̂̉�]�l�Ƃ���
	corpseInfo_.rot = MathUtil::VECTORDegreeToRadian(status_.rot);

	//�g�k���𓯂��ɂ���
	corpseInfo_.scale = scale_;

	//���̂𐶐��ł���t���O��true�ɂ���
	isCorpseGeneratable_ = true;
}

//�ו����^��
void Player::CarryObject()
{
	if (!status_.situation.isCanBeCarried) return;

	status_.situation.isInTransit = true;

	corpseModelPointer_->SetIsTransit(true);

	carryUpdateFunc_ = &Player::DropOffObject;
}

//�ו������낷
void Player::DropOffObject()
{
	if (status_.situation.isCanBeCarried)
	{
		status_.situation.isCanBeCarried = false;
		corpseModelPointer_->SetIsTransit(false);
		int frameNo = MV1SearchFrame(model_->GetModelHandle(), "PlaceToPutTheCorpse");
		VECTOR putPos = MV1GetFramePosition(model_->GetModelHandle(), frameNo);
		corpseModelPointer_->GetModelPointer()->SetPos(putPos);
		corpseModelPointer_.reset();
	}

	status_.situation.isInTransit = false;
	status_.situation.isCanBeCarried = false;

	carryUpdateFunc_ = &Player::CarryObject;
}

//�N�����N���񂷂��߂ɃN�����N���񂷃|�W�V�����ւƈړ�����
void Player::GoCrankRotationPosition()
{
	//�N�����N�̗����Ăق����|�W�V�������擾����
	VECTOR standPos = crank_->GetStandingPosition();

	//�����Ăق����|�W�V�����ƃv���C���[�̋����̃T�C�Y���擾����
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(status_.pos, standPos);

	//distanceSize�����͈̔͊O��������
	//���̑��x�ŗ����Ăق����|�W�V�����Ɍ�����
	if (distanceSize > permissible_range)
	{
		VECTOR distance = VNorm(VSub(standPos, status_.pos));
		VECTOR moveVec = VScale(distance, playerInfo_.walkSpeed);
		status_.pos = VAdd(status_.pos, moveVec);
		model_->SetPos(status_.pos);
	}
	//distanceSize�����͈͓̔��ɓ�������
	//�����Ăق����|�W�V�������v���C���[�̃|�W�V�����Ƃ���
	else 
	{
		status_.pos = standPos;
		model_->SetPos(status_.pos);
		angle_ = -degree_of_90;
		status_.rot = VGet(0, angle_, 0);
		model_->SetRot(MathUtil::VECTORDegreeToRadian(status_.rot));
		ChangeAnimNo(PlayerAnimType::Crank, false, anim_change_time);
		updateFunc_ = &Player::CrankUpdate;
	}
}

//�N�����N����]������A�b�v�f�[�g
void Player::CrankRotationUpdate(float rotZ)
{
	float radian = MathUtil::DegreeToRadian(rotZ);

	int frameNo = MV1SearchFrame(crank_->GetModelPointer()->GetModelHandle(), "Crank");

	VECTOR pos = MV1GetFramePosition(crank_->GetModelPointer()->GetModelHandle(), frameNo);

	VECTOR distance = crank_pos;

	MATRIX mat = {};

	float x = MathUtil::DegreeToRadian(degree_of_90);

	//���s�ړ��s��
	MATRIX posMat = MGetTranslate(distance);
	//��]�s��
	MATRIX rotMatZ = MGetRotZ(-radian);
	MATRIX rotMatX = MGetRotX(x);

	mat = MMult(rotMatX, rotMatZ);
	mat = MMult(mat, posMat);

	MV1SetFrameUserLocalMatrix(crank_->GetModelPointer()->GetModelHandle(), frameNo, mat);
	
	crank_->SetRotZ(rotZ);
}

//�N�����N�̍X�V
void Player::CrankUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();
	auto& sound = SoundManager::GetInstance();

	//�ړ��x�N�g����0�ɂ���
	status_.moveVec = VGet(0, 0, 0);

	//�N�����N�̉�]���擾����(���ۂɒl��ς���悤)
	float rotZ = crank_->GetRotZ();

	//�N�����N���񂷏������~�߂�
	if (input.IsTriggered(InputType::Activate))
	{
		crank_.reset();
		status_.situation.isGimmickCanBeOperated = false;
		updateFunc_ = &Player::NormalUpdate;
		return;
	}

	//1�t���[���O�̉�]���
	RotationState oldRotState = currentRotState_;

	//�ǂ�ȉ�]��Ԃ��擾����
	currentRotState_ = WhatRotationState();

	//�ڕW�̊p�x�ɉ�]���邱�Ƃ��o���邩
	if (!CanRotation(rotZ))
	{
		currentRotState_ = oldRotState;
	}

	//�N�����N���ڎw���p�x
	crankTargetAngle_ = rotData_[static_cast<int>(currentRotState_)].targetAngle_;

	//���݂̊p�x����ڕW�̊p�x�̍�
	float differenceAngle = crankTargetAngle_ - rotZ;
	
	//���݂̊p�x����߂��p�x�ɉ��
	RotateAtACloseAngle(differenceAngle, crankTargetAngle_, rotZ);

	//�ڕW�̊p�x�ƌ��݂̊p�x�������Ŗ�����Έȍ~�̏������s��
	if (differenceAngle != 0.0f)
	{
		//-�̋L���̕t���Ȃ����̊p�x���擾����
		float unsignedAngle = (std::max)(differenceAngle, -differenceAngle);

		//��]�̃X�s�[�h���擾����
		float angleSpeed = differenceAngle / unsignedAngle;

		//���ۂɌ��݂̊p�x�ɃX�s�[�h�𑫂�
		rotZ = rotZ + angleSpeed * crank_rot_speed_rate;

		//�N�����N����]������A�b�v�f�[�g
		CrankRotationUpdate(rotZ);
	}
	
	//�p�x�ɑ΂���A�j���[�V�����t���[�����̎擾
	int naturalNumber = static_cast<int>((std::max)(rotZ, -rotZ));
	float animTime = static_cast<float>(naturalNumber % static_cast<int>(crank_->GetMaxRotZ())) / crank_rot_speed_rate;

	//�A�j���[�V�����t���[���̐ݒ�
	model_->SetAnimationFrame(animTime);
}

//���o�[��|���|�W�V�����֍s��
void Player::GoLeverPullPosition()
{
	//�N�����N�̗����Ăق����|�W�V�������擾����
	VECTOR standPos = lever_->GetStandingPosition();
	//�����Ăق����|�W�V�����ƃv���C���[�̋����̃T�C�Y���擾����
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(status_.pos, standPos);

	//distanceSize�����͈̔͊O��������
	//���̑��x�ŗ����Ăق����|�W�V�����Ɍ�����
	if (distanceSize > permissible_range)
	{
		VECTOR distance = VNorm(VSub(standPos, status_.pos));
		VECTOR moveVec = VScale(distance, playerInfo_.walkSpeed);
		status_.pos = VAdd(status_.pos, moveVec);
		model_->SetPos(status_.pos);
	}
	//distanceSize�����͈͓̔��ɓ�������
	//�����Ăق����|�W�V�������v���C���[�̃|�W�V�����Ƃ���
	else
	{
		status_.pos = standPos;
		model_->SetPos(status_.pos);
		angle_ = 0.0f;
		lever_->OnAnimation();
		status_.rot = VGet(0, angle_, 0);
		model_->SetRot(MathUtil::VECTORDegreeToRadian(status_.rot));
		ChangeAnimNo(PlayerAnimType::LeverOn, false, anim_change_time);
		updateFunc_ = &Player::LeverUpdate;
	}
}

//���o�[�̍X�V
void Player::LeverUpdate()
{
	if (!lever_->IsOn())
	{
		updateFunc_ = &Player::NormalUpdate;
	}
}

//�������Ƃ̏Փ˃A�b�v�f�[�g
void Player::BulletHitMeUpdate()
{
	//�d��
	status_.jump.jumpVec += gravity;
	status_.moveVec.y = status_.jump.jumpVec;

	//�m�b�N�o�b�N
	status_.moveVec = VScale(status_.moveVec, knockback_rate);

	//�ړ��x�N�g���𑫂����s������擾����
	VECTOR destinationPos = VAdd(status_.pos, status_.moveVec);

	//�ړ��x�N�g���𑫂����s���悪-250�ȉ���������
	//�ړ��x�N�g����Z�l��0�ɂ���
	if (destinationPos.z < movement_restrictions_Z)
	{
		status_.moveVec.z = 0.0f;
	}

	//�v���C���[�̃|�W�V�����Ɉړ��x�N�g���𑫂�
	status_.pos = VAdd(status_.pos, status_.moveVec);

	//���f���Ƀ|�W�V������ݒ肷��
	model_->SetPos(status_.pos);

	float moveVecSize = VSize(status_.moveVec);

	if (moveVecSize < knockback_vector_truncation_range)
	{
		updateFunc_ = &Player::NormalUpdate;
	}
}

//�A�j���[�V������ҋ@�ɕύX����
void Player::ChangeAnimIdle()
{
	//�ҋ@�A�j���[�V�����ɖ߂�
	if (!status_.situation.isMoving)
	{
		ChangeAnimNo(PlayerAnimType::Idle, true, anim_change_time);
	}
}

//�A�j���[�V�����̕ύX���s��
void Player::ChangeAnimNo(PlayerAnimType type, bool isAnimLoop, int changeTime)
{
	status_.animNo = static_cast<int>(type);
	status_.isAnimLoop = isAnimLoop;
	model_->ChangeAnimation(status_.animNo, status_.isAnimLoop, false, changeTime);
}

//�v���C���[�̑��x�ݒ�
float Player::PlayerSpeed(bool pressedShift)
{
	//�V�t�g��������Ă��邩�ǂ���
	if (pressedShift)
	{
#ifdef _DEBUG
		if (debugCreativeMode_) {
			return playerInfo_.runningSpeed * 3;
		}
		return playerInfo_.runningSpeed;
#else
		return playerInfo_.runningSpeed;
#endif // _DEBUG
	} 
	
	return playerInfo_.walkSpeed;
}

//�����̍Đ�
void Player::FootStepSound()
{
	//�������Ă��邩
	bool playSound = false;

	//�v���C���[�̃A�j���[�V�����ɂ����
	//�A�j���[�V�����t���[���̓���t���[����
	//�������Ă��邩�̃t���O��true�ɂ���
	switch (static_cast<PlayerAnimType>(status_.animNo))
	{
	case PlayerAnimType::Walk:

		if (model_->GetSpecifiedAnimTime(walk_anim_foot_step_frame_type_1) || model_->GetSpecifiedAnimTime(walk_anim_foot_step_frame_type_2)) {
			playSound = true;
		}

		break;
	case PlayerAnimType::Run:

		if (model_->GetSpecifiedAnimTime(run_anim_foot_step_frame_type_1) || model_->GetSpecifiedAnimTime(run_anim_foot_step_frame_type_2)) {
			playSound = true;
		}

		break;
	}

	//�������Ă��Ȃ��ꍇ���^�[��
	if (!playSound)
	{
		return;
	}

	//�v���C���[�̑����̃I�u�W�F�N�g�̑f�ނɂ����
	//�炷����ς���
	switch (materialSteppedOn_)
	{
	case Material::Iron:
		SoundManager::GetInstance().Set3DSoundInfo(status_.pos, sound_radius, "ironStep");
		SoundManager::GetInstance().PlaySE("ironStep");
		break;
	case Material::Stone:
		SoundManager::GetInstance().Set3DSoundInfo(status_.pos, sound_radius, "asphaltStep");
		SoundManager::GetInstance().PlaySE("asphaltStep");
		break;
	}
}

//�����e���쐬�A�`��
void Player::DrawPolygon3D()
{
	//���_�̐����z������
	VERTEX3D vertex[shadow_vertex_num] = {};
	//�O�p�`���쐬���鏇�Ԃ�ۑ�����z��
	WORD index[number_of_orders_to_form_a_triangle] = {};

	//�J���[
	COLOR_U8 difColor = GetColorU8(51, 51, 51, 125);
	COLOR_U8 spcColor = GetColorU8(0, 0, 0, 0);

	//�@���x�N�g��
	VECTOR norm = VGet(0.0f, 1.0f, 0.0f);

	//�p�x�Ɋ���Ē��_�̈ʒu��ύX����
	float angle = 0.0f;

	//�Z�p�`�̒��S���W���擾
	vertex[0].pos = VGet(status_.pos.x, roundShadowHeight_, status_.pos.z);
	vertex[0].norm = norm;
	vertex[0].dif = difColor;
	vertex[0].spc = GetColorU8(0, 0, 0, 0);
	vertex[0].u = 0.0f;
	vertex[0].v = 0.0f;
	vertex[0].su = 0.0f;
	vertex[0].sv = 0.0f;

	//�p�x���Ƃ̒��_���擾
	for (int i = 1; i < shadow_vertex_num; i++)
	{
		vertex[i].pos = VertexPosition(angle);
		vertex[i].norm = norm;
		vertex[i].dif = difColor;
		vertex[i].spc = spcColor;
		vertex[i].u = 0.0f;
		vertex[i].v = 0.0f;
		vertex[i].su = 0.0f;
		vertex[i].sv = 0.0f;
		angle += 360.0f / shadow_vertex_num;
	}

	//�O�p�`���쐬���鏇��
	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 0;
	index[4] = 2;
	index[5] = 3;
	index[6] = 0;
	index[7] = 3;
	index[8] = 4;
	index[9] = 0;
	index[10] = 4;
	index[11] = 5;
	index[12] = 0;
	index[13] = 5;
	index[14] = 6;
	index[15] = 0;
	index[16] = 6;
	index[17] = 1;

	DrawPolygonIndexed3D(vertex, shadow_vertex_num, index, shadow_triangle_num, DX_NONE_GRAPH, true);
}

//�v���C���[�̗����e�Ɏg�p���钸�_���擾
VECTOR Player::VertexPosition(float angle)
{

	VECTOR pos = {};

	//�x���@���ʓx�@�ɕϊ�����
	float radian = MathUtil::DegreeToRadian(angle);
	
	//�p�x�ɂ����W���擾����
	pos.x = sin(radian);
	pos.z = cos(radian);
	pos.y = 0.0f;

	//�|�W�V������25�{����(�T�C�Y����)
	pos = VScale(pos, shadow_radius);

	//�v���C���[�̃|�W�V�����Ə�L�Ŏ擾�����|�W�V�����𑫂�
	pos = VAdd(status_.pos, pos);

	//Y���W�͐��ƃ|���S���̓����蔻��Ŏ擾����
	//��ԋ߂��|���S����Y���W��������
	pos.y = roundShadowHeight_;

	return pos;
}
