#include "Corpse.h"

#include "Player.h"

#include "../util/Model.h"

namespace {
	//色を変更するマテリアルの番号
	constexpr int change_material_color_num = 8;

	//モデルフレーム名
	const char* const coll_frame_death = "CollDown";

	//アニメーション名
	const char* const anim_name_death = "Death";
}

//コンストラクタ
Corpse::Corpse(const int handle, const Material materialType, const ObjectInfo& objInfo) : CharacterBase(handle, materialType,objInfo)
{
	//衝突判定を行う
	isCollCheck_ = true;

	//衝突判定を行うモデルフレームの更新が必要かの設定を行う
	isUpdateColl_ = true;

	//マテリアルの色を変更する
	MV1SetMaterialDifColor(model_->GetModelHandle(), change_material_color_num, GetColorF(1.0f,0.0f,0.0f,1.0f));

	//アニメーションの設定
	model_->SetAnimation(static_cast<int>(PlayerAnimType::Death), false, true);

	//アニメーションの指定の再生時間を設定する
	model_->SetAnimEndFrame(static_cast<int>(PlayerAnimType::Death));

	//衝突判定を行うことを設定する
	model_->SetUseCollision(isCollCheck_, isUpdateColl_,coll_frame_death);
}

//デストラクタ
Corpse::~Corpse()
{
}

//更新
void Corpse::Update(Player& player)
{
	pos_ = model_->GetPos();
}

//描画
void Corpse::Draw()
{
	model_->Draw();
}