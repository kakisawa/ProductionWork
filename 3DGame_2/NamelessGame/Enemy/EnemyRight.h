#pragma once
#include "EnemyBase.h"

class EnemyState;
class EnemyRight :
	public EnemyBase
{
public:
	EnemyRight();
	~EnemyRight();

	void Init()override;
	void Update()override;
	void Draw()override;
	void End()override;
	
	void SetModelFramePosition(int ModelHandle, char* FrameName, int SetModelHandle);

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

	std::shared_ptr<EnemyState> m_pState;

	AnimationData m_animData;
};