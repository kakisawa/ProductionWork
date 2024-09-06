#pragma once
#include "EnemyBase.h"
#include "../../Util/Collision.h"
#include <array>

class Player;
class EnemyState;
class EnemyRight :
	public EnemyBase
{
public:
	EnemyRight();
	~EnemyRight();

	void Init(std::shared_ptr<GameMap> pMap)override;
	void Update(const Player& player)override;
	void Draw()override;
	void End()override;

	void UIDraw();
	
	void SetModelFramePosition(int ModelHandle, const char* FrameName, int SetModelHandle);

	Collision GetColSphere() const { return m_colSphere; }

	int GetHp() const { return m_hp; }
	VECTOR GetPos() const { return m_pos; }

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

	std::array<int, 4> m_uiGraph{};	// UI用画像

	struct MapPlace
	{
		VECTOR leftBack;		// マップ左奥座標
		VECTOR rightFront;	// マップ右前座標
	}mp;
	
	VECTOR m_upPos;			// カプセル上座標


	std::shared_ptr<EnemyState> m_pState;

	AnimationData m_animData;

	Collision m_colSphere;
};