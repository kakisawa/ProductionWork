#include "TransparentObject.h"
#include "Switch.h"
#include "../util/Model.h"
#include "../util/ExternalFile.h"
#include "../util/Util.h"
#include <algorithm>

namespace
{
	//�A���t�@�l�̑���
	constexpr float add_alpha = 0.1f;

	//�A���t�@�l�̍ő�l
	constexpr float max_alpha_value = 1.0f;

	//�A���t�@�l�̍ŏ�
	constexpr float min_alpha_value = 0.1f;
}

//�R���X�g���N�^
TransparentObject::TransparentObject(const int handle, const Material materialType, const ObjectInfo& objInfo) : GimmickBase(handle, materialType, objInfo)
{
	//�Փ˔���p�t���[���̐ݒ�
	model_->SetUseCollision(true, false);

	//������̖����𐔒l�Ƃ��Ď擾����
	int num = StrUtil::GetNumberFromString(objInfo.name, ".");
	//�������Ɛ��l��A����������������擾
	std::string name = StrUtil::GetConcatenateNumAndStrings("TransSwitch", ".", num);

	//�f�[�^�̎擾
	auto info = ExternalFile::GetInstance().GetSpecifiedGimmickInfo(name);
	//�X�C�b�`�̃C���X�^���X��
	switch_ = std::make_shared<Switch>(info);

	//�Փ˔�����s��Ȃ���Ԃŏ�����
	isCollCheck_ = false;

	//���f���̃}�e���A���̃A���t�@�l��0�ɂ��ē��߂�����
	materialNum_ = MV1GetMaterialNum(model_->GetModelHandle());

	COLOR_F color = {};

	for (int i = 0; i < materialNum_;i++) 
	{
		color = MV1GetMaterialDifColor(model_->GetModelHandle(), i);
		color.a = 0.0f;
		MV1SetMaterialDifColor(model_->GetModelHandle(), i, color);
	}
}

//�f�X�g���N�^
TransparentObject::~TransparentObject()
{
}

//�X�V
void TransparentObject::Update(Player& player)
{
	switch_->Update(player);

	//�X�C�b�`���N�����Ă�����
	//���f���Ƃ̏Փ˔�����s���悤�ɂ���
	if (switch_->TransCollResult())
	{
		isCollCheck_ = true;
	}
	else {
		isCollCheck_ = false;
	}

	//�A���t�@�l��ύX����
	if (isCollCheck_)
	{
		alphaValue_ = (std::min)(alphaValue_ + add_alpha, max_alpha_value);
	}
	else 
	{
		alphaValue_ = (std::max)(alphaValue_ - add_alpha, min_alpha_value);
	}

	COLOR_F color = {};

	for (int i = 0; i < materialNum_; i++)
	{
		color = MV1GetMaterialDifColor(model_->GetModelHandle(), i);
		color.a = alphaValue_;
		MV1SetMaterialDifColor(model_->GetModelHandle(), i, color);
	}
}

//�`��
void TransparentObject::Draw()
{
	model_->Draw();

	switch_->Draw();
}

//�X�C�b�`���f���Ǝ��̂̏Փ˔�����s��
void TransparentObject::UpdateForCorpse(const std::shared_ptr<ObjectBase>& deadPerson)
{
	switch_->HitColl(deadPerson);
}

//�Փ˔�����s�����f���̒ǉ�
std::shared_ptr<Model> TransparentObject::AddCollModel() const
{
	return switch_->GetModelPointer();
}
