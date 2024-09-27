#include "CrankScaffold.h"
#include "ManualCrank.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"

#include "../object/Player.h"

namespace
{
	//Z軸の回転の最低値
	constexpr int min_rot_Z = 0;

	//サウンドが聞こえる範囲
	constexpr float sound_range = 1500.0f;
}

//コンストラクタ
CrankScaffold::CrankScaffold(const int handle, const Material materialType, const ObjectInfo& objInfo) : GimmickBase(handle, materialType, objInfo)
{
	//短縮化
	auto& file = ExternalFile::GetInstance();

	//objInfoの名前から末尾の数値を取得する
	int num = StrUtil::GetNumberFromString(objInfo.name, ".");

	//クランクの配置データを取得する
	ObjectInfo crankData = GetLoadObjectInfo("Crank", num);

	//インスタンス化
	crank_ = std::make_shared<ManualCrank>(crankData);

	//上昇ベクトルの初期化
	upVecY_ = GetUpVec(num);

	//衝突判定用フレームの設定
	model_->SetUseCollision(true, false);

	//初期ポジションの初期化
	initPos_ = objInfo.pos;
}

//デストラクタ
CrankScaffold::~CrankScaffold()
{
}

//更新
void CrankScaffold::Update(Player& player)
{
	//クランクとプレイヤーの衝突判定を行い
	//当たっていた場合プレイヤーに当たっていたクラスのポインターを
	//セットする
	if (crank_->HitCollPlayer(player))
	{
		player.SetCrankPointer(crank_);
	}

	//クランクの更新
	crank_->Update();

	//音を鳴らす
	PlayStopCrankSound();

	//モデルの移動
	MoveModel();

	//Z回転の値を残しておく
	oldRotZ_ = crank_->GetRotZ();
}

//描画
void CrankScaffold::Draw()
{
	//モデルの描画
	model_->Draw();

	//クランクの描画
	crank_->Draw();
}

//サウンドを鳴らす
void CrankScaffold::PlayStopCrankSound()
{
	//短縮化
	auto& sound = SoundManager::GetInstance();

	//サウンドを鳴らす条件が満たせていなかった場合return
	if (!CanSound()) 
	{
		return;
	}

	//停止サウンドを鳴らす
	sound.Set3DSoundInfo(pos_, sound_range, "stopCrank");
	sound.PlaySE("stopCrank");
}

//サウンドを鳴らすことが出来るかの条件分岐関数
bool CrankScaffold::CanSound()
{
	//短縮化
	auto& sound = SoundManager::GetInstance();

	//現在のZの回転を取得する
	float nowRotZ = crank_->GetRotZ();

	//以降の条件を満たしていなかったらサウンドを鳴らすことは出来ない
	if (oldRotZ_ == nowRotZ) return false;										//現在の回転値と現在の回転値が同じならfalse
	if (sound.CheckSoundFile("stopCrank")) return false;						//stopCrankという音がなっていなかったのならfalse
	if (min_rot_Z != nowRotZ && crank_->GetMaxRotZ() != nowRotZ) return false;	//現在の回転値が最小回転値または最大回転値でもないのならfalse

	//音を鳴らす条件が揃っている
	return true;
}

//モデルの移動
void CrankScaffold::MoveModel()
{
	//移動
	pos_.y = crank_->GetRotZ() * upVecY_ + initPos_.y;

	//ポジションのセット
	model_->SetPos(pos_);
}

//足場の上限と下限の距離のサイズを取得する
float CrankScaffold::GetUpperAndLowerDistanceSize(int num)
{
	//足場の上限の配置データを取得する
	ObjectInfo upperLimitData = GetLoadObjectInfo("CrankUpperLimit", num);

	//足場の下限の配置データを取得する
	ObjectInfo lowerLimitData = GetLoadObjectInfo("CrankLowerLimit", num);

	//上限と下限の距離を取得する
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(upperLimitData.pos, lowerLimitData.pos);

	//距離のサイズを返す
	return distanceSize;
}

//足場の上昇ベクトルを取得する
float CrankScaffold::GetUpVec(int num)
{
	//上限と下限の距離を取得する
	float distanceSize = GetUpperAndLowerDistanceSize(num);

	//上昇ベクトルの初期化
	float up = distanceSize / crank_->GetMaxRotZ();

	return up;
}
