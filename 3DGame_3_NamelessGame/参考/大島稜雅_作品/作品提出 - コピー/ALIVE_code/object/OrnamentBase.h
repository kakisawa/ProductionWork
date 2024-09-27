#pragma once
#include "ObjectBase.h"

class OrnamentBase : public ObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="materialType">マテリアルのタイプ</param>
	/// <param name="objInfo">配置データ</param>
	OrnamentBase(const int handle, const Material materialType, const ObjectInfo objInfo);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~OrnamentBase();
};

