#include "ManualCrank.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../object/Player.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"
#include "../util/ModelManager.h"

#include <algorithm>
#include <math.h>

namespace
{
	//�ő��]�l
	constexpr float max_rot_Z = 360.0f;

	//�Փ˔���p�J�v�Z���̔��a
	constexpr float radius_capsule = 60.0f;

	//�ŏ��Փː�
	constexpr int min_hit_num = 1;

	//�T�E���h����������͈�
	constexpr float sound_range = 1500.0f;
}

//�R���X�g���N�^
ManualCrank::ManualCrank(const ObjectInfo& objInfo)
{
	//���f���N���X�̏�����
	model_ = std::make_shared<Model>(ModelManager::GetInstance().GetModelHandle(objData_[static_cast<int>(ObjectType::Crank)].name), Material::Iron);
	model_->SetUseCollision(true,false,"Foundation");
	model_->SetScale(objInfo.scale);
	model_->SetPos(objInfo.pos);

	//�N�����N���񂷗����ʒu���擾
	int standingNum = StrUtil::GetNumberFromString(objInfo.name, ".");
	ObjectInfo standingData = GetLoadObjectInfo("StandingPosition", standingNum);

	//���ʒu�̏�����
	standingPos_ = standingData.pos;

	//�|�W�V�����̏�����
	pos_ = objInfo.pos;
}

//�f�X�g���N�^
ManualCrank::~ManualCrank()
{
}

//�X�V
void ManualCrank::Update()
{
	//�T�E���h���Đ�����
	PlayCrankSound();

	//1�t���[���O�̉�]����ۑ�
	oldRotZ_ = rotZ_;
}

//�`��
void ManualCrank::Draw()
{
	//�`��
	model_->Draw();
}

//�N�����N�̉����Đ�
void ManualCrank::PlayCrankSound()
{
	//�Z�k��
	auto& sound = SoundManager::GetInstance();

	//�T�E���h���Đ����邱�Ƃ��o���邩
	if (CanPlaySound())
	{
		sound.Set3DSoundInfo(pos_, sound_range, "crank");

		//�T�E���h���Đ�
		sound.PlaySE("crank");
	}
}

//�T�E���h���Đ����邱�Ƃ��o���邩
bool ManualCrank::CanPlaySound() const
{
	//�Z�k��
	auto& sound = SoundManager::GetInstance();

	if (oldRotZ_ == rotZ_)					return false;
	if(sound.CheckSoundFile("crank") == 1)  return false;

	return true;
}

//�v���C���[�Ƃ̏Փ˔���
bool ManualCrank::HitCollPlayer(Player& player) const
{
	//�Փˌ��ʗp�ϐ�
	MV1_COLL_RESULT_POLY_DIM result = {};

	//�v���C���[�̃|�W�V����
	VECTOR playerPos = player.GetStatus().pos;

	//�Փ˗p���̃��t���b�V��
	MV1RefreshCollInfo(model_->GetModelHandle(), model_->GetColFrameIndex());

	//�Փ˔���
	result = MV1CollCheck_Sphere(model_->GetModelHandle(), model_->GetColFrameIndex(), playerPos, radius_capsule);

	//�Փː���0�ȏゾ������true��Ԃ�
	if (result.HitNum > min_hit_num)
	{
		//�Փ˔���̌�n��
		MV1CollResultPolyDimTerminate(result);
		return true;
	}

	//�Փ˔���̌�n��
	MV1CollResultPolyDimTerminate(result);

	return false;
}

//Z���̍ő��]�����擾����
float ManualCrank::GetMaxRotZ() const
{
	return max_rot_Z;
}

//�w��̃I�u�W�F�N�g�̔z�u�f�[�^���擾����
ObjectInfo ManualCrank::GetLoadObjectInfo(const std::string& name, int num)
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	//���O�ɔԍ��𑫂�����������擾����
	std::string str = StrUtil::GetConcatenateNumAndStrings(name, ".", num);

	//��L�Ŏ擾����������̃I�u�W�F�N�g�̔z�u�f�[�^���擾����	
	ObjectInfo objInfo = file.GetSpecifiedGimmickInfo(str);

	//�z�u�f�[�^��Ԃ�
	return objInfo;
}