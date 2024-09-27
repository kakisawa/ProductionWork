#pragma once
#include "../object/ObjectData.h"
#include <memory>
#include <DxLib.h>

class Model;

class Lever
{
private:
	//アニメーションタイプ
	enum class AnimType {
		on,
		off,
		max,
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="info">配置データ</param>
	Lever(const ObjectInfo& info);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Lever();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定
	/// </summary>
	/// <param name="playerPos">プレイヤーのポジション</param>
	/// <returns>true : 当たっている  false : 当たっていない</returns>
	bool CollCheck(const VECTOR& playerPos) const;

	/// <summary>
	/// レバーの起動
	/// </summary>
	void OnAnimation();

	/// <summary>
	/// レバーの停止
	/// </summary>
	void OffAnimation();

	/// <summary>
	/// レバーが起動しているかどうか取得
	/// </summary>
	/// <returns>レバーが起動しているか</returns>
	bool IsOn() const { return isOn_; }


	////////////////Getter////////////////

	/// <summary>
	/// 特殊な名前のオブジェクトの配置データを取得する
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="sign">記号</param>
	/// <param name="groupNum">グループ番号</param>
	/// <param name="num">番号</param>
	/// <returns>配置データ</returns>
	ObjectInfo GetSpecialNameObjectInfo(const std::string& name, const std::string& sign, int groupNum, int num);

	/// <summary>
	/// モデルポインタの取得
	/// </summary>
	/// <returns>モデルのポインタ</returns>
	const std::shared_ptr<Model>& GetModelPointer() const { return model_; }

	/// <summary>
	/// レバーを引くポジションの取得
	/// </summary>
	/// <returns>レバーを引くポジション</returns>
	const VECTOR& GetStandingPosition() const { return standingPos_; }


	////////////////Setter////////////////

	/// <summary>
	/// レバーを引くポジションを設定する
	/// </summary>
	/// <param name="standingPos">レバーを引く位置</param>
	void SetStandingPos(const VECTOR& standingPos) { standingPos_ = standingPos; }

private:

	bool isOn_ = false;							//レバーが起動しているかどうか

	VECTOR pos_ = {};							//ポジション
	VECTOR standingPos_ = {};					//エレベータの停止ポジション

	std::shared_ptr<Model> model_;				//モデルクラスのスマートポインタ

};

