#pragma once
#include "GimmickBase.h"
#include <memory>

class Switch;

class TransparentObject final : public GimmickBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="materialType">マテリアルのタイプ</param>
	/// <param name="objInfo">配置データ</param>
	TransparentObject(const int handle, const Material materialType, const ObjectInfo& objInfo);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~TransparentObject();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="player">プレイヤーの参照</param>
	void Update(Player& player)override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// スイッチモデルと死体の衝突判定を行う
	/// </summary>
	/// <param name="deadPerson">死体のポインタ</param>
	void UpdateForCorpse(const std::shared_ptr<ObjectBase>& corpse)override;

	/// <summary>
	/// 衝突判定を行うモデルの追加
	/// </summary>
	/// <returns>モデルポインタ</returns>
	virtual std::shared_ptr<Model> AddCollModel() const override;

private:

	int materialNum_ = 0;								//このクラスでインスタンス化される3Dモデルのマテリアルの数を保管する

	float alphaValue_ = 0.0f;							//透過値を保管する

	std::shared_ptr<Switch> switch_;					//スイッチのポインタ

};

