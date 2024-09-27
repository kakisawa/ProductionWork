#pragma once
#include "GimmickBase.h"
#include <memory>

class ManualCrank;

class CrankScaffold final : public GimmickBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="materialType">マテリアルのタイプ</param>
	/// <param name="objInfo">配置データ</param>
	CrankScaffold(const int handle, const Material materialType, const ObjectInfo& objInfo);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CrankScaffold();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="player">プレイヤーの参照</param>
	void Update(Player& player)override;
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;;

	/// <summary>
	/// サウンドを鳴らす
	/// </summary>
	void PlayStopCrankSound();

	/// <summary>
	/// サウンドを鳴らすことが出来るかの条件分岐関数
	/// </summary>
	bool CanSound();

	/// <summary>
	/// モデルの移動
	/// </summary>
	void MoveModel();

	////////////////Getter////////////////

	/// <summary>
	/// 足場の上限と下限の距離のサイズを取得する
	/// </summary>
	/// <param name="num">番号</param>
	/// <returns>距離サイズ</returns>
	float GetUpperAndLowerDistanceSize(int num);

	/// <summary>
	/// 足場の上昇ベクトルを取得する
	/// </summary>
	/// <param name="num">番号</param>
	/// <returns>上昇ベクトル</returns>
	float GetUpVec(int num);
private:

	float oldRotZ_ = 0.0f;						//古いZの回転
	float upVecY_ = 0.0f;						//Y座標の上昇ベクトル

	VECTOR initPos_ = {};						//初期ポジション

	std::shared_ptr<ManualCrank> crank_;		//クランククラスのスマートポインタ

};

