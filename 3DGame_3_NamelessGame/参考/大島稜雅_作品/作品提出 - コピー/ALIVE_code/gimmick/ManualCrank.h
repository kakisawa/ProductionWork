#pragma once
#include "../object/ObjectData.h"
#include <DxLib.h>
#include <memory>

class Model;
class InputState;
class Player;

class ManualCrank
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="info">配置データ</param>
	ManualCrank(const ObjectInfo& objInfo);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ManualCrank();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// クランクの音を再生
	/// </summary>
	void PlayCrankSound();

	/// <summary>
	/// サウンドを再生することが出来るか
	/// </summary>
	/// <returns></returns>
	bool CanPlaySound() const;

	/// <summary>
	/// プレイヤーとモデルの当たり判定を行う
	/// </summary>
	/// <param name="player">プレイヤーの参照</param>
	/// <returns>プレイヤーと衝突したか</returns>
	bool HitCollPlayer(Player& player) const;

	////////////////Getter////////////////

	/// <summary>
	/// Z軸の最大回転率を取得する
	/// </summary>
	/// <returns>Z軸の最大回転率</returns>
	float GetMaxRotZ() const;

	/// <summary>
	/// 指定のオブジェクトの配置データを取得する
	/// </summary>
	/// <param name="name">取得したいオブジェクトの名前</param>
	/// <param name="num">番号</param>
	/// <returns>配置データ</returns>
	ObjectInfo GetLoadObjectInfo(const std::string& name, int num);

	/// <summary>
	/// 現在のZ軸の回転率を取得する
	/// </summary>
	/// <returns>Z軸の回転率</returns>
	float GetRotZ() const { return rotZ_; }

	/// <summary>
	/// モデルポインタを取得
	/// </summary>
	/// <returns>モデルのポインタ</returns>
	const std::shared_ptr<Model>& GetModelPointer() const { return model_; }

	/// <summary>
	/// クランクを回す際の立ち位置の取得
	/// </summary>
	/// <returns>ポジション</returns>
	const VECTOR& GetStandingPosition() const { return standingPos_; }


	////////////////Setter////////////////

	/// <summary>
	/// 回転率を設定する
	/// </summary>
	/// <param name="rotZ">Z軸の回転率</param>
	void SetRotZ(float rotZ) { rotZ_ = rotZ; }

private:

	float rotZ_ = 0.0f;					//Z軸の回転量
	float radian_ = 0.0f;				//ラジアン
	float oldRotZ_ = 0.0f;				//前フレームのZ軸の回転量

	VECTOR pos_ = {};					//ポジション
	VECTOR initPos_ = {};				//初期ポジション
	VECTOR standingPos_ = {};			//クランクを回す立ち位置

	std::shared_ptr<Model> model_;		//モデルクラスのスマートポインタ

};

