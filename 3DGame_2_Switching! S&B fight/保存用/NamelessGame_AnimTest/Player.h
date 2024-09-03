#pragma once
#include "Animation.h"
#include "DxLib.h"
#include <memory>

class Camera;
class PlayerState;
class Player:public Animation
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
	struct AnimationData
	{
		int8_t kIdle = 1;		//�ҋ@���[�V����
		int8_t kWalk = 2;		//�������[�V����
		int8_t kJump = 12;		//�W�����v�����[�V����
		int8_t kAttack1 = 30;	//�U�����[�V����1
		int8_t kAttack2 = 31;	//�U�����[�V����2
		int8_t kAttack3 = 32;	//�U�����[�V����3
		int8_t kAttack4 = 33;	//�U�����[�V����4
	};


	// �A�j���[�V�������
	//enum class AnimKind
	//{
	//	kNone = -1,	// ����
	//	kUnKnown=0,	// �s��
	//	kIdle=1,	// �ҋ@
	//	kWalk=2,	// �ړ�
	//	kJump=12,	// �W�����v
	//	kAttack1 = 30,	// �ʏ팕�U��1
	//	kAttack2 = 31,	// �ʏ팕�U��2
	//	kAttack3 = 32,	// �ʏ팕�U��3
	//	kAttack4 = 33,	// �ʏ팕�U��4

	//	kAttack5 = 58,	// �ʏ�e�U��(�A�˂̂�)
	//
	//};

	// �ʏ팕�U�����
	//enum class AttackKind
	//{
	//	kNone=-1,			// ����
	//	kNormalAttack1 = 1,	// �ʏ팕�U��1
	//	kNormalAttack2 = 2,	// �ʏ팕�U��2
	//	kNormalAttack3 = 3,	// �ʏ팕�U��3
	//	kNormalAttack4 = 4,	// �ʏ팕�U��4

	//	kNormalShot = 5,	// �ʏ�e�U��

	//};

	// �e��Ԃ��Ƃ̏�����
	void IdleStateInit() {};			//�ҋ@��Ԃ̏�����
	void WalkStateInit() {};			//������Ԃ̏�����

	void JumpStateInit();			//�W�����v��Ԃ̏�����
	void AttackStateInit();			//�U����Ԃ̏�����

	// �e��Ԃ��Ƃ̍X�V
	void IdleStateUpdate();
	void WalkStateUpdate();
	void JumpStateUpdate();
	void AttackStateUpdate();




	//// �A�j���[�V������Ԃ̍X�V
	//void OldUpdateAnimState(State state);
	//// �A�j���[�V��������
	//void OldUpdateAnim();
	//// �A�j���[�V�������Đ�����
	//void OldPlayAnim(AnimKind animIndex);

	// �v���C���[�̈ړ��l�ݒ�
	void OldMoveValue(const Camera& camera, VECTOR& upMoveVec, VECTOR& leftMoveVec);
	void Move(const VECTOR& MoveVector);	// �v���C���[�̈ړ�����
	void Angle();							// �v���C���[�̉�]����
	
	void Attack();		// �U������
	void Jump();		// �W�����v����

private:
	int m_model;				// �v���C���[���f��
	float m_angle;				// �v���C���[�����p�x
	float m_jumpPower;			// �x�������̑��x
	float m_gravity;			// �v���C���[�ɂ�����d��
	
	bool m_isWalk;			// �ړ����t���O
	bool m_isAttack;		// �U�����t���O
	bool m_nextAttackFlag;	// ���̍U�������s����邩�̃t���O
	bool m_isJump;			// �W�����v���t���O

	// �v���C���[�X�e�C�g�|�C���^
	std::shared_ptr<PlayerState> m_pState;

	// �v���C���[�A�j���[�V�����f�[�^
	AnimationData m_animData;

	int m_multiAttack;		// �A���U���p�ϐ�
	bool m_isNextAttack;	// ���̍U�����s�����̃t���O

	VECTOR m_pos;			// �v���C���[�ʒu
	VECTOR m_move;			// �ړ���
	VECTOR m_targetDir;		// �v���C���[�������ׂ������̃x�N�g��
};