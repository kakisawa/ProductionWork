#pragma once
#include "Model.h"
#include "DxLib.h"
#include <memory>

class Camera;
class PlayerState;
class GameMap;
class Player
{
public:
	Player();
	~Player();

	void Init(std::shared_ptr<GameMap> pMap);
	void Update(const Camera& camera);
	void Draw();
	void End();


	int GetAddDamage() const { return m_addDamage; }
	bool GetDeathFlag() const { return m_isDeath; }
	VECTOR GetPos()const { return m_pos; }	// ���W�n��

private:

	// �U���̎��
	enum class AttackKind
	{
		kNone = -1,
		kAttackSord = 0,
		kAttackBow = 1
	};

	// �A�j���[�V�������
	struct AnimationData
	{
		int8_t kIdle = 1;		//�ҋ@���[�V����
		int8_t kWalk = 3;		//�������[�V����
		int8_t kJump = 12;		//�W�����v�����[�V����
		int8_t kAttack1 = 30;	//�U�����[�V����1
		int8_t kAttack2 = 31;	//�U�����[�V����2
		int8_t kAttack3 = 32;	//�U�����[�V����3
		int8_t kAttack4 = 33;	//�U�����[�V����4
		int8_t kAttackBow = 54;	//�U�����[�V����(�e)
	};

	// �e��Ԃ��Ƃ̏�����
	void IdleStateInit() {};		//�ҋ@��Ԃ̏�����
	void WalkStateInit() {};		//������Ԃ̏�����

	void JumpStateInit();			//�W�����v��Ԃ̏�����
	void AttackSordStateInit();		//���U����Ԃ̏�����
	void AttackBowStateInit();		// �|�U����Ԃ̏�����

	// �e��Ԃ��Ƃ̍X�V
	void IdleStateUpdate();
	void WalkStateUpdate();
	void JumpStateUpdate();
	void AttackSordStateUpdate();
	void AttackBowStateUpdate();

	// �v���C���[�̈ړ��l�ݒ�
	void OldMoveValue(const Camera& camera, VECTOR& upMoveVec, VECTOR& leftMoveVec);
	void Move(const VECTOR& MoveVector);	// �v���C���[�̈ړ�����
	void Angle();							// �v���C���[�̉�]����

	void Attack();		// �U������
	void Jump();		// �W�����v����

	void Death();		// ���S����

private:
	int m_hp;					// �v���C���[HP
	int m_addDamage;			// �v���C���[���G�ɗ^����_���[�W��
	int m_uiGraph;				// �v���C���[�pUI�摜

	float m_angle;				// �v���C���[�����p�x
	float m_jumpPower;			// �x�������̑��x
	float m_gravity;			// �v���C���[�ɂ�����d��

	bool m_isWalk;			// �ړ����t���O
	bool m_isAttack;		// �U�����t���O
	bool m_isNextAttackFlag;	// ���̍U�������s����邩�̃t���O
	bool m_isFirstAttack;	// �ŏ��̍U�����t���O
	bool m_isAttackDamage;	// �_���[�W���󂯂�HP���ϓ����邩�̃t���O
	bool m_isJump;			// �W�����v���t���O
	bool m_isDeath;			// ���S�������t���O

	struct MapPlace
	{
		VECTOR leftBack;		// �}�b�v�������W
		VECTOR rightFront;	// �}�b�v�E�O���W
	}mp;

	// �v���C���[�X�e�C�g�|�C���^
	std::shared_ptr<PlayerState> m_pState;

	// �v���C���[�A�j���[�V�����f�[�^
	AnimationData m_animData;

	// �U����ރf�[�^
	AttackKind m_attackKind;

	int m_multiAttack;		// �A���U���p�ϐ�
	bool m_isNextAttack;	// ���̍U�����s�����̃t���O

	VECTOR m_pos = m_pModel->GetPos();			// �v���C���[�ʒu
	VECTOR m_move;			// �ړ���
	VECTOR m_targetDir;		// �v���C���[�������ׂ������̃x�N�g��
	VECTOR m_UpPos;			// �J�v�Z������W

	//���f���N���X
	std::shared_ptr<Model> m_pModel;
};