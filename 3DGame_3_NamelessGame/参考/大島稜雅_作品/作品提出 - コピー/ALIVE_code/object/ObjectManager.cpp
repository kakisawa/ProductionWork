#include "ObjectManager.h"

#include "Player.h"
#include "Corpse.h"
#include "EnemyBase.h"
#include "ThrowEnemy.h"
#include "ObjectBase.h"
#include "StreetLight.h"
#include "OrnamentBase.h"
#include "SignBoardEnemy.h"

#include "../gimmick/Elevator.h"
#include "../gimmick/HopStepJump.h"
#include "../gimmick/CrankScaffold.h"
#include "../gimmick/TransparentObject.h"
#include "../gimmick/PenetrationScaffld.h"

#include "../util/game.h"
#include "../util/Util.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/ModelManager.h"

namespace
{
	//円の半径
	constexpr float distance_from_center = 200.0f;

	//敵を円周上に出現させる際の一体当たりの角度
	constexpr float center_angle = 10.0f;

	//オブジェクトの更新を行わない範囲
	constexpr float not_update_range = 4000.0f;

	//死体を生成する最大数
	constexpr int max_corpse_num = 10;
}

//コンストラクタ
ObjectManager::ObjectManager()
{
}

//デストラクタ
ObjectManager::~ObjectManager()
{
}

//メインステージのオブジェクト生成
void ObjectManager::MainStageObjectGenerator()
{
	//短縮化
	auto& file = ExternalFile::GetInstance();

	//置物の生成
	for (const auto& objTable : file.GetLoadMainStageObjectInfo())
	{
		for (const auto& objInfo : objTable.second)
		{
			for (const auto& data : objData_)
			{
				if (objInfo.name == data.name)
				{
					OrnamentGenerator(data.name, data.type, data.material, objInfo);
				}
			}
		}
	}

	//ギミックの生成
	for (const auto& objTable : file.GetGimmickInfo())
	{
		for (const auto& objInfo : objTable.second)
		{
			for (const auto& data : objData_)
			{
				if (objTable.first == data.name)
				{
					GimmickObjectGenerator(data.name, data.type, data.material, objInfo);
				}
			}
		}
	}
}

//オープニングシーンのオブジェクトを生成する
void ObjectManager::OpeningStageObjectGenerator()
{
	//短縮化
	auto& file = ExternalFile::GetInstance();

	//置物の生成
	for (const auto& objTable : file.GetLoadOpeningStageObjectInfo())
	{
		for (const auto& objInfo : objTable.second)
		{
			for (const auto& data : objData_)
			{
				if (objInfo.name == data.name)
				{
					OrnamentGenerator(data.name, data.type, data.material, objInfo);
				}
			}
		}
	}
}

//エンディングシーンのオブジェクトを生成する
void ObjectManager::EndStageObjectGenerator()
{
	//短縮化
	auto& file = ExternalFile::GetInstance();

	//置物の生成
	for (const auto& objTable : file.GetLoadEndingStageObjectInfo())
	{
		for (const auto& objInfo : objTable.second)
		{
			for (const auto& data : objData_)
			{
				if (objInfo.name == data.name)
				{
					OrnamentGenerator(data.name, data.type, data.material, objInfo);
				}
			}
		}
	}
}

//死体生成
void ObjectManager::CorpseGenerator(const int handle, const ObjectInfo& objInfo)
{
	//死体を一つ生成する
	objects_[ObjectType::Corpse].push_back(std::make_shared<Corpse>(handle, Material::Other, objInfo));

	//死体が10個未満だったらリターン
	if (objects_[ObjectType::Corpse].size() < max_corpse_num) return;
	
	//死体のリストの一番先頭(古い)死体を削除する
	objects_[ObjectType::Corpse].remove(objects_[ObjectType::Corpse].front());
}

