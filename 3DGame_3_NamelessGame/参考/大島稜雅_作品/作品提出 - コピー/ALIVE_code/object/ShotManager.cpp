#include "ShotManager.h"

#include "../object/Shot.h"
#include "../object/ObjectData.h"

#include "../util/ModelManager.h"

namespace 
{
	//����
	constexpr int half = 2;

	//�������̑��x
	constexpr float shot_speed = 20.0f;
}

//�R���X�g���N�^
ShotManager::ShotManager()
{
}

//�f�X�g���N�^
ShotManager::~ShotManager()
{
}

//�X�V
void ShotManager::Update()
{
	//�e�̍X�V
	for (const auto& shot : shots_)
	{
		if (shot->GetIsEnabled())
		{
			shot->Update();
		}
	}

	//���݂��Ă���t���O��false�̒e������
	shots_.remove_if([](std::shared_ptr<Shot> shot) {return !shot->GetIsEnabled(); });
}

//�`��
void ShotManager::Draw()
{
	//�e�̕`��
	for (const auto& shot : shots_)
	{
		if (shot->GetIsEnabled()) 
		{
			shot->Draw();
		}
	}
}

//�e���v���C���[�ƏՓ˂������`�F�b�N����
void ShotManager::Hit(Player& player)
{
	//�e�ƃv���C���[�̏Փ˔���
	for (const auto shot : shots_)
	{
		if (shot->GetIsEnabled()) 
		{
			shot->HitCheck(player);
		}
	}
}

//�e������
void ShotManager::Fire(const VECTOR& framePos, const  VECTOR& playerPos, const float height)
{
	//�v���C���[�߂����ăV���b�g������
	VECTOR distance = VSub(VGet(playerPos.x, playerPos.y + height / half, playerPos.z), framePos);
	VECTOR moveVec = VScale(VNorm(distance), shot_speed);

	shots_.push_back(std::make_shared<Shot>(ModelManager::GetInstance().GetModelHandle(objData_[static_cast<int>(ObjectType::Rock)].name), framePos, moveVec));
}
