#pragma once
#include "../object/ObjectData.h"
#include <list>
#include <memory>
#include <DxLib.h>

class Player;
class ObjectBase;
class Model;

class Switch
{
private:
	//スイッチのアニメーション
	enum class Anim
	{
		On,
		Off,
	};
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="objInfo">配置データ</param>
	Switch(const ObjectInfo& objInfo);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Switch();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="player">プレイヤーの参照</param>
	void Update(Player& player);
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// スイッチモデルと死体の衝突判定を行う
	/// </summary>
	/// <param name="deadPerson">死体のポインタ</param>
	void HitColl(const std::shared_ptr<ObjectBase>& corpse);

	/// <summary>
	/// エレベーターの衝突判定の結果
	/// </summary>
	/// <returns>true：衝突　false：衝突していない</returns>
	bool ElevatorCollResult();

	/// <summary>
	/// 透過オブジェクトの衝突判定の結果
	/// </summary>
	/// <returns>true：衝突　false：衝突していない</returns>
	bool TransCollResult();

	/// <summary>
	/// 衝突結果の削除
	/// </summary>
	void DeleteHitResult();

	/// <summary>
	/// 起動中かを変更する
	/// </summary>
	/// <param name="time">時間</param>
	void ChangeDuringStartup(const float time);

	/// <summary>
	/// サウンドを鳴らす
	/// </summary>
	void PlayOnSound();

	////////////////Getter////////////////

	/// <summary>
	/// 起動中か
	/// </summary>
	/// <returns>true : 起動中 false : 起動していない</returns>
	bool GetIsDuringStartup()  const { return isDuringStartup_; }

	/// <summary>
	/// 衝突判定を行うモデルを追加する
	/// </summary>
	/// <returns>衝突判定を行うモデルのポインタ</returns>
	const std::shared_ptr<Model>& GetModelPointer() const;

private:
	/// <summary>
	/// スイッチモデルとプレイヤーの衝突判定を行う
	/// </summary>
	/// <param name="player">プレイヤーの参照</param>
	void HitCollPlayer(Player& player);

	/// <summary>
	/// スイッチをオンにする
	/// </summary>
	void OnAnim();

	/// <summary>
	/// スイッチをオフにする
	/// </summary>
	void OffAnim();
private:

	bool isDuringStartup_ = false;									//スイッチが押されたか　true：on　false：off

	VECTOR pos_ = {};									//ポジション

	std::shared_ptr<Model> model_;						//モデルクラスのスマートポインタ
	std::list<MV1_COLL_RESULT_POLY_DIM> hitDim_;		//衝突判定の配列

	void(Switch::* stateFunc_)();
};

