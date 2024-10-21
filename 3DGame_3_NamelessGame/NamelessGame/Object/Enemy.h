#pragma once
#include "ModelBase.h"

class Map;
class Enemy :
	public ModelBase
{
public:
	struct Situation {
		bool isMoving = false;	// 移動中
	};

	// 敵の状態
	struct EnemyStatus {
		int animNo = -1;		// アニメーション番号
		float animSpeed = 0.0f;	// アニメーション再生スピード
		Situation situation;	// 状態
		bool isLoop = false;	// ループ再生
	}m_status;

	// アニメーション
	enum class EnemyAnim {
		Idle,		// 待機
		Walk,		// 歩く
		Run,			// 走る
		Provocation,	// 挑発
		Attack1,		// 攻撃1
		Attack2,		// 攻撃2
		Hit,			// 被ダメージ
		BlownAway,		// 吹っ飛び
		Death,			// 死亡
		MAX,
	};

	// アニメーション切り替えにかかる時間
	struct AnimChangeTime {
		int Idle;			// 待機
		int Walk;			// 歩く
		int Run;			// 走る
		int Provocation;	// 挑発
		int Attack1;		// 攻撃1
		int Attack2;		// 攻撃2
		int Hit;			// 被ダメージ
		int BlownAway;		// 吹っ飛び
		int Death;			// 死亡
	}m_animChangeTime;

	// アニメーションの再生速度
	struct AnimSpeed {
		float Default = 0.5f;	// 基本
		float Idle = 0.5f;		// 待機
		float Walk;				// 歩く
		float Run;				// 走る
		float Provocation;		// 挑発
		float Attack1;			// 攻撃1
		float Attack2;			// 攻撃2
		float Hit;				// 被ダメージ
		float BlownAway;		// 吹っ飛び
		float Death;			// 死亡
	}m_animSpeed;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Enemy();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const Map& map);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="map"></param>
	void Move(const Map& map);

	/// <summary>
	/// 一番近い座標を出す
	/// </summary>
	/// <param name="map"></param>
	void SearchNearPosition(const Map& map);



	/// <summary>
	/// 座標渡し
	/// </summary>
	/// <returns>座標</returns>
	VECTOR GetPos() const { return m_pos; }

private:
	VECTOR m_targetPos;	// 目標座標
};