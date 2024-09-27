#include "Lever.h"

#include "../util/Model.h"
#include "../util/Util.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"
#include "../util/ModelManager.h"

namespace 
{
	//アニメーションの変更時間
	constexpr int anim_change_time = 10;

	//衝突判定用球の半径
	constexpr float radius_sphere = 100.0f;
}

//コンストラクタ
Lever::Lever(const ObjectInfo& info)
{
	//短縮化
	auto& file = ExternalFile::GetInstance();
	auto& model = ModelManager::GetInstance();

	//レバーがどのエレベーターグループに属しているか取得する
	int groupNum = StrUtil::GetNumberFromString(info.name, ".");

	//取得したグループの何番目のレバーかを取得する
	int num = StrUtil::GetNumberFromString(info.name, "-");

	//立ち位置の取得
	ObjectInfo standingData = GetSpecialNameObjectInfo("LeverStandingPos", "-", groupNum, num);

	//モデルクラスのインスタンス化
	model_ = std::make_shared<Model>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Lever)].name), Material::Iron);

	//モデルの初期化
	model_->SetPos(info.pos);
	model_->SetScale(info.scale);
	model_->SetRot(info.rot);

	//ポジションの初期化
	pos_ = info.pos;

	//立つ位置の初期化
	standingPos_ = standingData.pos;

	model_->SetUseCollision(true, true);
}

//デストラクタ
Lever::~Lever()
{
}

//更新
void Lever::Update()
{
	//更新
	model_->Update();

	//アニメーションを停止アニメーションにする
	if (model_->IsAnimEnd())
	{
		OffAnimation();
	}
}

//描画
void Lever::Draw()
{
	//描画
	model_->Draw();
}

//衝突判定
bool Lever::CollCheck(const VECTOR& playerPos) const
{
	//プレイヤーとレバーモデルの衝突判定
	auto result = MV1CollCheck_Sphere(model_->GetModelHandle(), model_->GetColFrameIndex(), playerPos, radius_sphere);

	//衝突判定が1以上あったらtrueを返す
	if (result.HitNum > 0)
	{
		MV1CollResultPolyDimTerminate(result);
		return true;
	}

	return false;
}

//レバーの起動
void Lever::OnAnimation()
{
	model_->ChangeAnimation(static_cast<int>(AnimType::on), false, false, anim_change_time);
	isOn_ = true;
}

//レバーの停止
void Lever::OffAnimation()
{
	model_->ChangeAnimation(static_cast<int>(AnimType::off), false, false, anim_change_time);
	isOn_ = false;
}

//特殊な名前のオブジェクトの配置データを取得する
ObjectInfo Lever::GetSpecialNameObjectInfo(const std::string& name, const std::string& sign, int groupNum, int num)
{
	//短縮化
	auto& file = ExternalFile::GetInstance();

	//名前とグループ番号を結合した文字列を取得する
	std::string str = StrUtil::GetConcatenateNumAndStrings(name, ".", groupNum);

	//名前と記号と番号を結合した文字列を取得する
	str = StrUtil::GetConcatenateNumAndStrings(str, "-", num);

	//配置データの取得
	ObjectInfo objInfo = file.GetSpecifiedGimmickInfo(str);

	return objInfo;
}