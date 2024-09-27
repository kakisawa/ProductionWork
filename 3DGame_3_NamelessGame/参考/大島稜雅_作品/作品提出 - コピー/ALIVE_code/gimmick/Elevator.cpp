#include "Elevator.h"
#include "Switch.h"
#include "Lever.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/Easing.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"

#include "../object/Player.h"

namespace 
{
	//�A�j���[�V�������ς��t���[����
	constexpr int anim_cange_frame = 10;

	//���o�[�̐�����
	constexpr int lever_generate_num = 3;

	//��~�|�W�V�����̐�
	constexpr int stop_position_num = 2;

	//���B����
	constexpr float total_time = 180.0f;

	//�T�E���h����������͈�
	constexpr float sound_range = 1500.0f;

	//�^�C�}�[��i�߂鎞��
	constexpr float add_time = 1.0f;

	//���o�[�Ɋւ���I�u�W�F�N�g��Y���W�̕␳�l
	constexpr float correction_y = 8.0f;

	//�G���x�[�^�[�̃J�v�Z���̔��a
	constexpr float elevator_capsel_range = 200.0f;

	//�v���C���[�̃J�v�Z���̔��a
	constexpr float player_capsel_range = 20.0f;
}

//�R���X�g���N�^
Elevator::Elevator(const int handle, const Material materialType, const ObjectInfo& objInfo):GimmickBase(handle, materialType, objInfo)
{
	//���̃G���x�[�^�[�����Ԗڂ̃G���x�[�^�[���𖼑O�̖�������擾����
	int elevatorNum = StrUtil::GetNumberFromString(objInfo.name, ".");

	//�X�C�b�`�̔z�u�f�[�^���擾����
	ObjectInfo leverData = GetSpecialNameObjectInfo("MovingLever", "-", elevatorNum, 0);

	//�X�C�b�`�̃C���X�^���X��
	movingLever_ = std::make_shared<Lever>(leverData);

	//���o�[�̃C���X�^���X��
	for (int i = 1; i < lever_generate_num; i++)
	{
		ObjectInfo LeverData = GetSpecialNameObjectInfo("ElevatorLever", "-", elevatorNum, i);
		levers_.push_back(std::make_shared<Lever>(LeverData));
	}

	//��~�|�W�V�����̎擾
	for (int i = 0; i < stop_position_num; i++)
	{
		ObjectInfo pointData = GetSpecialNameObjectInfo("ElevatorPoint", "-", elevatorNum, i);
		stopPos_[static_cast<ElevatorState>(i)] = pointData.pos;
	}

	//�^�[�Q�b�g�|�W�V�����̏�����
	targetPos_ = pos_;

	//�Փ˔���p�t���[���̐ݒ�
	model_->SetUseCollision(true, false);

	//�A�j���[�V�����̐ݒ�
	model_->SetAnimation(static_cast<int>(ElevatorAnimType::OpenIdle), true, false);

	//���݂̃G���x�[�^�[�̃X�e�[�^�X��ݒ肷��
	SetInitState();
}

//�f�X�g���N�^
Elevator::~Elevator()
{
}

//�X�V
void Elevator::Update(Player& player)
{
	//�Z�k��
 	auto& sound = SoundManager::GetInstance();

	//�v���C���[�̃|�W�V�����̎擾
	VECTOR playerPos = player.GetStatus().pos;

	//���f���̍X�V
	model_->Update();

	//���o�[�̍X�V
	movingLever_->Update();

	//�������o�[�Ƃ̏Փ˔���
	if (movingLever_->CollCheck(playerPos))
	{
		player.SetLeverPointer(movingLever_);
	}

	//���o�[�̍X�V
	for (const auto& lever : levers_)
	{
		lever->Update();
	}

	//���o�[�ƃv���C���[�̏Փ˔�����s��
	//�Փ˂��Ă����ꍇ�v���C���[�Ƀ|�C���^��ݒ肷��
	for (const auto& lever : levers_)
	{
		if (lever->CollCheck(playerPos)) 
		{
			player.SetLeverPointer(lever);
		}
	}

	//�o���o���邩���擾����
	isDeparture_ = CanDeparture();

	//�^�[�Q�b�g�|�W�V�����̕ύX
	if (targetPos_.y == pos_.y)
	{
		TargetPosition();
	}

	//�A�j���[�V�������I������A�ړ����J�n����
	if (model_->IsAnimEnd()) 
	{
		//�㏸�x�N�g���̎擾
		float upVecY = Move();

		//�Փ˔�����s��
		int result = HitCheck_Capsule_Capsule(pos_, VGet(pos_.x, pos_.y + scale_.y * 2, pos_.z), elevator_capsel_range, playerPos, VGet(playerPos.x, playerPos.y, playerPos.z), player_capsel_range);

		//�Փ˂��Ă�����
		if (result > 0)
		{
			//�v���C���[�̈ړ��x�N�g���ɏ㏸�x�N�g���𑫂�
			VECTOR playerMoveVec = player.GetStatus().moveVec;
			playerMoveVec.y += upVecY;

			//�v���C���[�̈ړ��x�N�g����ݒ肷��
			player.SetMoveVec(playerMoveVec);
		}
	}
}

