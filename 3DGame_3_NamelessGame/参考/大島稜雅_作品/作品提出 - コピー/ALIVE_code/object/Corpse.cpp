#include "Corpse.h"

#include "Player.h"

#include "../util/Model.h"

namespace {
	//�F��ύX����}�e���A���̔ԍ�
	constexpr int change_material_color_num = 8;

	//���f���t���[����
	const char* const coll_frame_death = "CollDown";

	//�A�j���[�V������
	const char* const anim_name_death = "Death";
}

//�R���X�g���N�^
Corpse::Corpse(const int handle, const Material materialType, const ObjectInfo& objInfo) : CharacterBase(handle, materialType,objInfo)
{
	//�Փ˔�����s��
	isCollCheck_ = true;

	//�Փ˔�����s�����f���t���[���̍X�V���K�v���̐ݒ���s��
	isUpdateColl_ = true;

	//�}�e���A���̐F��ύX����
	MV1SetMaterialDifColor(model_->GetModelHandle(), change_material_color_num, GetColorF(1.0f,0.0f,0.0f,1.0f));

	//�A�j���[�V�����̐ݒ�
	model_->SetAnimation(static_cast<int>(PlayerAnimType::Death), false, true);

	//�A�j���[�V�����̎w��̍Đ����Ԃ�ݒ肷��
	model_->SetAnimEndFrame(static_cast<int>(PlayerAnimType::Death));

	//�Փ˔�����s�����Ƃ�ݒ肷��
	model_->SetUseCollision(isCollCheck_, isUpdateColl_,coll_frame_death);
}

//�f�X�g���N�^
Corpse::~Corpse()
{
}

//�X�V
void Corpse::Update(Player& player)
{
	pos_ = model_->GetPos();
}

//�`��
void Corpse::Draw()
{
	model_->Draw();
}