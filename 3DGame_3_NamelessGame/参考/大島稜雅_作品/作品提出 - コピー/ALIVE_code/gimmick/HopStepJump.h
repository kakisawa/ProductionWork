#pragma once
#include "GimmickBase.h"

class HopStepJump final : public GimmickBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="materialType">マテリアルのタイプ</param>
	/// <param name="objInfo">配置データ</param>
	HopStepJump(const int handle, const Material materialType, const ObjectInfo& objInfo);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~HopStepJump();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="player">プレイヤーの参照</param>
	void Update(Player& player)override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// 現在の番号を変える
	/// </summary>
	void ChangeCurrentNum();

	/// <summary>
	/// アルファ値を変更する
	/// </summary>
	void ChangeAlphaValue();

	/// <summary>
	/// マテリアルの色を変える
	/// </summary>
	void ChangeMaterialColor();

private:

	int currentNum_ = 0;			//現在の番号
	int materialNum_ = 0;			//マテリアルの数

	float elapseddTime_ = 0.0f;		//経過時間
	float alphaValue_ = 0.0f;		//透過値

};

