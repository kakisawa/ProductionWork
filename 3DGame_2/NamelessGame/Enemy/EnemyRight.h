#pragma once
#include "EnemyBase.h"
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
	
	VECTOR m_UpPos;			// �J�v�Z������W


	std::shared_ptr<EnemyState> m_pState;

	AnimationData m_animData;
};