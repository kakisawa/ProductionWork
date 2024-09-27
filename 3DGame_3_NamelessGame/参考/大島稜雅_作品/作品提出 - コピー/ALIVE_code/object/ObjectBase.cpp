#include "ObjectBase.h"

#include "Player.h"

#include "../util/Model.h"

//�R���X�g���N�^
ObjectBase::ObjectBase(const int handle, const Material materialType, const ObjectInfo& objInfo)
{
	//�|�W�V����
	pos_ = objInfo.pos;

	//�g�k
	scale_ = objInfo.scale;

	//��]
	rot_ = objInfo.rot;

	//���f���̐ݒ�
	model_ = std::make_shared<Model>(handle,materialType);
	model_->SetPos(pos_);
	model_->SetRot(rot_);
	model_->SetScale(scale_);

	//���݂��Ă���t���O
	isEnable_ = true;
}

//�f�X�g���N�^
ObjectBase::~ObjectBase()
{
}

//�X�V
void ObjectBase::Update(Player& player)
{
	model_->Update();
}

//�`��
void ObjectBase::Draw()
{
	model_->Draw();
}

//���̂ɑ΂��Ă̍X�V
void ObjectBase::UpdateForCorpse(const std::shared_ptr<ObjectBase>& pointer)
{
}

//�Փ˔�����s�����f����ǉ�����
std::shared_ptr<Model> ObjectBase::AddCollModel() const
{
	return nullptr;
}

//���f���̃X�}�[�g�|�C���^���擾����
const std::shared_ptr<Model>& ObjectBase::GetModelPointer()const
{
	return model_;
}