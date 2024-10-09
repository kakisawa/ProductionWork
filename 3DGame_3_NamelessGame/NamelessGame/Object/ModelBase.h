#pragma once
#include "DxLib.h"

class ModelBase
{
public:
	ModelBase();
	virtual ~ModelBase();

	virtual void Init();
	virtual void Update();
	virtual void Draw();

	// プレイヤーデータ
	struct CharaData
	{
		int maxHp;		// 最大HP
		float initPosX;	// 初期化座標X
		float initPosY; // 初期化座標Y
		float initPosZ; // 初期化座標Z
		float modelSize;// モデルサイズ
		float walkSpeed;// 歩く速度
		float rotaSpeed;// 回転速度

	}m_chara;

	// アニメーション関係

	/// <summary>
	/// アニメーションを変更させる
	/// </summary>
	/// <param name="animNo">アニメーション番号</param>
	/// <param name="isLoop">ループ再生させるか</param>
	/// <param name="isForceChange">既に指定されたアニメーションが再生されている場合も変更するか</param>
	/// <param name="changeFrame">何フレームかけて変化させるのか</param>
	void ChangeAnimation(const int animNo, const bool isLoop, const bool isForceChange, const int changeFrame);

	/// <summary>
	/// アニメーションを設定する
	/// </summary>
	/// <param name="animNo">アニメーション番号</param>
	/// <param name="isLoop">ループ再生させるか</param>
	/// <param name="isForceChange">既に指定されたアニメーションが再生されている場合も変更するか</param>
	void SetAnimation(const int animNo,const bool isLoop,const bool isForceChange);

	/// <summary>
	/// アニメーションが終了しているかどうか
	/// </summary>
	/// <returns>アニメーションが終了したか</returns>
	bool IsAnimEnd();

protected:
	int m_model;				// モデル
	float m_angle;			// プレイヤー向き角度

	VECTOR m_pos;			// 座標
	VECTOR m_move;			// 移動量
	VECTOR m_targetDir;		// プレイヤーが向くべき方向のベクトル

	/*アニメーション関係*/
	int m_animChangeFrame;			// 現在の切り替えフレーム数
	int m_animChangeFrameTotal;		// 切り替えにかける総フレーム数

	// アニメーション情報
	struct AnimData {
		int animNo;		// アニメーション番号
		int attachNo;	// アタッチ番号
		float totalTime;// 総再生時間
		float isLoop;	// ループさせるか
	};

	// アニメーションのアタッチ番号
	AnimData m_animPrev;	// 変更前アニメーション情報
	AnimData m_animNext;	// 変更後アニメーションデータ

	/// <summary>
	/// アニメーションデータの初期化
	/// </summary>
	/// <param name="anim">アニメーション情報</param>
	void InitAnim(AnimData& anim);

	/// <summary>
	/// アニメーションの更新
	/// </summary>
	/// <param name="anim">アニメーション情報</param>
	/// <param name="dt"></param>
	void UpdateAnim(const AnimData& anim, const float dt = 0.5f);

	/// <summary>
	/// アニメーションのブレンド率の設定
	/// </summary>
	void UpdateAnimBlendRate();	

};