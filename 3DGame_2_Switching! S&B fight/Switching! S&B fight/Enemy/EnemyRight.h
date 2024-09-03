#pragma once
#include "EnemyBase.h"
#include "../Util/Collision.h"

class Player;
class EnemyState;
class EnemyRight :
	public EnemyBase
{
public:
	EnemyRight();
	~EnemyRight();

	void Init()override;
	void Update(const Player& player)override;
	void Draw()override;
	void End()override;
	
	void SetModelFramePosition(int ModelHandle, char* FrameName, int SetModelHandle);

	Collision GetColSphere() const { return m_colSphere; }

private:
	// アニメーション情報
	struct AnimationData
	{
		int8_t kIdle = 15;
		int8_t kWalk = 89;
	};

	// 各状態ごとの初期化
	void IdleStateInit() {};
	void WalkStateInit() {};

	// 各状態ごとの更新
	void IdleStateUpdate();
	void WalkStateUpdate();

private:
	int m_sordModel;
	
	VECTOR m_upPos;			// カプセル上座標


	std::shared_ptr<EnemyState> m_pState;

	AnimationData m_animData;

	Collision m_colSphere;
};