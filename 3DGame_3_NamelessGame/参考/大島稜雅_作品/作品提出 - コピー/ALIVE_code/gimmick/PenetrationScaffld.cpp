#include "PenetrationScaffld.h"

#include "../util/Model.h"

#include "../object/Player.h"

//�R���X�g���N�^
PenetrationScaffld::PenetrationScaffld(const int handle,const Material materialType,const ObjectInfo& objInfo):GimmickBase(handle, materialType, objInfo)
{
	//���f���̃}�e���A���̃A���t�@�l��0�ɂ��ē��߂�����
	materialNum_ = MV1GetMaterialNum(model_->GetModelHandle());

	//�����蔻����s��Ȃ�
	isCollCheck_ = false;

	//�}�e���A���̐F��ύX����
	ChangeMaterialColor();
}

//�f�X�g���N�^
PenetrationScaffld::~PenetrationScaffld()
{
}

//�`��
void PenetrationScaffld::Draw()
{
	model_->Draw();
}

//�}�e���A���̐F��ύX����
void PenetrationScaffld::ChangeMaterialColor()
{
	//�J���[�ϐ�
	COLOR_F color = {};

	//�}�e���A���̃A���t�@�l��ύX����
	for (int i = 0; i < materialNum_; i++)
	{
		color = MV1GetMaterialDifColor(model_->GetModelHandle(), i);
		color.a = alphaValue_;
		MV1SetMaterialDifColor(model_->GetModelHandle(), i, color);
	}
}
