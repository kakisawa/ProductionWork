#pragma once
#include "../object/ObjectData.h"
#include <string>
#include <DxLib.h>

class Model
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="fileName">モデルが入っているファイルパス</param>
	Model(const std::string& fileName,const Material materialType);

	/// <summary>
	/// duplicate用コンストラクタ
	/// </summary>
	/// <param name="orgModel"></param>
	Model(const int orgModel,const Material materialType);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Model();

	/// <summary>
	/// collisionフレームをcollisionとして使う
	/// </summary>
	/// <param name="isUse">使用するか</param>
	/// <param name="isNeedUpdate">更新が必要か</param>
	/// <param name="collFrameName">特定のフレームを衝突判定用フレームにする場合、フレーム名をいれる</param>
	void SetUseCollision(const bool isUse, const  bool isNeedUpdate, const std::string& collFrameName = "");

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// アニメーションを変更する
	/// </summary>
	/// <param name="animNo">アニメーション番号</param>
	/// <param name="isLoop">ループ再生するか</param>
	/// <param name="isForceChange">強制的に変更するか</param>
	/// <param name="changeFrame">アニメーションを変更する際に何秒かけて変化するのか</param>
	void ChangeAnimation(const int animNo, const  bool isLoop, const  bool isForceChange, const int changeFrame);

	/// <summary>
	/// アニメーションの終わりを取得する
	/// </summary>
	/// <returns>アニメーションが終わったか</returns>
	bool IsAnimEnd();

	////////////////Getter////////////////

	/// <summary>
	/// 指定したフレームの座標を取得する
	/// </summary>
	/// <param name="animNo">アニメーション番号</param>
	/// <param name="frameName">取得したいアニメーション名</param>
	/// <returns>座標</returns>
	VECTOR GetFrameLocalPosition(const std::string& frameName)const;

	/// <summary>
	/// アニメーションの特定フレームと現在のアニメーションフレーム数が同じかを取得する
	/// </summary>
	/// <param name="specifiedTime">特定のフレーム時間</param>
	/// <returns>true : 同じ  false : 違う</returns>
	bool GetSpecifiedAnimTime(const int specifiedTime)const;

	/// <summary>
	/// モデルを取得する
	/// </summary>
	/// <returns>モデルハンドルを返す</returns>
	int GetModelHandle() const { return modelHandle_; }

	/// <summary>
	/// collisionフレームを取得する
	/// </summary>
	/// <returns>collisionフレームを返す</returns>
	int GetColFrameIndex() const { return colFrameIndex_; }

	/// <summary>
	/// ポジションを取得する
	/// </summary>
	/// <returns>ポジションを返す</returns>
	VECTOR GetPos() const { return pos_; }

	/// <summary>
	/// 回転値を取得する
	/// </summary>
	/// <returns>回転値</returns>
	VECTOR GetRot()const { return rot_; }

	/// <summary>
	/// 現在のアニメーション番号を取得する
	/// </summary>
	/// <returns>アニメーション番号</returns>
	int GetCurrentAnimNo() const { return animNext_.animNo; }

	/// <summary>
	/// アニメーションの再生時間を取得する
	/// </summary>
	/// <returns>一アニメーションの再生時間</returns>
	float GetAnimTotalTime()const { return animNext_.totalTime; }

	/// <summary>
	/// オブジェクトのマテリアルタイプを取得
	/// </summary>
	/// <returns>タイプ</returns>
	Material GetMaterialType() const { return materialType_; }

	////////////////Setter////////////////

	/// <summary>
	/// ポジションを設定する
	/// </summary>
	/// <param name="pos">ポジション</param>
	void SetPos(const VECTOR& pos);

	/// <summary>
	/// 角度を設定する
	/// </summary>
	/// <param name="rot">角度</param>
	void SetRot(const VECTOR& rot);

	/// <summary>
	/// サイズを設定する
	/// </summary>
	/// <param name="scale">拡縮率</param>
	void SetScale(const VECTOR& scale);

	/// <summary>
	/// collisionとして使いたいフレームを指定する
	/// </summary>
	/// <param name="collFrameName">collisionとして使いたいフレーム名。なければ空</param>
	void SetCollFrame(const std::string& collFrameName = "");

	/// <summary>
	/// アニメーションを設定する
	/// </summary>
	/// <param name="animNo">アニメーション番号</param>
	/// <param name="isLoop">ループ再生するか</param>
	/// <param name="IsForceChange">強制的に変更するか</param>
	void SetAnimation(const int animNo, const bool isLoop, const bool IsForceChange);

	/// <summary>
	/// アニメーション番号の最後のフレームを設定する
	/// </summary>
	/// <param name="animNo">アニメーション番号</param>
	void SetAnimEndFrame(const int animNo);

	/// <summary>
	/// アニメーションのフレームを設定する
	/// </summary>
	/// <param name="value">フレーム数</param>
	void SetAnimationFrame(const float value);

private:
	//計　14byte
	struct AnimData {
		int animNo;			//4byte
		int attachNo;		//4byte
		float totalTime;	//4byte
		bool isLoop;		//2byte
	};
private:
	/// <summary>
	/// アニメーションデータを初期化する
	/// </summary>
	/// <param name="anim"></param>
	void ClearAnimData(AnimData& anim);

	/// <summary>
	/// アニメーションの更新
	/// </summary>
	/// <param name="anim"></param>
	/// <param name="dt"></param>
	void UpdateAnim(const AnimData& anim, const  float dt = 1.0f);

	/// <summary>
	/// アニメーションのブレンド
	/// </summary>
	void UpdateAnimBlendRate();
private:
	int modelHandle_ = -1;				//モデルハンドル
	int colFrameIndex_ = 0;				//衝突判定用フレーム
	int animChangeFrame_ = 0;			//アニメーションの変更フレーム
	int animChangeFrameTotal_ = 0;		//アニメーションを変更するのにかかる総時間
										
	bool isUseCollision_ = false;		//衝突判定を行うか
	bool isUpdateCollsion_ = false;		//衝突判定用フレームの更新を行うか
										
	VECTOR pos_ = {};					//ポジション
	VECTOR rot_ = {};					//回転

	AnimData animPrev_ = {};			//前のアニメーション情報
	AnimData animNext_ = {};			//次のアニメーション情報
										
	Material materialType_ = {};		//マテリアルの素材
};

