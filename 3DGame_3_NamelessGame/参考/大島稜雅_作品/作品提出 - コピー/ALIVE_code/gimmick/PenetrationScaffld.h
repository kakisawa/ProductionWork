#pragma once
#include "GimmickBase.h"

class PenetrationScaffld final : public GimmickBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="materialType">マテリアルのタイプ</param>
	/// <param name="objInfo">配置データ</param>
	PenetrationScaffld(const int handle,const Material materialType,const ObjectInfo& objInfo);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PenetrationScaffld();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// マテリアルの色を変更する
	/// </summary>
	void ChangeMaterialColor();
private:

	int materialNum_ = 0;				//マテリアルの数

	float alphaValue_ = 0.3f;			//アルファ値

	bool alphaValueIncrease_ = false;	//アルファ値の増加
};

