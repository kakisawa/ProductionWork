#include "CrankScaffold.h"
#include "ManualCrank.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"

#include "../object/Player.h"

namespace
{
	//Z���̉�]�̍Œ�l
	constexpr int min_rot_Z = 0;

	//�T�E���h����������͈�
	constexpr float sound_range = 1500.0f;
}

//�R���X�g���N�^
CrankScaffold::CrankScaffold(const int handle, const Material materialType, const ObjectInfo& objInfo) : GimmickBase(handle, materialType, objInfo)
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	//objInfo�̖��O���疖���̐��l���擾����
	int num = StrUtil::GetNumberFromString(objInfo.name, ".");

	//�N�����N�̔z�u�f�[�^���擾����
	ObjectInfo crankData = GetLoadObjectInfo("Crank", num);

	//�C���X�^���X��
	crank_ = std::make_shared<ManualCrank>(crankData);

	//�㏸�x�N�g���̏�����
	upVecY_ = GetUpVec(num);

	//�Փ˔���p�t���[���̐ݒ�
	model_->SetUseCollision(true, false);

	//�����|�W�V�����̏�����
	initPos_ = objInfo.pos;
}

//�f�X�g���N�^
CrankScaffold::~CrankScaffold()
{
}

//�X�V
void CrankScaffold::Update(Player& player)
{
	//�N�����N�ƃv���C���[�̏Փ˔�����s��
	//�������Ă����ꍇ�v���C���[�ɓ������Ă����N���X�̃|�C���^�[��
	//�Z�b�g����
	if (crank_->HitCollPlayer(player))
	{
		player.SetCrankPointer(crank_);
	}

	//�N�����N�̍X�V
	crank_->Update();

	//����炷
	PlayStopCrankSound();

	//���f���̈ړ�
	MoveModel();

	//Z��]�̒l���c���Ă���
	oldRotZ_ = crank_->GetRotZ();
}

//�`��
void CrankScaffold::Draw()
{
	//���f���̕`��
	model_->Draw();

	//�N�����N�̕`��
	crank_->Draw();
}

//�T�E���h��炷
void CrankScaffold::PlayStopCrankSound()
{
	//�Z�k��
	auto& sound = SoundManager::GetInstance();

	//�T�E���h��炷�������������Ă��Ȃ������ꍇreturn
	if (!CanSound()) 
	{
		return;
	}

	//��~�T�E���h��炷
	sound.Set3DSoundInfo(pos_, sound_range, "stopCrank");
	sound.PlaySE("stopCrank");
}

//�T�E���h��炷���Ƃ��o���邩�̏�������֐�
bool CrankScaffold::CanSound()
{
	//�Z�k��
	auto& sound = SoundManager::GetInstance();

	//���݂�Z�̉�]���擾����
	float nowRotZ = crank_->GetRotZ();

	//�ȍ~�̏����𖞂����Ă��Ȃ�������T�E���h��炷���Ƃ͏o���Ȃ�
	if (oldRotZ_ == nowRotZ) return false;										//���݂̉�]�l�ƌ��݂̉�]�l�������Ȃ�false
	if (sound.CheckSoundFile("stopCrank")) return false;						//stopCrank�Ƃ��������Ȃ��Ă��Ȃ������̂Ȃ�false
	if (min_rot_Z != nowRotZ && crank_->GetMaxRotZ() != nowRotZ) return false;	//���݂̉�]�l���ŏ���]�l�܂��͍ő��]�l�ł��Ȃ��̂Ȃ�false

	//����炷�����������Ă���
	return true;
}

//���f���̈ړ�
void CrankScaffold::MoveModel()
{
	//�ړ�
	pos_.y = crank_->GetRotZ() * upVecY_ + initPos_.y;

	//�|�W�V�����̃Z�b�g
	model_->SetPos(pos_);
}

//����̏���Ɖ����̋����̃T�C�Y���擾����
float CrankScaffold::GetUpperAndLowerDistanceSize(int num)
{
	//����̏���̔z�u�f�[�^���擾����
	ObjectInfo upperLimitData = GetLoadObjectInfo("CrankUpperLimit", num);

	//����̉����̔z�u�f�[�^���擾����
	ObjectInfo lowerLimitData = GetLoadObjectInfo("CrankLowerLimit", num);

	//����Ɖ����̋������擾����
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(upperLimitData.pos, lowerLimitData.pos);

	//�����̃T�C�Y��Ԃ�
	return distanceSize;
}

//����̏㏸�x�N�g�����擾����
float CrankScaffold::GetUpVec(int num)
{
	//����Ɖ����̋������擾����
	float distanceSize = GetUpperAndLowerDistanceSize(num);

	//�㏸�x�N�g���̏�����
	float up = distanceSize / crank_->GetMaxRotZ();

	return up;
}
