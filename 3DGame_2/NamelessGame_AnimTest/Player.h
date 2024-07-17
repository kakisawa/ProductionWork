#pragma once
#include "DxLib.h"
#include<memory>
#include <stdint.h>

class Camera;
class PlayerState;
class Player
{
public:
	Player();
	~Player();

	void Init();
	void Update(const Camera& camera);
	void Draw();
	void End();

	VECTOR GetPos()const { return m_pos; }	// ���W�n��

private:

	
	// �A�j���[�V�������
	enum class AnimKind
	{
		kNone = -1,	// ����
		kUnKnown=0,	// �s��
		kIdle=1,	// �ҋ@
		kWalk=2,	// �ړ�
		kJump=12,	// �W�����v
		kFall=14,	// ����
		kAttack1 = 30,	// �ʏ팕�U��1
		kAttack2 = 31,	// �ʏ팕�U��2
		kAttack3 = 32,	// �ʏ팕�U��3
		kAttack4 = 33,	// �ʏ팕�U��4

		kAttack5 = 58,	// �ʏ�e�U��(�A�˂̂�)
	
	};

	// �A�j���[�V�������
	struct AnimKindStruct
	{
		int8_t kNone = 0;	// ����
		int8_t kUnKnown = 0;	// �s��
		int8_t kIdle = 0;	// �ҋ@
		int8_t kWalk = 0;	// �ړ�
		int8_t kJump = 0;	// �W�����v
		int8_t kFall = 0;	// ����
		int8_t kAttack1 = 0;	// �ʏ팕�U��1
		int8_t kAttack2 = 0;	// �ʏ팕�U��2
		int8_t kAttack3 = 0;	// �ʏ팕�U��3
		int8_t kAttack4 = 0;	// �ʏ팕�U��4

		int8_t kAttack5 = 0;	// �ʏ�e�U��(�A�˂̂�)

	};

	//�A�j���[�V�������x
	enum kStateAnimSpeed
	{
		Idle = 2,				//�ҋ@
		Walk = 2,				//����
		Jump = 1,				//�W�����v
		Attack = 1,				//�U��
	};

	// �ʏ팕�U�����
	enum class AttackKind
	{
		kNormalAttack1 = 1,
		kNormalAttack2 = 2,
		kNormalAttack3 = 3,
		kNormalAttack4 = 4,

		kNormalShot = 5,

	};

	struct AnimData
	{
		int animNo;			// �A�j���[�V�����ԍ�
		int attachNo;		// �A�^�b�`�ԍ�
		float totalTime;	// �A�j���[�V�����̑��Đ�����
		float elapsedTime;	// �A�j���[�V�����̌o�ߎ���
		bool isLoop;		// �A�j���[�V���������[�v���邩
	};


	// �A�j���[�V������Ԃ̍X�V
	void UpdateAnimState(State state);
	// �A�j���[�V��������
	void UpdateAnim();
	// �A�j���[�V�������Đ�����
	void PlayAnim(AnimKind animIndex);


	// ���������A�j���[�V��������
	void InitAnim(AnimData& anim);
	// �A�j���[�V�����̐ݒ肷��
	void SetAnim(int animNo, bool loop, bool isForceChange);
	// �A�j���[�V������ω�������
	void ChangeAnimation(int animNo, bool Loop, bool isForceChange, int changeFrame);
	// ���݂̃A�j���[�V�������I�����Ă��邩�ǂ���
	bool IsAnimEnd();
	// �A�j���[�V�����̍X�V(��)
	void UpdateAnimation(AnimData anim, float dt);
	// �A�j���[�V�����̃u�����h�Ǘ�
	void UpdateAnimBlendRate();

private:
	void IdleStateInit(){}
	void WalkStateInit(){}
	void JumpStateInit();
	void AttackStateInit();

	// �e��Ԃ��Ƃ̍X�V
	void IdleStateUpdate();			//�ҋ@���̍X�V
	void WalkStateUpdate();			//�������̍X�V
	void JumpStateUpdate();			//�W�����v���̍X�V
	void AttackStateUpdate();		//�ʏ�U�����̍X�V
	

	State MoveValue(const Camera& camera, VECTOR& upMoveVec, VECTOR& leftMoveVec);						// �v���C���[�̈ړ��l�ݒ�
	void Move(const VECTOR& MoveVector);	// �v���C���[�̈ړ�����
	void Angle();							// �v���C���[�̉�]����
	State AttackState();					// �v���C���[�̍U������
	State JumpState();						// �v���C���[�̃W�����v����

	void Attack();		// �U������
	void Jump();		// �W�����v����

private:
	int m_model;				// �v���C���[���f��
	float m_angle;				// �v���C���[�����p�x

	float m_currentJumpPower;	// �x�������̑��x
	float m_gravity;			// �v���C���[�ɂ�����d��
	
	bool m_isWalk;			// �ړ����t���O
	bool m_isAttack;		// �U�����t���O
	bool m_isNextAttack;
	//bool m_isForward;		// �U�����ɏ����O�i����t���O
	bool m_nextAttackFlag;	// ���̍U�������s����邩�̃t���O
	bool m_isJump;			// �W�����v���t���O

	// �A�j���[�V�������

	AnimData m_current;		// �ύX��A�j���[�V�����f�[�^
	AnimData m_prev;		// �ύX�O�A�j���[�V�����f�[�^

	int m_animChangeFrameTotal = 1;	// �؂�ւ��ɂ����鑍�t���[����
	int m_animChangeFrame = 1;		// ���݂̐؂�ւ��t���[����

	AnimKindStruct m_animData;		

	int m_animSpeed;		


	//int m_currentAnimNo;		// �Đ����̃A�j���[�V�����̃A�^�b�`�ԍ�
	//float m_currentAnimCount;	// �Đ����̃A�j���[�V�����̍Đ�����
	//int m_prevAnimNo;			// �O�̍Đ��A�j���[�V�����̃A�^�b�`�ԍ�
	//float m_prevAnimCount;		// �O�̍Đ��A�j���[�V�����̍Đ�����
	float m_animBlendRate;		// ���݂Ɖߋ��̃A�j���[�V�����̍�������
									// 0.0f:prev���Đ������
									// 1.0f:current���Đ������

	int m_multiAttack;		// �A���U���p�ϐ�

	State m_currentState;	// ���݂̃v���C���[�̏��

	VECTOR m_pos;			// �v���C���[�ʒu
	VECTOR m_move;			// �ړ���
	//VECTOR m_moveAttack;	// �U�����̉����l
	VECTOR m_targetDir;		// �v���C���[�������ׂ������̃x�N�g��

	//�v���C���[�X�e�C�g�|�C���^
	std::shared_ptr<PlayerState> m_pState;
};