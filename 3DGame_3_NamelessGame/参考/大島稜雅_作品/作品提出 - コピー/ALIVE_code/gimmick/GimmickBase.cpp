#include "GimmickBase.h"

#include "../object/Player.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/ExternalFile.h"

//作成済みのモデルをコピーするコンストラクタ
GimmickBase::GimmickBase(const int handle, const Material materialType, const ObjectInfo& objInfo): OrnamentBase(handle, materialType,objInfo)
{
}

//デストラクタ
GimmickBase::~GimmickBase()
{
}

//指定のオブジェクトの配置データを取得する
ObjectInfo GimmickBase::GetLoadObjectInfo(const std::string& name, int num)
{
	//短縮化
	auto& file = ExternalFile::GetInstance();

	//名前に番号を足した文字列を取得する
	std::string str = StrUtil::GetConcatenateNumAndStrings(name, ".", num);

	//上記で取得した文字列のオブジェクトの配置データを取得する	
	ObjectInfo objInfo = file.GetSpecifiedGimmickInfo(str);

	//配置データを返す
	return objInfo;
}