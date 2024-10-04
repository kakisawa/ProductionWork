#pragma once
#include "ModelBase.h"
class Enemy :
    public ModelBase
{
	struct Situation {
		bool isMoving = false;					//移動中か	
	};

	struct PlayerStatus {
		int animNo;				// アニメーション番号
		Situation situation;	// 状態
		bool isLoop;			// ループ再生
	}m_status;

	// アニメーション
	enum class EnemyAnim {
		Idle,			// 待機
		Walk,			// 歩く
		Run,			// 走る
		Provocation,	// 挑発
		Attack1,		// 攻撃1
		Attack2,		// 攻撃2
		Hit,			// 被ダメージ
		BlownAway,		// 吹っ飛び
		Death,			// 死亡
		MAX,
	};

	struct AnimChangeTime
	{
		int Idle;
		int Walk;
		int Run;
		int Provocation;
		int Attack1;
		int Attack2;
		int Hit;
		int BlownAway;
		int Death;
	}m_animChangeTime;


public:
    Enemy();
    ~Enemy();

    void Init() override;
    void Update() override;
    void Draw() override;

	/// <summary>
	/// アニメーションの変更
	/// </summary>
	/// <param name="anim">選択するアニメーション</param>
	/// <param name="isAnimLoop">ループさせるか</param>
	/// <param name="changeTime">切り替えにかかる時間</param>
	void ChangeAnimNo(const EnemyAnim anim, const bool isAnimLoop, const int changeTime);

	/// <summary>
	/// アニメーションを待機状態に変更する
	/// </summary>
	void ChangeAnimIdle();

private:
};