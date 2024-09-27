#include "Switch.h"

#include "../object/Player.h"
#include "../object/ObjectBase.h"

#include "../util/Model.h"
#include "../util/SoundManager.h"
#include "../util/ModelManager.h"

namespace
{
	//色を変更するマテリアルの番号
	constexpr int change_material_color_num = 1;

	//アニメーションが変わる時間
	constexpr int anim_change_time = 10;

	//総時間
	constexpr float total_time = 180.0f;

	//サウンドが聞こえる範囲
	constexpr float sound_range = 1500.0f;

	//球の半径
	constexpr float sphere_radius = 30.0f;

	//赤色
	constexpr COLOR_F red = { 1.0f, 0.0f, 0.0f, 1.0f };

	//青色
	constexpr COLOR_F blue = { 0.0f, 0.0f, 1.0f, 1.0f };
}

//コンストラクタ
Switch::Switch(const ObjectInfo& objInfo)
{
	//モデルクラスの初期化
	model_ = std::make_shared<Model>(ModelManager::GetInstance().GetModelHandle(objData_[static_cast<int>(ObjectType::Switch)].name), Material::Iron);
	model_->SetScale(objInfo.scale);
	model_->SetPos(objInfo.pos);
	model_->SetRot(objInfo.rot);
	model_->SetUseCollision(true,true);

	//ポジションの初期化
	pos_ = objInfo.pos;

	//メンバ関数ポインタをoffAnimにする
	stateFunc_ = &Switch::OffAnim;
}

//デストラクタ
Switch::~Switch()
{
}

//更新
void Switch::Update(Player& player)
{
	//アニメーションの更新
	model_->Update();

	//衝突判定
	HitCollPlayer(player);

	(this->*stateFunc_)();
}

//描画
void Switch::Draw()
{
	model_->Draw();
}

//衝突結果の削除
void Switch::DeleteHitResult()
{
	//衝突結果の削除
	for (const auto& result : hitDim_) 
	{
		MV1CollResultPolyDimTerminate(result);
	}

	//衝突結果を保持する配列の削除
	hitDim_.clear();
}

//起動中かを変更する
void Switch::ChangeDuringStartup(const float time)
{
	//短縮化
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

//サウンドを鳴らす
void Switch::PlayOnSound()
{
	if (stateFunc_ == &Switch::OffAnim)
	{
		SoundManager::GetInstance().Set3DSoundInfo(pos_, sound_range, "switchOn");
		SoundManager::GetInstance().PlaySE("switchOn");
	}
}

//衝突判定
void Switch::HitCollPlayer(Player& player)
{
	//プレイヤーのポジション
	VECTOR playerPos = player.GetStatus().pos;

	//プレイヤーの位置情報を元にしたカプセルとスイッチモデルの判定
	hitDim_.push_back(MV1CollCheck_Sphere(model_->GetModelHandle(), model_->GetColFrameIndex(), playerPos, sphere_radius));
}

//スイッチモデルと死体の衝突判定を行う
void Switch::HitColl(const std::shared_ptr<ObjectBase>& corpse)
{
	//持ち運び中だったら以降の処理を行わない
	if (corpse->GetIsTransit()) 
	{
		return;
	}

	//プレイヤーの位置情報を元にしたカプセルとスイッチモデルの判定
	hitDim_.push_back(MV1CollCheck_Sphere(corpse->GetModelPointer()->GetModelHandle(), corpse->GetModelPointer()->GetColFrameIndex(), pos_, sphere_radius));
}

//衝突判定結果の初期化
bool Switch::ElevatorCollResult()
{
	//当たっている数を数える
	for (const auto& result : hitDim_)
	{
		if (result.HitNum > 0)
		{
			isDuringStartup_ = true;
		}
	}

	DeleteHitResult();

	//当たっていなかったら
	//アニメーションを変更し終了
	if (!isDuringStartup_) 
	{
		return false;
	}

	//音を鳴らす
	PlayOnSound();

	stateFunc_ = &Switch::OnAnim;

	return true;
}

//透過オブジェクトの衝突判定の結果
bool Switch::TransCollResult()
{
	int hitNum = 0;

	//当たっている数を数える
	for (const auto& result : hitDim_) 
	{
		if (result.HitNum > 0)
		{
			hitNum++;
		}
	}

	DeleteHitResult();

	//当たっていなかったら
	//アニメーションを変更し終了
	if (hitNum < 1)
	{
		stateFunc_ = &Switch::OffAnim;
		return false;
	}

	//音を鳴らす
	PlayOnSound();

	stateFunc_ = &Switch::OnAnim;

	return true;
}

//衝突判定を行うモデルの追加
const std::shared_ptr<Model>& Switch::GetModelPointer() const
{
	return model_;
}

//スイッチオンアニメーション
void Switch::OnAnim()
{
	model_->ChangeAnimation(static_cast<int>(Anim::On), false, false, anim_change_time);
	MV1SetMaterialDifColor(model_->GetModelHandle(), change_material_color_num, blue);
}

//スイッチオフアニメーション
void Switch::OffAnim()
{
	model_->ChangeAnimation(static_cast<int>(Anim::Off), false, false, anim_change_time);
	MV1SetMaterialDifColor(model_->GetModelHandle(), change_material_color_num, red);
}
