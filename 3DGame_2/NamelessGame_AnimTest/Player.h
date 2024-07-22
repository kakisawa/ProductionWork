#pragma once
#include "DxLib.h"
#include <memory>

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
	struct AnimationData
	{
		int8_t kIdle = 0;		//�ҋ@���[�V����
		int8_t kWalk = 0;		//�������[�V����
		int8_t kJump = 0;		//�W�����v�����[�V����
		int8_t kAttack1 = 0;	//�U�����[�V����1
		int8_t kAttack2 = 0;	//�U�����[�V����2
		int8_t kAttack3 = 0;	//�U�����[�V����3
		int8_t kAttack4 = 0;	//�U�����[�V����4
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

	struct AnimData			// �A�j���[�V�����f�[�^
	{
		int animNo;			// �A�j���[�V�����ԍ�
		int attachNo;		// �A�^�b�`�ԍ�
		float totalTime;	// �A�j���[�V�����̑��Đ�����
		float elapsedTime;	// �A�j���[�V�����̌o�ߎ���
		bool isLoop;		// �A�j���[�V���������[�v���邩
	};

	// �Z�V�K�쐬_�A�j���[�V����
	// �A�j���[�V������Ԃ̏�����
	void InitAnim(AnimData& anim);
	/// <summary>
	/// �A�j���[�V�����ύX
	/// </summary>
	/// <param name="animNo">�ύX��A�j���[�V�����ԍ�</param>
	/// <param name="isLoop">���[�v�Đ������邩</param>
	/// <param name="isForceChange">���Ɏw�肳�ꂽ�A�j���[�V�������Đ�����Ă���ꍇ���Đ����邩</param>
	/// <param name="isChangeFrame">���t���[�������Đ؂�ւ���</param>
	void ChangeAnim(int animNo, bool isLoop, bool isForceChange, bool isChangeFrame);
	// ���݂̃A�j���[�V�����؂�ւ���񂩂�A�j���[�V�����̃u�����h����ݒ肷��
	void UpdateAnimBlendRate();
	// �A�j���[�V�����̍X�V
	void UpdateAnimation(AnimData anim,float dt=1.0f);

	// �e��Ԃ��Ƃ̍X�V
	void IdleStateUpdate();
	void WalkStateUpdate();
	void JumpStateUpdate();
	void AttackStateUpdate();




	// �A�j���[�V������Ԃ̍X�V
	void OldUpdateAnimState(State state);
	// �A�j���[�V��������
	void OldUpdateAnim();
	// �A�j���[�V�������Đ�����
	void OldPlayAnim(AnimKind animIndex);

	// �v���C���[�̈ړ��l�ݒ�
	void OldMoveValue(const Camera& camera, VECTOR& upMoveVec, VECTOR& leftMoveVec);
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
	//bool m_isForward;		// �U�����ɏ����O�i����t���O
	bool m_nextAttackFlag;	// ���̍U�������s����邩�̃t���O
	bool m_isJump;			// �W�����v���t���O


	// �A�j���[�V�������
	AnimData m_current;		// �ύX��A�j���[�V�����f�[�^
	AnimData m_prev;		// �ύX�O�A�j���[�V�����f�[�^
	float m_animBlendRate;		// ���݂Ɖߋ��̃A�j���[�V�����̍�������	(�t���[��)
									// 0.0f:prev���Đ������
									// 1.0f:current���Đ������

	float m_animSpeed;		// �A�j���[�V�����ύX���x

	// �v���C���[�X�e�C�g�|�C���^
	std::shared_ptr<PlayerState> m_pState;

	// �A�j���[�V�����f�[�^
	AnimationData m_animData;



	int m_multiAttack;		// �A���U���p�ϐ�
	bool m_isNextAttack;	// ���̍U�����s�����̃t���O
	AttackKind m_attackKind;// �ʏ�U���̒i�K

	State m_currentState;	// ���݂̃v���C���[�̏��

	VECTOR m_pos;			// �v���C���[�ʒu
	VECTOR m_move;			// �ړ���
	//VECTOR m_moveAttack;	// �U�����̉����l
	VECTOR m_targetDir;		// �v���C���[�������ׂ������̃x�N�g��
};