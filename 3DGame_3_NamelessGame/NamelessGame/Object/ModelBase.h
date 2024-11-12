#pragma once
#include "../Collision.h"
#include "DxLib.h"

class ModelBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ModelBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ModelBase();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// プレイヤーデータ
	/// </summary>
	struct CharaCommonData
	{
		int maxHp;			// 最大HP
		float initPosX;		// 初期化座標X
		float initPosY;		// 初期化座標Y
		float initPosZ;		// 初期化座標Z
		float modelSize;	// モデルサイズ
		float walkSpeed;	// 歩く速度
		float rotaSpeed;	// 回転速度
		float bodyColRad;	// 体当たり判定半径
		float bodyColUpY;	// 当たり判定上座標
	}m_chara;


	/// <summary>
	/// アニメーションを変更させる
	/// </summary>
	/// <param name="animNo">アニメーション番号</param>
	/// <param name="animSpeed">アニメーションの再生速度</param>
	/// <param name="isLoop">ループ再生させるか</param>
	/// <param name="isForceChange">既に指定されたアニメーションが再生されている場合も変更するか</param>
	/// <param name="changeFrame">何フレームかけて変化させるのか</param>
	void ChangeAnimation(const int animNo, const float animSpeed, const bool isLoop, const bool isForceChange, const int changeFrame);

	/// <summary>
	/// アニメーションを設定する
	/// </summary>
	/// <param name="animNo">アニメーション番号</param>
	/// <param name="animSpeed">アニメーションの再生速度</param>
	/// <param name="isLoop">ループ再生させるか</param>
	/// <param name="isForceChange">既に指定されたアニメーションが再生されている場合も変更するか</param>
	void SetAnimation(const int animNo, const float animSpeed, const bool isLoop, const bool isForceChange);

	/// <summary>
	/// アニメーションが終了しているかどうか
	/// </summary>
	/// <returns>アニメーションが終了したか</returns>
	bool IsAnimEnd();

	/// <summary>
	/// ループアニメーションが終了したかどうか
	/// </summary>
	/// <returns></returns>
	bool IsLoopAnimEnd();

	// 死亡しているかどうかのフラグを渡す
	bool GetDeathFlag()const { return m_deathFlag; }

protected:
	int m_model;			// モデル
	int m_hp;				// HP
	int m_attack;			// 攻撃力
	float m_angle;			// プレイヤー向き角度
	float m_nextAnimTime;	// 新しいアニメーションの再生時間

	bool m_deathFlag;		// 死亡しているかのフラグ

	VECTOR m_pos;			// 座標
	VECTOR m_move;			// 移動量
	VECTOR m_targetDir;		// プレイヤーが向くべき方向のベクトル

	Collision m_col;		// 当たり判定

	// アニメーション関係

	int m_animChangeFrame;		// 現在の切り替えフレーム数
	int m_animChangeFrameTotal;	// 切り替えにかける総フレーム数

	bool m_isLoopFinish;		// ループ再生中に1回のアニメーションが終了したかのフラグ

	/// <summary>
	/// アニメーション情報
	/// </summary>
	struct AnimData {
		int animNo;		// アニメーション番号
		int attachNo;	// アタッチ番号
		float animSpeed;// アニメーションの再生速度
		float time;		// 再生時間
		float totalTime;// 総再生時間
		float isLoop;	// ループさせるか
	};

	// アニメーションのアタッチ番号
	AnimData m_animPrev;	// 前のアニメーション情報
	AnimData m_animNext;	// 新しいアニメーション情報

	/// <summary>
	/// アニメーション情報の初期化
	/// </summary>
	/// <param name="anim">アニメーション情報</param>
	void InitAnim(AnimData& anim);

	/// <summary>
	/// アニメーションの更新
	/// </summary>
	/// <param name="anim">アニメーション情報</param>
	/// <param name="dt"></param>
	void UpdateAnim(const AnimData& anim);

	/// <summary>
	/// アニメーションのブレンド率の設定
	/// </summary>
	void UpdateAnimBlendRate();
};