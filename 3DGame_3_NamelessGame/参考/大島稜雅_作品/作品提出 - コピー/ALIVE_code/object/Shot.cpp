#include "Shot.h"

#include "Player.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/SoundManager.h"
#include "../util/EffectManager.h"

namespace {
	//消失ライン
	constexpr float vanishing_line = 1500.0f;

	//重力
	constexpr float gravity = -1.0f;

	//球の半径
	constexpr float sphere_radius = 30.0f;

	//カプセルの半径
	constexpr float capsule_radius = 20.0f;

	//ノックバック率
	constexpr float knockback_rate = 10.0f;

	//音が聞こえる範囲
	constexpr float sound_range = 1500.0f;

	//エフェクトのサイズ
	constexpr float effect_size = 100.0f;

	//石のサイズ
	constexpr float stone_size = 20.0f;
}

//コンストラクタ
Shot::Shot(const int handle, const VECTOR& initPos, const VECTOR& moveVec)
{
	//モデルのインスタンス化
	model_ = std::make_shared<Model>(handle,Material::Stone);

	//サイズの調整
	float scale = stone_size;
	model_->SetScale(VGet(scale, scale, scale));

	//ポジション
	pos_ = initPos;

	//初期位置
	initialisePos_ = initPos;

	//ベクトル
	moveVec_ = moveVec;

	//存在しているフラグ
	isEnable_ = true;
}

//デストラクタ
Shot::~Shot()
{
}

//更新
void Shot::Update()
{
	//ポジションの更新
	pos_ = VAdd(pos_, moveVec_);

	//モデルのポジションを設定
	model_->SetPos(pos_);

	//距離サイズ
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(initialisePos_, pos_);

	//距離サイズが一定ラインを超えると存在しているフラグをfalseにする
	if (distanceSize > vanishing_line)
	{
		isEnable_ = false;
	}

	//モデルの更新
	model_->Update();
}

//描画
void Shot::Draw()
{
	model_->Draw();
}

//衝突判定
void Shot::HitCheck(Player& player)
{
	//プレイヤーの座標
	VECTOR playerPos = player.GetStatus().pos;

	//プレイヤーの高さ
	float playerHeight = player.GetStatus().height;

	//プレイヤーとショットの当たり判定
	//カプセルと球体
	bool hit = HitCheck_Sphere_Capsule(pos_, sphere_radius, playerPos, VGet(playerPos.x, playerPos.y + playerHeight, playerPos.z),capsule_radius);

	//衝突したらノックバックベクトルを作り
	// 音を鳴らして
	//プレイヤーのベクトルに設定し、存在するフラグをfalseにする
	if (hit) 
	{
		VECTOR nockBack = VScale(VNorm(moveVec_), knockback_rate);
		player.BulletHitMe(nockBack);

		//サウンドを鳴らす
		SoundManager::GetInstance().Set3DSoundInfo(playerPos, sound_range, "hit");
		SoundManager::GetInstance().PlaySE("hit");

		//エフェクトを出す
		EffectManager::GetInstance().AddEffect("BigHit", effect_size, pos_);

		isEnable_ = false;
	}
}
