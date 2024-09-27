#include "Lever.h"

#include "../util/Model.h"
#include "../util/Util.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"
#include "../util/ModelManager.h"

namespace 
{
	//�A�j���[�V�����̕ύX����
	constexpr int anim_change_time = 10;

	//�Փ˔���p���̔��a
	constexpr float radius_sphere = 100.0f;
}

//�R���X�g���N�^
Lever::Lever(const ObjectInfo& info)
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();
	auto& model = ModelManager::GetInstance();

	//���o�[���ǂ̃G���x�[�^�[�O���[�v�ɑ����Ă��邩�擾����
	int groupNum = StrUtil::GetNumberFromString(info.name, ".");

	//�擾�����O���[�v�̉��Ԗڂ̃��o�[�����擾����
	int num = StrUtil::GetNumberFromString(info.name, "-");

	//�����ʒu�̎擾
	ObjectInfo standingData = GetSpecialNameObjectInfo("LeverStandingPos", "-", groupNum, num);

	//���f���N���X�̃C���X�^���X��
	model_ = std::make_shared<Model>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Lever)].name), Material::Iron);

	//���f���̏�����
	model_->SetPos(info.pos);
	model_->SetScale(info.scale);
	model_->SetRot(info.rot);

	//�|�W�V�����̏�����
	pos_ = info.pos;

	//���ʒu�̏�����
	standingPos_ = standingData.pos;

	model_->SetUseCollision(true, true);
}

//�f�X�g���N�^
Lever::~Lever()
{
}

//�X�V
void Lever::Update()
{
	//�X�V
	model_->Update();

	//�A�j���[�V�������~�A�j���[�V�����ɂ���
	if (model_->IsAnimEnd())
	{
		OffAnimation();
	}
}

//�`��
void Lever::Draw()
{
	//�`��
	model_->Draw();
}

//�Փ˔���
bool Lever::CollCheck(const VECTOR& playerPos) const
{
	//�v���C���[�ƃ��o�[���f���̏Փ˔���
	auto result = MV1CollCheck_Sphere(model_->GetModelHandle(), model_->GetColFrameIndex(), playerPos, radius_sphere);

	//�Փ˔��肪1�ȏ゠������true��Ԃ�
	if (result.HitNum > 0)
	{
		MV1CollResultPolyDimTerminate(result);
		return true;
	}

	return false;
}

//���o�[�̋N��
void Lever::OnAnimation()
{
	model_->ChangeAnimation(static_cast<int>(AnimType::on), false, false, anim_change_time);
	isOn_ = true;
}

//���o�[�̒�~
void Lever::OffAnimation()
{
	model_->ChangeAnimation(static_cast<int>(AnimType::off), false, false, anim_change_time);
	isOn_ = false;
}

//����Ȗ��O�̃I�u�W�F�N�g�̔z�u�f�[�^���擾����
ObjectInfo Lever::GetSpecialNameObjectInfo(const std::string& name, const std::string& sign, int groupNum, int num)
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