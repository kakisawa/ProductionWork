#pragma once
#include "../object/OrnamentBase.h"

class GimmickBase : public OrnamentBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="materialType">マテリアルのタイプ</param>
	/// <param name="objInfo">配置データ</param>
	GimmickBase(const int handle, const Material materialType, const ObjectInfo& objInfo);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~GimmickBase();

	/// <summary>
	/// 指定のオブジェクトの配置データを取得する
	/// </summary>
	/// <param name="name">取得したいオブジェクトの名前</param>
	/// <param name="num">番号</param>
	/// <returns>配置データ</returns>
	ObjectInfo GetLoadObjectInfo(const std::string& name, int num);
};

