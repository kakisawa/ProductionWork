#include "ShotManager.h"

#include "../object/Shot.h"
#include "../object/ObjectData.h"

#include "../util/ModelManager.h"

namespace 
{
	//半分
	constexpr int half = 2;

	//投擲物の速度
	constexpr float shot_speed = 20.0f;
}

//コンストラクタ
ShotManager::ShotManager()
{
}

//デストラクタ
ShotManager::~ShotManager()
{
}

//更新
void ShotManager::Update()
{
	//弾の更新
	for (const auto& shot : shots_)
	{
		if (shot->GetIsEnabled())
		{
			shot->Update();
		}
	}

	//存在しているフラグがfalseの弾を消す
	shots_.remove_if([](std::shared_ptr<Shot> shot) {return !shot->GetIsEnabled(); });
}

//描画
void ShotManager::Draw()
{
	//弾の描画
	for (const auto& shot : shots_)
	{
		if (shot->GetIsEnabled()) 
		{
			shot->Draw();
		}
	}
}

//弾がプレイヤーと衝突したかチェックする
void ShotManager::Hit(Player& player)
{
	//弾とプレイヤーの衝突判定
	for (const auto shot : shots_)
	{
		if (shot->GetIsEnabled()) 
		{
			shot->HitCheck(player);
		}
	}
}

//弾を撃つ
void ShotManager::Fire(const VECTOR& framePos, const  VECTOR& playerPos, const float height)
{
	//プレイヤーめがけてショットを撃つ
	VECTOR distance = VSub(VGet(playerPos.x, playerPos.y + height / half, playerPos.z), framePos);
	VECTOR moveVec = VScale(VNorm(distance), shot_speed);

	shots_.push_back(std::make_shared<Shot>(ModelManager::GetInstance().GetModelHandle(objData_[static_cast<int>(ObjectType::Rock)].name), framePos, moveVec));
}
