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

	std::array<int, 4> m_uiGraph{};	// UI�p�摜

	struct MapPlace
	{
		VECTOR leftBack;		// �}�b�v�������W
		VECTOR rightFront;	// �}�b�v�E�O���W
	}mp;
	
	VECTOR m_upPos;			// �J�v�Z������W


	std::shared_ptr<EnemyState> m_pState;

	AnimationData m_animData;

	Collision m_colSphere;
};