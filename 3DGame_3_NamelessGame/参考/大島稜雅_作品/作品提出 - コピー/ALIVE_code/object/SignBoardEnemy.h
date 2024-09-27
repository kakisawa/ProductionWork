#pragma once
#include "EnemyBase.h"
#include <DxLib.h>

class SignBoardEnemy final : public EnemyBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="materialType">マテリアルのタイプ</param>
	/// <param name="objInfo">配置データ</param>
	SignBoardEnemy(const int handle, const Material materialType, const ObjectInfo& objInfo);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SignBoardEnemy();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="player">プレイヤーの参照</param>
	void Update(Player& player)override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 数字の変換行列を行い、フレーム番号を返す
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="displayFrameName">表示したい場所のフレーム名</param>
	/// <param name="number">数字の文字列</param>
	/// <returns>フレーム番号</returns>
	int NumericTransformationMatrix(int handle,const std::string& displayFrameName, const std::string& number);
private:
	int numModel_[2] = {};							//数のモデル
	int deathCount_ = 0;							//死んだ回数

	VECTOR numScale_ = { 80.0f,80.0f, 80.0f };		//数の拡縮率
	VECTOR numRot_	 = { 0.0f,0.0f, -15.0f };		//数の回転率
};

