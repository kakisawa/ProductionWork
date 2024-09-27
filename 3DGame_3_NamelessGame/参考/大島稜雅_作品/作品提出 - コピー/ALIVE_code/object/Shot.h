#pragma once
#include <DxLib.h>
#include <memory>

class Model;
class Player;

class Shot
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="initPos">初期ポジション</param>
	/// <param name="moveVec">移動ベクトル</param>
	Shot(const int handle, const  VECTOR& initPos, const VECTOR& moveVec);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Shot();

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
	/// <param name="player">プレイヤーの参照</param>
	void HitCheck(Player& player);

	/// <summary>
	/// 存在しているかどうか取得する
	/// </summary>
	/// <returns>true：存在している　false：していない</returns>
	bool GetIsEnabled() const { return isEnable_; }

private:

	bool isEnable_ = false;						//存在しているか

	VECTOR pos_ = {};							//ポジション
	VECTOR moveVec_ = {};						//移動ベクトル
	VECTOR initialisePos_ = {};					//初期化時のポジション

	std::shared_ptr<Model> model_;				//モデルポインタのリスト

};

