#include "ManualCrank.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../object/Player.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"
#include "../util/ModelManager.h"

#include <algorithm>
#include <math.h>

namespace
{
	//最大回転値
	constexpr float max_rot_Z = 360.0f;

	//衝突判定用カプセルの半径
	constexpr float radius_capsule = 60.0f;

	//最小衝突数
	constexpr int min_hit_num = 1;

	//サウンドが聞こえる範囲
	constexpr float sound_range = 1500.0f;
}

//コンストラクタ
ManualCrank::ManualCrank(const ObjectInfo& objInfo)
{
	//モデルクラスの初期化
	model_ = std::make_shared<Model>(ModelManager::GetInstance().GetModelHandle(objData_[static_cast<int>(ObjectType::Crank)].name), Material::Iron);
	model_->SetUseCollision(true,false,"Foundation");
	model_->SetScale(objInfo.scale);
	model_->SetPos(objInfo.pos);

	//クランクを回す立ち位置を取得
	int standingNum = StrUtil::GetNumberFromString(objInfo.name, ".");
	ObjectInfo standingData = GetLoadObjectInfo("StandingPosition", standingNum);

	//立つ位置の初期化
	standingPos_ = standingData.pos;

	//ポジションの初期化
	pos_ = objInfo.pos;
}

//デストラクタ
ManualCrank::~ManualCrank()
{
}

//更新
void ManualCrank::Update()
{
	//サウンドを再生する
	PlayCrankSound();

	//1フレーム前の回転率を保存
	oldRotZ_ = rotZ_;
}

//描画
void ManualCrank::Draw()
{
	//描画
	model_->Draw();
}

//クランクの音を再生
void ManualCrank::PlayCrankSound()
{
	//短縮化
	auto& sound = SoundManager::GetInstance();

	//サウンドを再生することが出来るか
	if (CanPlaySound())
	{
		sound.Set3DSoundInfo(pos_, sound_range, "crank");

		//サウンドを再生
		sound.PlaySE("crank");
	}
}

//サウンドを再生することが出来るか
bool ManualCrank::CanPlaySound() const
{
	//短縮化
	auto& sound = SoundManager::GetInstance();

	if (oldRotZ_ == rotZ_)					return false;
	if(sound.CheckSoundFile("crank") == 1)  return false;

	return true;
}

//プレイヤーとの衝突判定
bool ManualCrank::HitCollPlayer(Player& player) const
{
	//衝突結果用変数
	MV1_COLL_RESULT_POLY_DIM result = {};

	//プレイヤーのポジション
	VECTOR playerPos = player.GetStatus().pos;

	//衝突用情報のリフレッシュ
	MV1RefreshCollInfo(model_->GetModelHandle(), model_->GetColFrameIndex());

	//衝突判定
	result = MV1CollCheck_Sphere(model_->GetModelHandle(), model_->GetColFrameIndex(), playerPos, radius_capsule);

	//衝突数が0以上だったらtrueを返す
	if (result.HitNum > min_hit_num)
	{
		//衝突判定の後始末
		MV1CollResultPolyDimTerminate(result);
		return true;
	}

	//衝突判定の後始末
	MV1CollResultPolyDimTerminate(result);

	return false;
}

//Z軸の最大回転率を取得する
float ManualCrank::GetMaxRotZ() const
{
	return max_rot_Z;
}

//指定のオブジェクトの配置データを取得する
ObjectInfo ManualCrank::GetLoadObjectInfo(const std::string& name, int num)
{
	//短縮化
	auto& file = ExternalFile::GetInstance();

	//名前に番号を足した文字列を取得する
	std::string str = StrUtil::GetConcatenateNumAndStrings(name, ".", num);

	//上記で取得した文字列のオブジェクトの配置データを取得する	
	ObjectInfo objInfo = file.GetSpecifiedGimmickInfo(str);

	//配置データを返す
	return objInfo;
}