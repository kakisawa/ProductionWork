#include "OrnamentBase.h"

#include "../util/Model.h"

//コンストラクタ
OrnamentBase::OrnamentBase(const int handle, const Material materialType, const ObjectInfo objInfo) : ObjectBase(handle, materialType,objInfo)
{
	//衝突判定を行う
	isCollCheck_ = true;

	//衝突判定を行うモデルフレームの更新が必要かの設定を行う
	isUpdateColl_ = false;

	//衝突判定用フレームの設定
	model_->SetUseCollision(isCollCheck_, isUpdateColl_);
}

//デストラクタ
OrnamentBase::~OrnamentBase()
{
}