//�`��
void Elevator::Draw()
{
	//���f���̕`��
	model_->Draw();

	//�X�C�b�`�̕`��
	//switch_->Draw();
	movingLever_->Draw();

	//���o�[�̕`��
	for (auto& lever : levers_) 
	{
		lever->Draw();
	}
}

//����炷
void Elevator::PlayDoorSound()
{
	//�Z�k��
	auto& sound = SoundManager::GetInstance();

	//���̐ݒ�
	sound.Set3DSoundInfo(pos_, sound_range, "door");

	//���̍Đ�
	sound.PlaySE("door");
}

//�o�����邱�Ƃ��\��
bool Elevator::CanDeparture()
{
	//���o�[��IsOn(�������)��true�ɂȂ�����
	//�o���ϐ���true�ɂ���
	if (movingLever_->IsOn())
	{
		return true;
	}

	//���o�[��IsOn(�������)��true�ɂȂ�����
	//�o���ϐ���true�ɂ���
	for (auto& lever : levers_)
	{
		if (lever->IsOn())
		{
			return true;
		}
	}

	return false;
}

//����Ȗ��O�̃I�u�W�F�N�g�̔z�u�f�[�^���擾����
ObjectInfo Elevator::GetSpecialNameObjectInfo(const std::string& name, const std::string& sign, int groupNum, int num)
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	//���O�ƃO���[�v�ԍ�������������������擾����
	std::string str = StrUtil::GetConcatenateNumAndStrings(name, ".", groupNum);

	//���O�ƋL���Ɣԍ�������������������擾����
	str = StrUtil::GetConcatenateNumAndStrings(str, "-", num);

	//�z�u�f�[�^�̎擾
	ObjectInfo objInfo = file.GetSpecifiedGimmickInfo(str);

	return objInfo;
}

//���݂̃G���x�[�^�[�̃X�e�[�^�X��ݒ肷��
void Elevator::SetInitState()
{
	//��~�|�W�V�����Ƃ̋������擾
	float upperDistanceSize = MathUtil::GetSizeOfDistanceTwoPoints(pos_, stopPos_[ElevatorState::Upper]);
	float lowerDistanceSize = MathUtil::GetSizeOfDistanceTwoPoints(pos_, stopPos_[ElevatorState::Lower]);

	//���݂̃|�W�V�����ƒ�~�|�W�V�����Ƃ̋������߂����̏�Ԃ�ۑ�����
	if (upperDistanceSize > lowerDistanceSize)
	{
		state_ = ElevatorState::Lower;
	}
	else
	{
		state_ = ElevatorState::Upper;
	}
}

//�G���x�[�^�[���ړ�������
float Elevator::Move()
{
	//1�t���[���O��Y���W�̃x�N�g�����擾����
	float oldPosY = pos_.y;
	
	//���Ԃ̍X�V
	elapsedTime_ = (std::min)(elapsedTime_ + add_time, total_time);

	//���W�̈ړ�
	pos_.y = Easing::InOutCubic(elapsedTime_, total_time, targetPos_.y, pos_.y);

	//�ړ��I����A�j���[�V������ύX����
	if (elapsedTime_ == total_time && !isStopping_)
	{
		isStopping_ = true;
		PlayDoorSound();
		model_->ChangeAnimation(static_cast<int>(ElevatorAnimType::Open), false, false, anim_cange_frame);
	}

	if (model_->IsAnimEnd() && model_->GetCurrentAnimNo() == static_cast<int>(ElevatorAnimType::Open))
	{
		model_->ChangeAnimation(static_cast<int>(ElevatorAnimType::OpenIdle), true, false, anim_cange_frame);
	}

	//�|�W�V�����̐ݒ�
	model_->SetPos(pos_);

	//�X�C�b�`�̃|�W�V�����̐ݒ�
	movingLever_->GetModelPointer()->SetPos(VGet(pos_.x, pos_.y + correction_y, pos_.z));

	//���o�[�����������ʒu�̎擾
	VECTOR standingPos = movingLever_->GetStandingPosition();

	//���o�[�����������ʒu�̐ݒ�
	movingLever_->SetStandingPos(VGet(standingPos.x, pos_.y + correction_y, standingPos.z));

	//�ߋ��ƌ��݂̃|�W�V�����̍����擾����
	float upVecY = pos_.y - oldPosY;

	return upVecY;
}

//�ړ���̃|�W�V�������擾����
void Elevator::TargetPosition()
{
	if (!isDeparture_)
	{
		return;
	}

	//�T�E���h��炷
	PlayDoorSound();

	//�o�ߎ��Ԃ�0�ɂ���
	elapsedTime_ = 0;

	//��~���Ă���t���O��false�ɂ���
	isStopping_ = false;

	//�A�j���[�V������ύX
	model_->ChangeAnimation(static_cast<int>(ElevatorAnimType::Close), false, false, anim_cange_frame);

	//elevator�̏�Ԃɂ����
	//elevator�̒�~�|�W�V������ύX����
	if (state_ == ElevatorState::Upper)
	{
		targetPos_ = stopPos_[ElevatorState::Lower];
		state_ = ElevatorState::Lower;
	}
	else
	{
		targetPos_ = stopPos_[ElevatorState::Upper];
		state_ = ElevatorState::Upper;
	}

	//�o���ł���t���O��false�ɂ���
	isDeparture_ = false;
}