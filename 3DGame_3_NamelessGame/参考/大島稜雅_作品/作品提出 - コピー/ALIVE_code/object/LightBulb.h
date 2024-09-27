#pragma once
#include "ObjectData.h"
#include <memory>

class Model;

class LightBulb
{
public: 
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="info">配置データ</param>
	LightBulb(const int handle, const ObjectInfo& info);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~LightBulb();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	////////////////Getter////////////////

	/// <summary>
	/// 回転ベクトルを取得する
	/// </summary>
	/// <returns>回転ベクトル</returns>
	VECTOR GetRotVec() const;

	/// <summary>
	/// 特定のフレームの座標を取得する
	/// </summary>
	/// <returns></returns>
	VECTOR GetFramePos() const;
private:

	float elapsedTime_ = 0.0f;			//経過時間
	float angle_ = 30.0f;				//角度
	float targetAngle_ = -30.0f;		//目標の角度

	VECTOR rot_ = {};					//回転ベクトル

	std::shared_ptr<Model> model_;		//モデルポインタ

};

