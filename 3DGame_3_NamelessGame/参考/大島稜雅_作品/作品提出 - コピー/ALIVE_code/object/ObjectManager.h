#pragma once
#include "ObjectData.h"
#include <list>
#include <string>
#include <memory>
#include <DxLib.h>
#include <unordered_map>

class ObjectBase;
class Model;
class Player;
class ShotManager;

class ObjectManager
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ObjectManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ObjectManager();

	/// <summary>
	/// メインステージのオブジェクト生成
	/// </summary>
	void MainStageObjectGenerator();

	/// <summary>
	/// オープニングシーンのオブジェクトを生成する
	/// </summary>
	void OpeningStageObjectGenerator();

	/// <summary>
	/// エンディングシーンのオブジェクトを生成する
	/// </summary>
	void EndStageObjectGenerator();

	/// <summary>
	/// 死体生成
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="objInfo">オブジェクトの配置データ等</param>
	void CorpseGenerator(const int handle, const ObjectInfo& objInfo);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="player">プレイヤーの参照</param>
	/// <param name="shotManager">ショットマネージャーのスマートポインタ</param>
	void Update(Player& player,const std::shared_ptr<ShotManager>& shotManager);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定を行うモデルを追加する
	/// </summary>
	void AddCheckCollModel();

	/// <summary>
	/// 円周上のポジションを取得する
	/// </summary>
	/// <param name="info"></param>
	/// <param name="loadObjPos"></param>
	void CircumferencePosition(const float angle,VECTOR& infoPos, const VECTOR& playerPos);

	/// <summary>
	/// 敵生成
	/// </summary>
	/// <param name="deathCount">今までにプレイヤーが死亡した数</param>
	/// <param name="info">配置データ</param>
	void EnemyGenerator(const int deathCount, const ObjectInfo& info);

	////////////////Getter////////////////

	/// <summary>
	/// 衝突判定に使用するモデルを取得する
	/// </summary>
	/// <returns>衝突判定を行うモデルスマートポインタのリスト</returns>
	const std::list<std::shared_ptr<Model>>& GetAllCheckCollModel();

	/// <summary>
	/// 特定のモデルポインタを取得する
	/// </summary>
	/// <param name="type">取得したいオブジェクトタイプ</param>
	/// <returns>特定のモデルポインタ</returns>
	std::shared_ptr<Model> GetSpecificModel(const ObjectType type);

	/// <summary>
	/// 特定のオブジェクトベースポインタを取得する
	/// </summary>
	/// <param name="type">取得したオブジェクトタイプ</param>
	/// <returns>特定のオブジェクトベースポインタ</returns>
	std::list<std::shared_ptr<ObjectBase>> GetSpecificObject(const ObjectType type);

private:
	/// <summary>
	/// 死んだ回数分生成する
	/// </summary>
	/// <param name="deathCount">今までにプレイヤーが死亡した数</param>
	/// <param name="info">配置データ</param>
	void GeneratedForTheNumberOfTimesYouDie(const int deathCount,ObjectInfo info);

	/// <summary>
	/// 既定の回数分生成する
	/// </summary>
	/// <param name="deathCount">今までにプレイヤーが死亡した数</param>
	/// <param name="str">文字列</param>
	/// <param name="info">配置データ</param>
	void GeneratePredeterminedNumberOfTimes(const int deathCount, const std::string& str, const ObjectInfo& info);

	/// <summary>
	/// 死んだ回数によって補助足場を生成する
	/// </summary>
	/// <param name="deathCount">死んだ回数</param>
	/// <param name="info">配置データ</param>
	void GenerateCorpseMountain(const int deathCount, const ObjectInfo& info);

	/// <summary>
	/// 置物生成機
	/// </summary>
	/// <param name="name">オブジェクトの名前</param>
	/// <param name="objType">オブジェクトのタイプ</param>
	/// <param name="materialType">オブジェクトのタイプ</param>
	/// <param name="objInfo">配置データ</param>
	void OrnamentGenerator(const std::string& name, const ObjectType objType, const Material materialType, const ObjectInfo& objInfo);

	/// <summary>
	/// ギミック生成機
	/// </summary>
	/// <param name="name">オブジェクトの名前</param>
	/// <param name="objType">オブジェクトのタイプ</param>
	/// <param name="materialType">オブジェクトのタイプ</param>
	/// <param name="objInfo">配置データ</param>
	void GimmickObjectGenerator(const std::string& name, const ObjectType objType, const Material materialType, const ObjectInfo& objInfo);
private:

	std::list<std::shared_ptr<Model>> checkCollList_;											//衝突判定を行うモデルのリスト
	std::unordered_map<std::string, bool>  usedEnemyList_;										//使用済みエネミーのリスト
	std::unordered_map<std::string, bool>  usedCorpseMtList_;									//使用済みの死体のリスト

	std::unordered_map<ObjectType, std::list<std::shared_ptr<ObjectBase>>> objects_;			//オブジェクトをobjectTypeをキーにlistでもつ
};