//更新
void ObjectManager::Update(Player& player,const std::shared_ptr<ShotManager>& shotManager)
{
	//objects_の各要素のisEnableを取得し、無効になっていれば該当コンテナの削除
	for (auto& list : objects_)
	{
		list.second.remove_if([](std::shared_ptr<ObjectBase> obj) {return !obj->GetIsEnabled(); });
	}

	//短縮化
	auto& model = ModelManager::GetInstance();

	float distanceSize = 0.0f;
	VECTOR playerPos = player.GetStatus().pos;

	//死体を生成する
	if (player.GetIsCorpseGeneratable())
	{
		CorpseGenerator(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Corpse)].name), player.GetCorpseInfo());
		player.SetIsCorpseGeneratable(false);
	}

	//死体とその他のオブジェクトの衝突判定を行う
	for (const auto& list : objects_)
	{
		for (const auto& obj : list.second)
		{
			if (list.first == ObjectType::Corpse) 
			{
				continue;
			}
			for (const auto& deadperson : objects_[ObjectType::Corpse])
			{
				distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(obj->GetPos(), playerPos);
				if (distanceSize < not_update_range)
				{
					obj->UpdateForCorpse(deadperson);
				}
			}
		}
	}

	//enemyのShot
	for (const auto& obj : objects_[ObjectType::Enemy])
	{
		if (std::dynamic_pointer_cast<ThrowEnemy>(obj) != nullptr)
		{
			std::dynamic_pointer_cast<ThrowEnemy>(obj)->Shot(shotManager, playerPos, player.GetStatus().height);
		}
	}

	//更新
	for (const auto& list : objects_)
	{
		for (const auto& obj : list.second)
		{
			distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(obj->GetPos(), playerPos);
			if (distanceSize < not_update_range)
			{
				obj->Update(player);
			}
		}
	}

	//敵の生成
	for (const auto& enemy : ExternalFile::GetInstance().GetEnemyInfo(playerPos))
	{
		if (!usedEnemyList_[enemy.name])
		{
			EnemyGenerator(player.GetDeathCount(), enemy);
		}
	}

	//死体の山の生成
	for (const auto& table : ExternalFile::GetInstance().GetGimmickInfo())
	{
		if (table.first != "CorpseMountain")
		{
			continue;
		}

		for (const auto& corpseMt : table.second)
		{
			if (!usedCorpseMtList_[corpseMt.name]) 
			{
				GenerateCorpseMountain(player.GetDeathCount(), corpseMt);
			}
		}
	}
}

//描画
void ObjectManager::Draw()
{
	for (const auto& objs : objects_)
	{
		for (const auto& obj : objs.second)
		{
			obj->Draw();
		}
	}
}

//衝突判定に使用するモデルを取得する
const std::list<std::shared_ptr<Model>>& ObjectManager::GetAllCheckCollModel()
{
	for (const auto& obj : objects_)
	{
		for (const auto& model : obj.second)
		{
			if (model->GetIsCollCheck())
			{
				checkCollList_.push_back(model->GetModelPointer());
			}
		}
	}

	return checkCollList_;
}

//特定のモデルポインタを取得する
std::shared_ptr<Model> ObjectManager::GetSpecificModel(const ObjectType type)
{
	std::shared_ptr<Model> specificPoint;

	//引数で指定されたオブジェクトのモデルポインタを上記で宣言した
	//配列に代入する
	for (const auto& obj : objects_)
	{
		//引数と違うタイプだった場合continue
		if (obj.first != type) 
		{
			continue;
		}
		//指定されたタイプだった場合
		//そのオブジェクトのリストからモデルポインタを取得し
		//specificListに代入する
		for (auto& objSecond : obj.second) {
			specificPoint = objSecond->GetModelPointer();
		}
	}

	//リストを返す
	return specificPoint;
}

//特定のオブジェクトベースポインタを取得する
std::list<std::shared_ptr<ObjectBase>> ObjectManager::GetSpecificObject(const ObjectType type)
{
	std::list<std::shared_ptr<ObjectBase>> obj = {};

	if (objects_.count(type) > 0)
	{
		obj = objects_[type];
	}

	return obj;
}

//衝突判定を行うモデルを追加する
void ObjectManager::AddCheckCollModel()
{
	checkCollList_.clear();

	for (const auto& obj : objects_)
	{
		for (const auto& objSecond : obj.second)
		{
			if (objSecond->AddCollModel() != nullptr) 
			{
				checkCollList_.push_back(objSecond->AddCollModel());
			}
		}
	}
}

//円周上のポジションを取得する
void ObjectManager::CircumferencePosition(const float angle,VECTOR& infoPos, const VECTOR& playerPos)
{
	VECTOR pos = {};
	float radian = MathUtil::DegreeToRadian(angle);

	pos.x = sin(radian);
	pos.z = cos(radian);
	pos.y = 0.0f;

	pos = VScale(pos, distance_from_center);

	pos.y = infoPos.y;

	pos = VAdd(playerPos, pos);

	pos.y = infoPos.y;

	infoPos = pos;
}

