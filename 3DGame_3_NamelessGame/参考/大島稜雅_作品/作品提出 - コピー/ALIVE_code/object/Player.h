#pragma once
#include "PlayerData.h"
#include "ObjectData.h"
#include <DxLib.h>
#include <memory>

class Model;
class ObjectManager;
class ObjectBase;
class ManualCrank;
class Lever;

class Player
{
private:
	//�v�@6byte
	struct JumpInfo {
		float jumpVec = 0.0f;					//�W�����v�x�N�g��		//4byte
		bool isJump = false;					//�W�����v�����ǂ���	//2byte
	};

	//�v�@10byte
	struct Situation {
		bool isMoving = false;					//�ړ�����					//2byte			
		bool isClim = false;					//�o��邩					//2byte
		bool isInTransit = false;				//�^������					//2byte
		bool isCanBeCarried = false;			//�����^�Ԃ��Ƃ��o���邩	//2byte
		bool isGimmickCanBeOperated = false;	//�M�~�b�N�𑀍쒆��		//2byte
	};

	//�v�@62byte
	struct PlayerStatus {
		int animNo;								//�A�j���[�V�����ԍ�				//4byte
		float height;							//����								//4byte
		VECTOR pos;								//�|�W�V����						//12byte
		VECTOR rot;								//��]								//12byte
		VECTOR moveVec;							//�ړ��x�N�g��						//12byte
		JumpInfo jump;							//�W�����v							//6byte
		Situation situation;					//�V�`���G�[�V����					//10byte
		bool isAnimLoop;						//�A�j���[�V�����̃��[�v���K�v��	//2byte
	};

	//��]�̃X�e�[�g
	enum class RotationState
	{
		Up,
		UpperRight,
		Right,
		LowerRight,
		Down,
		LowerLeft,
		Left,
		UpperLeft,

		Max,
	};

	//��]�̃f�[�^
	struct RotationData
	{
		float targetAngle_ = 0.0f;			//�ڕW�̊p�x
		RotationState rotState_ = {};		//��]�̏��
	};

