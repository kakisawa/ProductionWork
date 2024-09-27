#include "Switch.h"

#include "../object/Player.h"
#include "../object/ObjectBase.h"

#include "../util/Model.h"
#include "../util/SoundManager.h"
#include "../util/ModelManager.h"

namespace
{
	//�F��ύX����}�e���A���̔ԍ�
	constexpr int change_material_color_num = 1;

	//�A�j���[�V�������ς�鎞��
	constexpr int anim_change_time = 10;

	//������
	constexpr float total_time = 180.0f;

	//�T�E���h����������͈�
	constexpr float sound_range = 1500.0f;

	//���̔��a
	constexpr float sphere_radius = 30.0f;

	//�ԐF
	constexpr COLOR_F red = { 1.0f, 0.0f, 0.0f, 1.0f };

	//�F
	constexpr COLOR_F blue = { 0.0f, 0.0f, 1.0f, 1.0f };
}

//�R���X�g���N�^
Switch::Switch(const ObjectInfo& objInfo)
{
	//���f���N���X�̏�����
	model_ = std::make_shared<Model>(ModelManager::GetInstance().GetModelHandle(objData_[static_cast<int>(ObjectType::Switch)].name), Material::Iron);
	model_->SetScale(objInfo.scale);
	model_->SetPos(objInfo.pos);
	model_->SetRot(objInfo.rot);
	model_->SetUseCollision(true,true);

	//�|�W�V�����̏�����
	pos_ = objInfo.pos;

	//�����o�֐��|�C���^��offAnim�ɂ���
	stateFunc_ = &Switch::OffAnim;
}

//�f�X�g���N�^
Switch::~Switch()
{
}

//�X�V
void Switch::Update(Player& player)
{
	//�A�j���[�V�����̍X�V
	model_->Update();

	//�Փ˔���
	HitCollPlayer(player);

	(this->*stateFunc_)();
}

//�`��
void Switch::Draw()
{
	model_->Draw();
}

//�Փˌ��ʂ̍폜
void Switch::DeleteHitResult()
{
	//�Փˌ��ʂ̍폜
	for (const auto& result : hitDim_) 
	{
		MV1CollResultPolyDimTerminate(result);
	}

	//�Փˌ��ʂ�ێ�����z��̍폜
	hitDim_.clear();
}

//�N��������ύX����
void Switch::ChangeDuringStartup(const float time)
{
	//�Z�k��
	auto& sound = SoundManager::GetInstance();

	if (time == total_time)
	{
		if (!isDuringStartup_) 
		{
			stateFunc_ = &Switch::OffAnim;
		}
		isDuringStartup_ = false;
	}
}

//�T�E���h��炷
void Switch::PlayOnSound()
{
	if (stateFunc_ == &Switch::OffAnim)
	{
		SoundManager::GetInstance().Set3DSoundInfo(pos_, sound_range, "switchOn");
		SoundManager::GetInstance().PlaySE("switchOn");
	}
}

//�Փ˔���
void Switch::HitCollPlayer(Player& player)
{
	//�v���C���[�̃|�W�V����
	VECTOR playerPos = player.GetStatus().pos;

	//�v���C���[�̈ʒu�������ɂ����J�v�Z���ƃX�C�b�`���f���̔���
	hitDim_.push_back(MV1CollCheck_Sphere(model_->GetModelHandle(), model_->GetColFrameIndex(), playerPos, sphere_radius));
}

//�X�C�b�`���f���Ǝ��̂̏Փ˔�����s��
void Switch::HitColl(const std::shared_ptr<ObjectBase>& corpse)
{
	//�����^�ђ���������ȍ~�̏������s��Ȃ�
	if (corpse->GetIsTransit()) 
	{
		return;
	}

	//�v���C���[�̈ʒu�������ɂ����J�v�Z���ƃX�C�b�`���f���̔���
	hitDim_.push_back(MV1CollCheck_Sphere(corpse->GetModelPointer()->GetModelHandle(), corpse->GetModelPointer()->GetColFrameIndex(), pos_, sphere_radius));
}

//�Փ˔��茋�ʂ̏�����
bool Switch::ElevatorCollResult()
{
	//�������Ă��鐔�𐔂���
	for (const auto& result : hitDim_)
	{
		if (result.HitNum > 0)
		{
			isDuringStartup_ = true;
		}
	}

	DeleteHitResult();

	//�������Ă��Ȃ�������
	//�A�j���[�V������ύX���I��
	if (!isDuringStartup_) 
	{
		return false;
	}

	//����炷
	PlayOnSound();

	stateFunc_ = &Switch::OnAnim;

	return true;
}

//���߃I�u�W�F�N�g�̏Փ˔���̌���
bool Switch::TransCollResult()
{
	int hitNum = 0;

	//�������Ă��鐔�𐔂���
	for (const auto& result : hitDim_) 
	{
		if (result.HitNum > 0)
		{
			hitNum++;
		}
	}

	DeleteHitResult();

	//�������Ă��Ȃ�������
	//�A�j���[�V������ύX���I��
	if (hitNum < 1)
	{
		stateFunc_ = &Switch::OffAnim;
		return false;
	}

	//����炷
	PlayOnSound();

	stateFunc_ = &Switch::OnAnim;

	return true;
}

//�Փ˔�����s�����f���̒ǉ�
const std::shared_ptr<Model>& Switch::GetModelPointer() const
{
	return model_;
}

//�X�C�b�`�I���A�j���[�V����
void Switch::OnAnim()
{
	model_->ChangeAnimation(static_cast<int>(Anim::On), false, false, anim_change_time);
	MV1SetMaterialDifColor(model_->GetModelHandle(), change_material_color_num, blue);
}

//�X�C�b�`�I�t�A�j���[�V����
void Switch::OffAnim()
{
	model_->ChangeAnimation(static_cast<int>(Anim::Off), false, false, anim_change_time);
	MV1SetMaterialDifColor(model_->GetModelHandle(), change_material_color_num, red);
}