//敵生成
void ObjectManager::EnemyGenerator(const int deathCount, const ObjectInfo& info)
{
	//短縮化
	auto& model = ModelManager::GetInstance();

	//文字列のサイズを取得する
	int size = static_cast<int>(info.name.size());

	//「.」以降の文字列によって
	//エネミーの召喚パターンを変更する
	if (size > 0) {
		std::string numStr = StrUtil::GetStringAfterSign(info.name, ".");
		numStr = StrUtil::GetStringBeforeSign(numStr, "-");

		std::string str = StrUtil::GetStringBeforeSign(info.name, ".");

		//文字列が「ALL」だったら
		if (numStr == "ALL") 
		{
			GeneratedForTheNumberOfTimesYouDie(deathCount, info);
		}
		else if (str == "SignBoardEnemy") 
		{
			objects_[ObjectType::Enemy].push_back(std::make_shared<SignBoardEnemy>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::SignBoardPlayer)].name), Material::Other, info));
			usedEnemyList_[info.name] = true;
		}
		else 
		{
			GeneratePredeterminedNumberOfTimes(deathCount, numStr, info);
		}
	}
}

//死んだ回数分生成する
void ObjectManager::GeneratedForTheNumberOfTimesYouDie(const int deathCount,ObjectInfo info)
{
	//短縮化
	auto& model = ModelManager::GetInstance();

	float angle = 0.0f;
	for (int i = 0; i < deathCount; i++) 
	{
		//プレイヤーを中心に円周上でスポーンさせる
		CircumferencePosition(angle, info.pos, info.pos);

		//インスタンス化
		objects_[ObjectType::Enemy].push_back(std::make_shared<ThrowEnemy>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Player)].name), Material::Other, info));
		angle -= center_angle;

		usedEnemyList_[info.name] = true;
	}
}

//既定の回数分生成する
void ObjectManager::GeneratePredeterminedNumberOfTimes(const int deathCount, const std::string& str, const ObjectInfo& info)
{
	//短縮化
	auto& model = ModelManager::GetInstance();

	//最後の文字列をint型の数値に変換する
	int num = atoi(str.c_str());

	//文字列の最後の数よりもdeathCountが多ければ
	//エネミーを召喚する
	if (num <= deathCount) 
	{
		objects_[ObjectType::Enemy].push_back(std::make_shared<ThrowEnemy>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Player)].name), Material::Other, info));
		usedEnemyList_[info.name] = true;
	}
}

//死んだ回数によって補助足場を生成する
void ObjectManager::GenerateCorpseMountain(const int deathCount, const ObjectInfo& info)
{
	//短縮化
	auto& model = ModelManager::GetInstance();

	std::string str = StrUtil::GetStringAfterSign(info.name, ".");
	str = StrUtil::GetStringBeforeSign(str, "-");

	//最後の文字列をint型の数値に変換する
	int num = atoi(str.c_str());

	//文字列の最後の数よりもdeathCountが多ければ
	//エネミーを召喚する
	if (num <= deathCount) 
	{
		objects_[ObjectType::CorpseMountain].push_back(std::make_shared<OrnamentBase>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::CorpseMountain)].name), Material::Other, info));
		usedCorpseMtList_[info.name] = true;
	}
}

//置物生成機
void ObjectManager::OrnamentGenerator(const std::string& name, const ObjectType objType, const Material materialType, const ObjectInfo& objInfo)
{
	//短縮化
	auto& model = ModelManager::GetInstance();

	//ライトのインスタンス化
	if (objType == ObjectType::StreetLight)
	{
		objects_[objType].push_front(std::make_shared<StreetLight>(model.GetModelHandle(name), materialType, objInfo));
		return;
	}

	objects_[objType].push_front(std::make_shared<OrnamentBase>(model.GetModelHandle(name), materialType, objInfo));
}

//ギミック生成機
void ObjectManager::GimmickObjectGenerator(const std::string& name, const ObjectType objType, const  Material materialType, const ObjectInfo& objInfo)
{
	//短縮化
	auto& model = ModelManager::GetInstance();

	switch (objType) 
	{
	case ObjectType::TransScaffold:
		objects_[objType].push_front(std::make_shared<TransparentObject>(model.GetModelHandle(name),materialType, objInfo));
		break;
	case ObjectType::Elevator:
		objects_[objType].push_front(std::make_shared<Elevator>(model.GetModelHandle(name),materialType, objInfo));
		break;
	case ObjectType::CrankScaffold:
		objects_[objType].push_front(std::make_shared<CrankScaffold>(model.GetModelHandle(name),materialType, objInfo));
		break;
	case ObjectType::PenetrationScaffld:
		objects_[objType].push_front(std::make_shared<PenetrationScaffld>(model.GetModelHandle("Train"), materialType, objInfo));
		break;
	case ObjectType::HopStepJump:
		objects_[objType].push_front(std::make_shared<HopStepJump>(model.GetModelHandle(name), materialType, objInfo));
		break;
	}
}
