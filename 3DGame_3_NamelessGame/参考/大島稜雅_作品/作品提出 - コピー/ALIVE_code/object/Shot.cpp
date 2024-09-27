#include "Shot.h"

#include "Player.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/SoundManager.h"
#include "../util/EffectManager.h"

namespace {
	//�������C��
	constexpr float vanishing_line = 1500.0f;

	//�d��
	constexpr float gravity = -1.0f;

	//���̔��a
	constexpr float sphere_radius = 30.0f;

	//�J�v�Z���̔��a
	constexpr float capsule_radius = 20.0f;

	//�m�b�N�o�b�N��
	constexpr float knockback_rate = 10.0f;

	//������������͈�
	constexpr float sound_range = 1500.0f;

	//�G�t�F�N�g�̃T�C�Y
	constexpr float effect_size = 100.0f;

	//�΂̃T�C�Y
	constexpr float stone_size = 20.0f;
}

//�R���X�g���N�^
Shot::Shot(const int handle, const VECTOR& initPos, const VECTOR& moveVec)
{
	//���f���̃C���X�^���X��
	model_ = std::make_shared<Model>(handle,Material::Stone);

	//�T�C�Y�̒���
	float scale = stone_size;
	model_->SetScale(VGet(scale, scale, scale));

	//�|�W�V����
	pos_ = initPos;

	//�����ʒu
	initialisePos_ = initPos;

	//�x�N�g��
	moveVec_ = moveVec;

	//���݂��Ă���t���O
	isEnable_ = true;
}

//�f�X�g���N�^
Shot::~Shot()
{
}

//�X�V
void Shot::Update()
{
	//�|�W�V�����̍X�V
	pos_ = VAdd(pos_, moveVec_);

	//���f���̃|�W�V������ݒ�
	model_->SetPos(pos_);

	//�����T�C�Y
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(initialisePos_, pos_);

	//�����T�C�Y����胉�C���𒴂���Ƒ��݂��Ă���t���O��false�ɂ���
	if (distanceSize > vanishing_line)
	{
		isEnable_ = false;
	}

	//���f���̍X�V
	model_->Update();
}

//�`��
void Shot::Draw()
{
	model_->Draw();
}

//�Փ˔���
void Shot::HitCheck(Player& player)
{
	//�v���C���[�̍��W
	VECTOR playerPos = player.GetStatus().pos;

	//�v���C���[�̍���
	float playerHeight = player.GetStatus().height;

	//�v���C���[�ƃV���b�g�̓����蔻��
	//�J�v�Z���Ƌ���
	bool hit = HitCheck_Sphere_Capsule(pos_, sphere_radius, playerPos, VGet(playerPos.x, playerPos.y + playerHeight, playerPos.z),capsule_radius);

	//�Փ˂�����m�b�N�o�b�N�x�N�g�������
	// ����炵��
	//�v���C���[�̃x�N�g���ɐݒ肵�A���݂���t���O��false�ɂ���
	if (hit) 
	{
		VECTOR nockBack = VScale(VNorm(moveVec_), knockback_rate);
		player.BulletHitMe(nockBack);

		//�T�E���h��炷
		SoundManager::GetInstance().Set3DSoundInfo(playerPos, sound_range, "hit");
		SoundManager::GetInstance().PlaySE("hit");

		//�G�t�F�N�g���o��
		EffectManager::GetInstance().AddEffect("BigHit", effect_size, pos_);

		isEnable_ = false;
	}
}
