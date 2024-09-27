#include "OrnamentBase.h"

#include "../util/Model.h"

//�R���X�g���N�^
OrnamentBase::OrnamentBase(const int handle, const Material materialType, const ObjectInfo objInfo) : ObjectBase(handle, materialType,objInfo)
{
	//�Փ˔�����s��
	isCollCheck_ = true;

	//�Փ˔�����s�����f���t���[���̍X�V���K�v���̐ݒ���s��
	isUpdateColl_ = false;

	//�Փ˔���p�t���[���̐ݒ�
	model_->SetUseCollision(isCollCheck_, isUpdateColl_);
}

//�f�X�g���N�^
OrnamentBase::~OrnamentBase()
{
}