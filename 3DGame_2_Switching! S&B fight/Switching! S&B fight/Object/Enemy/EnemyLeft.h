#pragma once
#include "EnemyBase.h"
#include "../../Util/Collision.h"
#include <array>

class Player;
class EnemyState;
class GameMap;
class Effect;
class SoundManager;
class EnemyLeft :
    public EnemyBase
{
public:
	EnemyLeft();
	~EnemyLeft();
	
	void Init(std::shared_ptr<GameMap> pMap)override;
	void Update(const Player& player)override;
	void Draw()override;
	void End()override;

	void Move(const Player& player);

	void UIDraw();

	void SetModelFramePosition(int ModelHandle, const char *FrameName, int SetModelHandle);

	Collision GetColSphere() const { return m_colSphere; }

	int GetHp() const { return m_hp; }
	int GetAlive()const { return m_isAlive; }
	VECTOR GetPos() const { return m_pos; }
	VECTOR GetMovePos() const { return m_move; }

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
	float m_angle;
	bool m_isWalk;
	bool m_isEffect;
	bool m_isMoveFlag;		// プレイヤーへの移動

	std::array<int, 4> m_uiGraph{};	// UI用画像

	struct MapPlace
	{
		VECTOR leftBack;		// マップ左奥座標
		VECTOR rightFront;	// マップ右前座標
	}mp;

	VECTOR m_upPos;			// カプセル上座標
	VECTOR m_vecToPlayer;	// プレイヤー迄の距離
	VECTOR m_targetPos;
	VECTOR m_move;
	VECTOR m_distance;

	std::shared_ptr<EnemyState> m_pState;

	AnimationData m_animData;

	Collision m_colSphere;

	SoundManager* m_pSound;

	std::shared_ptr<Effect> m_pEffect;		// エフェクト
};