#pragma once
#include <memory>
#include <vector>

class LoadData;
class UIBattle;
class EffectManager;

/// <summary>
/// �L�����N�^�[�̊��N���X
/// </summary>
class CharacterBase
{
protected:
	// �����蔻��ʒu
	struct Collision
	{
		VECTOR armStartPos;		// �r�̓����蔻��n�_
		VECTOR armEndPos;		// �r�̓����蔻��I�_
		VECTOR legStartPos;		// �r�̓����蔻��n�_
		VECTOR legEndPos;		// �r�̓����蔻��I�_
		VECTOR bodyTopPos;		// �S�g�̓����蔻��n�_
		VECTOR bodyBottomPos;	// �S�g�̓����蔻��I�_
	};
	Collision m_col;

public:
	CharacterBase();
	~CharacterBase() {};
	virtual void Init(std::shared_ptr<EffectManager> pEffect, VECTOR pos) = 0;
	virtual void Draw() = 0;
	virtual void OnDamage(float damage);		// �_���[�W���󂯂��ۂ̏���

	void SetIsStartProduction(bool isStartProduction) { m_isStartProduction = isStartProduction; }				// �X�^�[�g���o��Ԃ��Z�b�g����
	void SetIsClearProduction(bool isClearProduction) { m_isClearProduction = isClearProduction; }				// �N���A���o��Ԃ��Z�b�g����
	void SetIsGameoverProduction(bool isGameoverProduction) { m_isGameoverProduction = isGameoverProduction; }  // �Q�[���I�[�o�[���o��Ԃ��Z�b�g����

	VECTOR GetPos() const { return m_pos; }			// ���ݒn�擾
	float GetHp() const { return m_hp; }			// ���݂�HP���擾
	bool GetIsGuard() const { return m_isGuard; }	// �K�[�h�����ǂ����擾
	Collision GetCol() const { return m_col; }		// �����蔻��ʒu���擾

	// �L�����N�^�[�̎��
	enum class CharaType
	{
		kPlayer,		// �v���C���[
		kEnemyTuto,		// �`���[�g���A���̓G
		kEnemyNinja,	// �E��
		kEnemyChef,		// �V�F�t
		kEnemyAbe,		// Abe
		kCharaNum,		// �L������
	};

	// �L�����N�^�[�̏��
	enum class State
	{
		kAvoid = 0,			 // ���
		kDown = 1,			 // �|���
		kFightIdle = 2,		 // �\��(��~��)
		kFightWalk = 3,		 // �\��(�ړ���)
		kGettingUp = 4,		 // �N���オ��
		kGrab = 5,			 // �͂�
		kGuard = 6,			 // �K�[�h
		kKick = 7,			 // �L�b�N
		kPunch1 = 8,		 // �p���`(1�R���{)
		kPunch2 = 9,		 // �p���`(2�R���{)
		kPunch3 = 10,		 // �p���`(3�R���{)
		kReceive = 11,		 // �U�����󂯂�
		kRun = 12,			 // �ړ�
		kSpecialAttack = 13, // �K�E�Z 
		kStand = 14,		 // �ҋ@
		kStumble = 15,		 // �͂ݎ��s
	};

	// �A�j���[�V�������
	enum class AnimKind
	{
		kNone = -1,			 // �Ȃ�
		kAvoid = 0,			 // ���
		kDown = 1,			 // �|���
		kFightIdle = 2,		 // �\��(��~��)
		kFightWalk = 3,		 // �\��(�ړ���)
		kGettingUp = 4,		 // �N���オ��
		kGrab = 5,			 // �͂�
		kGuard = 6,			 // �K�[�h
		kKick = 7,			 // �L�b�N
		kPunch1 = 8,		 // �p���`(1�R���{)
		kPunch2 = 9,		 // �p���`(2�R���{)
		kPunch3 = 10,		 // �p���`(3�R���{)
		kReceive = 11,	  	 // �U�����󂯂�
		kRun = 12,			 // �ړ�
		kSpecialAttack = 13, // �K�E�Z 
		kStand = 14,		 // �ҋ@
		kStumble = 15,		 // �͂ݎ��s
	};

	// �A�j���[�V�������x
	struct AnimSpeed
	{
		float none;			// �Ȃ�
		float avoid;		// ���
		float down;			// �|���
		float fightIdle;	// �\��(��~��)
		float fightWalk;	// �\��(�ړ���)
		float gettingUp;	// �N���オ��
		float grab;			// �͂�
		float guard;		// �K�[�h
		float kick;			// �L�b�N
		float punch1;		// �p���`(1�R���{)
		float punch2;		// �p���`(2�R���{)
		float punch3;		// �p���`(3�R���{)
		float specialAttack;// �K�E�Z
		float receive;		// �U�����󂯂�
		float run;			// �ړ�
		float stand;		// �ҋ@
		float Stumble;		// �͂ݎ��s
	};
	AnimSpeed m_animSpeed;

