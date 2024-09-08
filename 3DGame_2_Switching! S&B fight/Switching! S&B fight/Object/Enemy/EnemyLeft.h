#pragma once
#include "EnemyBase.h"
#include "../../Util/Collision.h"
#include <array>

class Player;
class EnemyState;
class GameMap;
class Effect;
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

	void Move();

	void UIDraw();

	void SetModelFramePosition(int ModelHandle, const char *FrameName, int SetModelHandle);

	Collision GetColSphere() const { return m_colSphere; }

	int GetHp() const { return m_hp; }
	VECTOR GetPos() const { return m_pos; }

private:
	// �A�j���[�V�������
	struct AnimationData
	{
		int8_t kIdle = 15;
		int8_t kWalk = 89;
	};

	// �e��Ԃ��Ƃ̏�����
	void IdleStateInit() {};
	void WalkStateInit() {};

	// �e��Ԃ��Ƃ̍X�V
	void IdleStateUpdate();
	void WalkStateUpdate();

private:
	int m_sordModel;
	bool m_isWalk;
	bool m_isEffect;

	float m_angle;

	std::array<int, 4> m_uiGraph{};	// UI�p�摜


	struct MapPlace
	{
		VECTOR leftBack;		// �}�b�v�������W
		VECTOR rightFront;	// �}�b�v�E�O���W
	}mp;

	VECTOR m_upPos;			// �J�v�Z������W
	VECTOR m_vecToPlayer;	// �v���C���[���̋���
	VECTOR m_targetPos;

	std::shared_ptr<EnemyState> m_pState;

	AnimationData m_animData;

	Collision m_colSphere;

	std::shared_ptr<Effect> m_pEffect;		// �G�t�F�N�g
};