	//��]�̃f�[�^�z��
	RotationData rotData_[static_cast<int>(RotationState::Max)]
	{
		//Up
		0.0f,						//�ڕW�̊p�x
		RotationState::Up,			//��]�̏��

		//UpperRight
		45.0f,						//�ڕW�̊p�x
		RotationState::UpperRight,	//��]�̏��

		//Right
		90.0f,						//�ڕW�̊p�x
		RotationState::Right,		//��]�̏��

		//LowerRight
		135.0f,						//�ڕW�̊p�x
		RotationState::LowerRight,	//��]�̏��

		//Down
		180.0f,						//�ڕW�̊p�x
		RotationState::Down,		//��]�̏��

		//LowerLeft
		225.0f,						//�ڕW�̊p�x
		RotationState::LowerLeft,	//��]�̏��

		//Left
		270.0f,						//�ڕW�̊p�x
		RotationState::Left,		//��]�̏��

		//UpperLeft
		315.0f,						//�ڕW�̊p�x
		RotationState::UpperLeft,	//��]�̏��
	};
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="info">�z�u�f�[�^</param>
	Player(const ObjectInfo& info);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Player();

	/// <summary>
	/// �v���C���[�̍X�V���s��
	/// </summary>
	void Update();

	/// <summary>
	/// �v���C���[�֘A�̕`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���݂̊p�x����߂��p�x�ɉ��
	/// </summary>
	/// <param name="differenceAngle">�p�x�̈Ⴂ</param>
	/// <param name="targetAngle">�ڕW�̊p�x</param>
	/// <param name="currentAngle">���݂̊p�x</param>
	void RotateAtACloseAngle(float& differenceAngle, float targetAngle, float currentAngle);

	/// <summary>
	/// �e�ɓ���������m�b�N�o�b�N��ǉ�����
	/// </summary>
	void BulletHitMe(const VECTOR& moveVec);

	/// <summary>
	/// �ǂ�ȉ�]��Ԃ��擾����
	/// </summary>
	/// <returns>��]��</returns>
	RotationState WhatRotationState();

	/// <summary>
	/// �ڕW�̊p�x�ɉ�]���邱�Ƃ��o���邩
	/// </summary>
	/// <param name="rotZ">���݂̊p�x</param>
	/// <returns>true:�o����  false:�o���Ȃ�</returns>
	bool CanRotation(float rotZ);

	///////Getter///////

	/// <summary>
	/// ���񂾉񐔂��擾����
	/// </summary>
	int GetDeathCount()const { return deathCount_; }

	/// <summary>
	/// �v���C���[�̃X�e�[�^�X���擾����
	/// </summary>
	const PlayerStatus& GetStatus() const { return status_; }

	/// <summary>
	/// �����^�Ԏ��̂̃|�C���^�[���擾����
	/// </summary>
	const std::shared_ptr<ObjectBase>& GetDeadPersonModelPointer() const { return corpseModelPointer_; }

	/// <summary>
	/// ���f���|�C���^�[���擾����
	/// </summary>
	/// <param name="height"></param>
	/// <param name="materialType"></param>
	const std::shared_ptr<Model>& GetModelPointer() const { return model_; }

	/// <summary>
	/// �e��`�悷�鍂�����擾����
	/// </summary>
	/// <returns>�e��`�悷�鍂��</returns>
	float GetRoundShadowHeight() const { return roundShadowHeight_; }

	/// <summary>
	/// ���̂𐶐����邱�Ƃ��o���邩
	/// </summary>
	/// <returns>true:�����ł���@false:�o���Ȃ�</returns>
	bool GetIsCorpseGeneratable() { return isCorpseGeneratable_; }

	/// <summary>
	/// ���̂̔z�u�����擾����
	/// </summary>
	/// <returns>���̂̔z�u�f�[�^</returns>
	const ObjectInfo& GetCorpseInfo() const { return corpseInfo_; }

	///////Setter///////

	/// <summary>
	/// �O������̃|�W�V�������󂯎��
	/// </summary>
	/// <param name="pos">�|�W�V�������</param>
	void SetPos(const VECTOR& pos);

	/// <summary>
	/// �O������̃W�����v�����󂯎��
	/// </summary>
	/// <param name="isJump">�W�����v�t���O</param>
	/// <param name="jumpVec">�W�����v�x�N�g��</param>
	void SetJumpInfo(const bool isJump, const  float jumpVec);

	/// <summary>
	/// �x�N�g����ݒ肷��
	/// </summary>
	/// <param name="vector">�x�N�g��</param>
	void SetMoveVec(VECTOR vector) { status_.moveVec = vector; }

	/// <summary>
	/// �����^�Ԏ����o����t���O�Ǝ����^�ԃ��f���̃|�C���^���󂯎��
	/// </summary>
	void SetCarryInfo(const bool isCarry,const std::shared_ptr<ObjectBase>& model);

	/// <summary>
	/// ManualCrank�̃|�C���^��ݒ肷��
	/// </summary>
	/// <param name="crank"></param>
	void SetCrankPointer(const std::shared_ptr<ManualCrank>& crank);

	/// <summary>
	/// ���o�[�̃|�C���^��ݒ肷��
	/// </summary>
	void SetLeverPointer(const std::shared_ptr<Lever>& lever);

	/// <summary>
	/// �n�ʂɕ`�悷��e�̍����Ɠ���ł���I�u�W�F�N�g�̑f�ނ�ݒ肷��
	/// </summary>
	/// <param name="height">����</param>
	/// <param name="materialType">������ł���I�u�W�F�N�g�����o�Ă��Ă��邩</param>
	void SetRoundShadowHeightAndMaterial(const float height, const  Material materialType);

	/// <summary>
	/// ���̂𐶐����邱�Ƃ��o���邩�̃t���O��ݒ肷��
	/// </summary>
	/// <param name="isCorpseGeneratable">true�F�o����@false�F�o���Ȃ�</param>
	void SetIsCorpseGeneratable(const bool isCorpseGeneratable) { isCorpseGeneratable_ = isCorpseGeneratable; }
private:

	/////////////�v���C���[�̋����Ɋ֌W����X�V/////////////

	/// <summary>
	/// �X�V
	/// </summary>
	void NormalUpdate();

	/// <summary>
	/// �ړ��̍X�V
	/// </summary>
	void MovingUpdate();

	/// <summary>
	/// �ړ�
	/// </summary>
	float Move();

	/// <summary>
	/// ��]����
	/// </summary>
	void RotationUpdate();

	/// <summary>
	/// �W�����v
	/// </summary>
	void JumpUpdate();

	/// <summary>
	/// �v���C���[�̎��̂ɗ^����������֐�
	/// </summary>
	void DeathUpdate();

	/// <summary>
	/// ���̂̌㏈��
	/// </summary>
	void CorpsePostProsessing();

	/// <summary>
	/// �v���C���[�̎��̂�vector�z��ŊǗ�����֐�
	/// </summary>
	void CorpseInfoGenerater();

	/// <summary>
	/// �ו����^��
	/// </summary>
	void CarryObject();

	/// <summary>
	/// �ו������낷
	/// </summary>
	void DropOffObject();

	/// <summary>
	/// �N�����N���񂷃|�W�V�����܂ōs��
	/// </summary>
	void GoCrankRotationPosition();

	/// <summary>
	/// �N�����N����]������A�b�v�f�[�g
	/// </summary>
	/// <param name="rotZ">Z�̉�]��</param>
	void CrankRotationUpdate(float rotZ);

	/// <summary>
	/// �N�����N�̍X�V
	/// </summary>
	void CrankUpdate();

	/// <summary>
	/// ���o�[��|���|�W�V�����֍s��
	/// </summary>
	void GoLeverPullPosition();

	/// <summary>
	/// ���o�[�̍X�V
	/// </summary>
	void LeverUpdate();

	/// <summary>
	/// �������Ƃ̏Փ˃A�b�v�f�[�g
	/// </summary>
	void BulletHitMeUpdate();

private:

	/////////////�v���C���[�̃X�e�[�^�X���Ɋ֌W����X�V/////////////

	/// <summary>
	/// �A�j���[�V������idle�ɕύX����
	/// </summary>
	void ChangeAnimIdle();

	/// <summary>
	/// �A�j���[�V�����̕ύX���s��
	/// </summary>
	/// <param name="type">�A�j���[�V�����̃^�C�v</param>
	void ChangeAnimNo(const PlayerAnimType type, const  bool isAnimLoop, const  int changeTime);

	/// <summary>
	/// �v���C���[�̈ړ����x��ݒ肷��
	/// </summary>
	/// <returns>float�^�̈ړ����x</returns>
	float PlayerSpeed(const bool pressedShift);

	/// <summary>
	/// ���ӂ݂̉�
	/// </summary>
	void FootStepSound();

	/// <summary>
	/// �����e�̕`��
	/// </summary>
	void DrawPolygon3D();

	/// <summary>
	/// �����e�̒��_�̃|�W�V�������擾����
	/// </summary>
	/// <param name="angle">�p�x</param>
	/// <returns>�|�W�V����</returns>
	VECTOR VertexPosition(const float angle);

private:

	int deathCount_ = 0;										//���񂾉񐔂��L�^����
	int crankLaps_ = 0;											//�N�����N�̎���

	float targetAngle_ = 0.0f;									//��]
	float differenceAngle_ = 0.0f;								//�ڕW�̊p�x�ƌ��݂̊p�x�̍�
	float angle_ = 0.0f;
	float roundShadowHeight_ = 0.0f;
	float crankTargetAngle_ = 0.0f;								//�N�����N���ڎw���p�x

	bool debugCreativeMode_ = false;							//�f�o�b�O�p�ϐ�
	bool isCorpseGeneratable_ = false;							//���̂𐶐����邱�Ƃ��o���邩

	VECTOR scale_ = {0.0f,0.0f, 0.0f};							//�g�k��

	PlayerInfo playerInfo_ = {};								//�v���C���[�̏��
	PlayerStatus status_ = {};									//�v���C���[�̃X�e�[�^�X
	ObjectInfo corpseInfo_ = {};								//���̂̔z�u���

	Material materialSteppedOn_ = Material::Stone;				//�����p�ϐ�
	RotationState currentRotState_ = RotationState::Up;			//��]�X�e�[�^�X

	std::shared_ptr<Model> model_;								//���f���N���X�̃|�C���^
	std::shared_ptr<ManualCrank> crank_;						//�N�����N�N���X�̃|�C���^
	std::shared_ptr<Lever> lever_;								//�N�����N�N���X�̃|�C���^
	std::shared_ptr<ObjectBase> corpseModelPointer_;			//�����^�Ԏ��̂̃��f���|�C���^

	void(Player::* updateFunc_)();		//�����o�֐��|�C���^
	void(Player::* carryUpdateFunc_)();		//�����o�֐��|�C���^
};