	// �L�����N�^�[�̃X�e�[�^�X
	struct Status
	{
		float maxHp;				// �ő�HP
		int punchReceptionTime;		// �p���`�R���{�̓��͎�t����
		int punchCoolTime;			// �p���`�̃N�[���^�C��
		int kickCoolTime;			// �L�b�N�̃N�[���^�C��
		int punchTime;				// �p���`�����̎���
		int kickTime;				// �L�b�N�����̎���
		float punchPower;			// �p���`�̍U����
		float secondPunchPower;		// �p���`2�R���{�ڂ̍U����
		float thirdPunchPower;		// �p���`3�R���{�ڂ̍U����
		float kickPower;			// �L�b�N�̍U����
		float specialAttackPower;	// �K�E�Z�̍U����
		float maxMoveSpeed;			// �ő�̈ړ����x
		float fightWalkSpeed;		// �\�����̈ړ����x
		float acceleration;			// �����x
		float deceleration;			// �����x
		float avoidDist;			// ����̋���
		int maxAvoidCount;			// �A���ŉ���ł����
		int avoidCoolTime;			// ����ł���悤�ɂȂ�܂ł̎���
		float guardAnimTime;		// �K�[�h�̃A�j���[�V�������~�߂鎞��
		float backMove;				// �K�[�h���ɍU�����󂯂��ۂɈړ����鋗��
	};
	Status m_status;

	// �����̓����蔻����
	struct CollisionInfo
	{
		float bodyHeight;		// �S�g�̓����蔻��̍���
		float bodyRadius;		// �S�g�̓����蔻��̔��a
		float aimRadius;		// �r�̓����蔻��̔��a
		float legRadius;		// �r�̓����蔻��̔��a
		VECTOR armStartPos;		// �r�̓����蔻��n�_
		VECTOR armEndPos;		// �r�̓����蔻��I�_
		VECTOR legStartPos;		// �r�̓����蔻��n�_
		VECTOR legEndPos;		// �r�̓����蔻��I�_
		VECTOR bodyTopPos;		// �S�g�̓����蔻��n�_
		VECTOR bodyBottomPos;	// �S�g�̓����蔻��I�_
	};
	CollisionInfo m_colInfo;

	// �G�̏��
	struct EnemyInfo
	{
		float approachRange;	// �v���C���[�ɋ߂Â��͈�
		float attackRange;		// �v���C���[���U������͈�
		int minStopTime;		// �ŏ��̒�~����
		int maxStopTime;		// �ő�̒�~����
		int maxProb;			// �ő�m��%
		int punchProb;			// �p���`�m��
		int kickProb;			// �L�b�N�m��
		int avoidProb;			// ����m��
		int guardProb;			// �K�[�h�m��
		int grabProb;			// �͂݊m��
		int changeAngleProb;	// �p�x���X�V����m��
		int changeAngleFrame;	// �p�x���X�V����t���[����
		int guardTime;			// �K�[�h����
		int stateIntervalTime;	// ��Ԃ��X�V����܂ł̎���
	};
	EnemyInfo m_enemyInfo;

protected:
	// �����蔻��ʒu�̍X�V
	void UpdateCol();
	// �A�j���[�V�����X�e�[�g�̍X�V
	void UpdateAnimState(CharacterBase::State prevState);
	// �A�j���[�V�������Đ�����
	void PlayAnim(AnimKind playAnimIndex);
	// �A�j���[�V��������
	void UpdateAnim();
	// �ʒu���O���X�V����
	void UpdatePosLog();
	// ����̏ꍇ�c����\������
	void DrawAfterImage();
	
protected:
	std::shared_ptr<LoadData> m_pLoadData;			// �L�����N�^�[���
	std::shared_ptr<UIBattle> m_pUIBattle;			// �o�g�����ɕ\������UI
	std::shared_ptr<EffectManager> m_pEffect;		// �G�t�F�N�g

	float m_hp;					  // HP
	VECTOR m_pos;				  // �ʒu
	std::vector<VECTOR> m_posLog; // �ߋ��̈ʒu��ۑ�����
	float m_moveSpeed;			  // �ړ����x
	float m_angle;				  // �����Ă�������̊p�x
	int m_punchCount;			  // ���݂̃p���`�̃R���{��
	int m_punchComboTime;		  // �R���{���͎�t����
	int m_punchCoolTime;		  // �p���`�ł��Ȃ�����
	int m_kickCoolTime;			  // �L�b�N�ł��Ȃ�����
	int m_attackTime;			  // �U������
	int m_avoidCoolTime;		  // ����ł��Ȃ�����
	int m_avoidCount;			  // ���������
	bool m_isAttack;			  // �U�������ǂ���(true:�U����)
	bool m_isGuard;				  // �K�[�h�����ǂ���(ture:�K�[�h��)
	bool m_isMove;				  // �ړ��������ǂ���(true:�ړ�����)
	bool m_isFighting;			  // �\�������ǂ���(true:�\����)
	bool m_isReceive;			  // �U�����󂯂Ă���Œ����ǂ���(true:�U�����󂯂Ă���)
	bool m_isSpecialAttack;		  // �K�E�Z�𔭓�����(true:������)
	bool m_isStartProduction;	  // �X�^�[�g���o����(true:���o��)
	bool m_isClearProduction;	  // �N���A���o����(true:���o��)
	bool m_isGameoverProduction;  // �Q�[���I�[�o�[���o����(true:���o��)
	State m_currentState;		  // ���݂̏��
	int m_modelHandle;			  // �L�����N�^�[��3D���f��

	// �A�j���[�V�������
	int m_currentPlayAnim;		  // �Đ����̃A�j���[�V�����̃A�^�b�`�ԍ�
	float m_currentAnimCount;	  // �Đ����̃A�j���[�V�����̍Đ�����
	int m_prevPlayAnim;			  // �O�̍Đ��A�j���[�V�����̃A�^�b�`�ԍ�
	float m_prevAnimCount;		  // �O�̍Đ��A�j���[�V�����̍Đ�����
	float m_animBlendRate;		  // ���݂Ɖߋ��̃A�j���[�V�����̃u�����h��
};