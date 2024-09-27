#include "ThrowEnemy.h"

#include "../object/Player.h"
#include "../object/ShotManager.h"

#include "../util/Util.h"
#include "../util/Model.h"

namespace
{
	//色を変えるマテリアルの番号
	constexpr int change_material_num = 8;

	//物を投げているときのアニメーションフレーム
	constexpr int throw_frame_time = 73;

	//アニメーションを変更するのにかかる時間
	constexpr int anim_change_time = 10;

	//右手のフレーム名
	const char* const hand_framename = "hand.R_end";

	//色
	constexpr COLOR_F color = { 1.0f, 0.0f, 0.0f, 1.0f };
}

//コンストラクタ
 ThrowEnemy::ThrowEnemy(const int handle, const Material materialType, const ObjectInfo& objInfo):EnemyBase(handle, materialType, objInfo)
{
	 //衝突判定の設定
	 isCollCheck_ = true;
	 model_->SetUseCollision(isCollCheck_, true);

	 //文字列の取得
	 std::string str = StrUtil::GetStringAfterSign(objInfo.name, "-");

	 //文字列がFakeだったら投げるふりをするフラグを立てる
	 if (str == "Fake")
	 {
		 isFakeThrow_ = true;
	 }

	 //マテリアルの色を変える
	 MV1SetMaterialDifColor(model_->GetModelHandle(), change_material_num, color);
}

 //デストラクタ
ThrowEnemy::~ThrowEnemy()
{
}

//更新
void ThrowEnemy::Update(Player& player)
{
	//プレイヤーの座標
	VECTOR playerPos = player.GetStatus().pos;

	//モデルの更新
	model_->Update();

	//プレイヤーを索敵する
	SearchForPlayer(playerPos);

	//投げるアニメーションが終わったら
	//投げているというフラグをfalseにする
	if (model_->IsAnimEnd() && isThrow_)
	{
		model_->ChangeAnimation(static_cast<int>(PlayerAnimType::Idle), true, false, anim_change_time);
		isThrow_ = false;
	}

	//プレイヤーを検知したら
	//プレイヤーの方向を見る
	if (isDetection_ && !isThrow_)
	{
		//プレイヤーと敵の距離
		VECTOR distance = VNorm(VSub(playerPos, pos_));

		//Y軸は考慮しない
		distance.y = 0.0f;

		//回転行列と拡縮行列の合成
		MATRIX mtx = CombiningRotAndScallMat(distance);

		//回転行列と拡縮行列を掛けた行列に平行移動行列をかける
		mtx = MMult(mtx, MGetTranslate(pos_));

		//行列をモデルにセットする
		MV1SetMatrix(model_->GetModelHandle(), mtx);
	}
}

//描画
void ThrowEnemy::Draw()
{
	//モデルの描画
	model_->Draw();

	//落ち影の描画
	DrawPolygon3D();
}

//弾を撃つ処理
void ThrowEnemy::Shot(const std::shared_ptr<ShotManager>& shotManager, const VECTOR& playerPos, const float height)
{
	//プレイヤーを検知しているかどうか
	//検知していなかったらreturn
	if (!isDetection_)
	{
		if (model_->GetCurrentAnimNo() != static_cast<int>(PlayerAnimType::Throw))
		{
			return;
		}
	}

	//アニメーションを投げるアニメーションに変更する
	model_->ChangeAnimation(static_cast<int>(PlayerAnimType::Throw), false, false, anim_change_time);

	//投げるふりをする敵だった場合リターンする
	if (isFakeThrow_)
	{
		return;
	}

	//投げているアニメーションの特定フレームで
	//弾を発射する
	if (model_->GetSpecifiedAnimTime(throw_frame_time))
	{
		VECTOR framePos = model_->GetFrameLocalPosition(hand_framename);
		shotManager->Fire(framePos, playerPos, height);
		isThrow_ = true;
	}
}