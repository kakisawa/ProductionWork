#pragma once
#include "DxLib.h"

class Player
{
public:
	Player();
	~Player();

	void Init();
	void Update();
	void Draw();
	void End();

	VECTOR GetPos()const { return m_pos; }	// ���W�n��

private:

	// �v���C���[�̏��
	enum class State
	{
		kIdle=1,	// �ҋ@
		kWalk=2,	// �ړ�
		kAttack=30,	// �U��
	};
	// �A�j���[�V�������
	enum class AnimKind
	{
		kNone = -1,	// ����
		kUnKnown=0,	// �s��
		kIdle=1,	// �ҋ@
		kWalk=2,	// �ړ�
		kAttack=30,	// �U��
	};

	// �A�j���[�V������Ԃ̍X�V
	void UpdateAnimState(State state);
	// �A�j���[�V��������
	void UpdateAnim();
	// �A�j���[�V�������Đ�����
	void PlayAnim(AnimKind animIndex);

	State MoveValue();						// �v���C���[�̈ړ��l�ݒ�
	void Move(const VECTOR& MoveVector);	// �v���C���[�̈ړ�����
	State Attack();							// �v���C���[�̍U������


private:
	int m_model;			// �v���C���[���f��
	float m_angle;			// �v���C���[�����p�x
	
	bool m_isWalk;			// �ړ����t���O
	bool m_isAttack;		// �U�����t���O

	// �A�j���[�V�������
	int m_currentAnimNo;		// �Đ����̃A�j���[�V�����̃A�^�b�`�ԍ�
	float m_currentAnimCount;	// �Đ����̃A�j���[�V�����̍Đ�����
	float m_prevAnimNo;			// �O�̍Đ��A�j���[�V�����̃A�^�b�`�ԍ�
	float m_prevAnimCount;		// �O�̍Đ��A�j���[�V�����̍Đ�����
	float m_animBlendRate;		// ���݂Ɖߋ��̃A�j���[�V�����̍�������
									// 0.0f:prev���Đ������
									// 1.0f:current���Đ������

	State m_currentState;	// ���݂̃v���C���[�̏��

	VECTOR m_pos;			// �v���C���[�ʒu
	VECTOR m_move;			// �ړ���
};