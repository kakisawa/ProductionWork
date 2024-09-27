#pragma once
#include "DxLib.h"
#include "ObjectBase.h"
#include <memory>

class CharacterBase : public ObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="materialType">マテリアルのタイプ</param>
	/// <param name="objInfo">配置データ</param>
	CharacterBase(const int handle, const Material materialType, const ObjectInfo objInfo);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CharacterBase() {};
};

