#pragma once
#include "DxLib.h"

class Camera;

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

	// �v���C���[�̏��
	enum class State
	{
		kIdle=1,	// �ҋ@
		kWalk=2,	// �ړ�
		kJump=3,	// �W�����v
		kAttack=4,	// �U��
	};
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

	// �ʏ팕�U�����
	enum class AttackKind
	{
		kNormalAttack1 = 1,
		kNormalAttack2 = 2,
		kNormalAttack3 = 3,
		kNormalAttack4 = 4,

		kNormalShot = 5,

	};

	// �A�j���[�V������Ԃ̍X�V
	void UpdateAnimState(State state);
	// �A�j���[�V��������
	void UpdateAnim();
	// �A�j���[�V�������Đ�����
	void PlayAnim(AnimKind animIndex);

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
	bool m_isForward;		// �U�����ɏ����O�i����t���O
	bool m_nextAttackFlag;	// ���̍U�������s����邩�̃t���O
	bool m_isJump;			// �W�����v���t���O

	// �A�j���[�V�������
	int m_currentAnimNo;		// �Đ����̃A�j���[�V�����̃A�^�b�`�ԍ�
	float m_currentAnimCount;	// �Đ����̃A�j���[�V�����̍Đ�����
	int m_prevAnimNo;			// �O�̍Đ��A�j���[�V�����̃A�^�b�`�ԍ�
	float m_prevAnimCount;		// �O�̍Đ��A�j���[�V�����̍Đ�����
	float m_animBlendRate;		// ���݂Ɖߋ��̃A�j���[�V�����̍�������
									// 0.0f:prev���Đ������
									// 1.0f:current���Đ������

	int m_multiAttack;		// �A���U���p�ϐ�

	State m_currentState;	// ���݂̃v���C���[�̏��

	VECTOR m_pos;			// �v���C���[�ʒu
	VECTOR m_move;			// �ړ���
	VECTOR m_moveAttack;	// �U�����̉����l
	VECTOR m_targetDir;		// �v���C���[�������ׂ������̃x�N�g��
};