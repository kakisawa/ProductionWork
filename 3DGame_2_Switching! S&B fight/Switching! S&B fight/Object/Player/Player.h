#pragma once
#include "DxLib.h"
#include "../../Util/Collision.h"
#include <memory>
#include <array>

class Model;
class GameMap;
class Camera;
class PlayerState;
class EnemyRight;
class EnemyLeft;
class SoundManager;
class Effect;
class Player
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Player();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Player();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="pMap">�}�b�v</param>
	void Init(std::shared_ptr<GameMap> pMap);

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="camera">�J����</param>
	/// <param name="enemyR">�E�̓G</param>
	/// <param name="enemyL">���̓G</param>
	void Update(const Camera& camera,const EnemyRight& enemyR, const EnemyLeft& enemyL);
	
	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

	/// <summary>
	/// �I������
	/// </summary>
	void End();

	/// <summary>
	/// �����̋߂��G��T��
	/// </summary>
	/// <param name="enemyR">�E�̓G</param>
	/// <param name="enemyL">���̓G</param>
	void NearByEnemy(const EnemyRight& enemyR, const EnemyLeft& enemyL);

	/// <summary>
	/// �ړ��l�ݒ�
	/// </summary>
	/// <param name="camera">�J����</param>
	/// <param name="upMoveVec"></param>
	/// <param name="leftMoveVec"></param>
	void OldMoveValue(const Camera& camera, VECTOR& upMoveVec, VECTOR& leftMoveVec);

	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="MoveVector">�ړ��x�N�g��</param>
	/// <param name="enemyR">�E�̓G</param>
	/// <param name="enemyL">���̓G</param>
	void Move(const VECTOR& MoveVector, const EnemyRight& enemyR, const EnemyLeft& enemyL);

	/// <summary>
	/// ��]����
	/// </summary>
	void Angle();

	/// <summary>
	/// �U������
	/// </summary>
	/// <param name="enemyR">�E�̓G</param>
	/// <param name="enemyL">���̓G</param>
	void Attack(const EnemyRight& enemyR, const EnemyLeft& enemyL);

	/// <summary>
	/// �W�����v����
	/// </summary>
	void Jump();

	/// <summary>
	/// ���S����
	/// </summary>
	void Death();

	void ColUpdate();				// �����蔻��̍X�V����

	// �e��Ԃ��Ƃ̏�����
	void IdleStateInit() {};		//�ҋ@��Ԃ̏�����
	void WalkStateInit() {};		//������Ԃ̏�����
	void JumpStateInit();			//�W�����v��Ԃ̏�����
	void AttackSordStateInit();		//���U����Ԃ̏�����
	void AttackBowStateInit();		//�|�U����Ԃ̏�����

	// �e��Ԃ��Ƃ̍X�V
	void IdleStateUpdate();			// �ҋ@��Ԃ̍X�V
	void WalkStateUpdate();			// ������Ԃ̍X�V
	void JumpStateUpdate();			// �W�����v��Ԃ̍X�V
	void AttackSordStateUpdate();	// ���U����Ԃ̍X�V
	void AttackBowStateUpdate();	// �|�U����Ԃ̍X�V

	int GetAddDamage() const { return m_addDamage; }					// �G�ɗ^����_���[�W�ʂ�n��
	bool GetAttackRight()const { return m_isSordAttackToRightEnemy; }	// �E�̓G���U�����邩�̃t���O��n��
	bool GetAttackLeft()const { return m_isSordAttackToLeftEnemy; }		// ���̓G���U�����邩�̃t���O��n��
	bool GetDeathFlag() const { return m_isDeath; }						// �v���C���[�����S���Ă��邩�̃t���O��n��

	VECTOR GetPos()const { return m_pos; }					// �v���C���[�̍��W�n��
	Collision GetCol() const { return m_colSphere; }		// �v���C���[�̓����蔻��n��

private:

	// �U���̎��
	enum class AttackKind{
		kNone = -1,		// ����
		kAttackSord = 0,// ���U��
		kAttackBow = 1	// �|�U��
	};

	// �A�j���[�V�������
	struct AnimationData{
		int8_t kIdle = 1;		//�ҋ@���[�V����
		int8_t kWalk = 3;		//�������[�V����
		int8_t kJump = 12;		//�W�����v�����[�V����
		int8_t kAttack1 = 30;	//�U�����[�V����1
		int8_t kAttack2 = 31;	//�U�����[�V����2
		int8_t kAttack3 = 32;	//�U�����[�V����3
		int8_t kAttack4 = 33;	//�U�����[�V����4
		int8_t kAttackBow = 54;	//�U�����[�V����(�e)
	};

	struct MapPlace{
		VECTOR leftBack;		// �}�b�v�������W
		VECTOR rightFront;		// �}�b�v�E�O���W
	}mp;

private:
	int m_hp;						// �v���C���[HP
	int m_addDamage;				// �v���C���[���G�ɗ^����_���[�W��
	int m_multiAttack;				// �A���U���p�ϐ�
	std::array<int, 5> m_uiGraph{};	// UI�p�摜

	float m_angle;					// �v���C���[�����p�x
	float m_jumpPower;				// �x�������̑��x
	float m_gravity;				// �v���C���[�ɂ�����d��

	bool m_isWalk;					// �ړ����t���O
	bool m_isJump;					// �W�����v���t���O
	bool m_isDeath;					// ���S�������t���O
	bool m_isAttack;				// �U�����t���O
	bool m_isFirstAttack;			// �ŏ��̍U�����t���O
	bool m_isNextAttackFlag;		// ���̍U�������s����邩�̃t���O
	bool m_isSordAttackDamage;		// ���U�����󂯂�HP���ϓ����邩�̃t���O
	bool m_isBowAttackDamage;		// ���U�����󂯂�HP���ϓ����邩�̃t���O
	bool m_isSordAttackToRightEnemy;// ���ŉE�̓G���U�����Ă���t���O
	bool m_isSordAttackToLeftEnemy;	// ���ō��̓G���U�����Ă���t���O
	bool m_isBowAttackToLeftEnemy;	// �|�ō��̓G���U�����Ă���t���O
	bool m_isCol;					// �G�Ƃ̍U�������蔻��t���O
	bool m_isBodyCol;				// �G�̑̂ƃv���C���[�̑̂Ƃ̓����蔻��

	VECTOR m_pos;			// ���W
	VECTOR m_move;			// �ړ���
	VECTOR m_targetDir;		// �v���C���[�������ׂ������̃x�N�g��
	VECTOR m_vecToEnemy;	// �v���C���[���̋���
	VECTOR m_UpPos;			// �J�v�Z������W
	VECTOR m_attackRange;	// �U���͈�

	// �v���C���[�A�j���[�V�����f�[�^
	AnimationData m_animData;
	// �U����ރf�[�^
	AttackKind m_attackKind;
	// �����蔻��
	Collision m_colSphere;

	// �v���C���[�X�e�C�g
	std::shared_ptr<PlayerState> m_pState;
	// �G�t�F�N�g
	std::shared_ptr<Effect> m_pEffect;
	//���f���N���X
	std::shared_ptr<Model> m_pModel;
	// �T�E���h�}�l�[�W���[�N���X
	SoundManager* m_pSound;
};