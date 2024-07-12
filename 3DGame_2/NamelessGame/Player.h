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
		kWalk=3,	// �ړ�
		kAttack=30,	// �U��
	};
	// �A�j���[�V�������
	enum class AnimKind
	{
		kNone = -1,	// ����
		kIdle=0,	// �ҋ@
		kWalk=1,	// �ړ�
		kAttack,	// �U��
	};

	// �A�j���[�V�����X�e�[�g�̍X�V
	void UpdateAnimState(State state);
	// �A�j���[�V��������
	void UpdateAnim();
	// �A�j���[�V�������Đ�����
	void PlayAnim(AnimKind animIndex);

	void Move();	// �v���C���[�̈ړ�����
	bool UpdateAnim(int attachNo);	// �A�j���[�V�����̐i�s
	// ���[�v�������ǂ�����Ԃ�
	void ChangeAnim(int animIndex);	// �A�j���[�V�����̕ύX

private:
	int m_model;			// �v���C���[���f��

	int m_currentAnimNo;	// ���݂̃A�j���[�V����
	int m_prevAnimNo;		// �ύX�O�̃A�j���[�V����
	float m_animBlemdRate;	// �A�j���[�V������������
								// 0.0f:prev���Đ������
								// 1.0f:current���Đ������

	float m_angle;			// �v���C���[�����p�x

	bool m_isWalk;
	bool m_isAttack;			// �U�����t���O

	VECTOR m_pos;			// �v���C���[�ʒu
	VECTOR m_move;			// �ړ���
};