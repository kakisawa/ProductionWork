#pragma once
#include "GimmickBase.h"
#include <memory>
#include <vector>
#include <DxLib.h>
#include <unordered_map>

class Switch;
class Lever;
class Model;

class Elevator final : public GimmickBase
{
private:
	//�G���x�[�^�[�̃A�j���^�C�v
	enum class ElevatorAnimType
	{
		OpenIdle,
		CloseIdle,
		Open,
		Close,

		Max,
	};

	//�G���x�[�^�[�̏��
	enum class ElevatorState
	{
		Upper,
		Lower,
	};

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="materialType">�}�e���A���̃^�C�v</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	Elevator(const int handle, const Material materialType, const ObjectInfo& objInfo);
	
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Elevator();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="player">�v���C���[�̎Q��</param>
	void Update(Player& player)override;
	
	/// <summary>
	/// �`��
	/// </summary>
	void Draw()override;

	/// <summary>
	/// ����炷
	/// </summary>
	void PlayDoorSound();

	/// <summary>
	/// �o�����邱�Ƃ��\��
	/// </summary>
	/// <returns></returns>
	bool CanDeparture();
	

	////////////////Getter////////////////

	/// <summary>
	/// ����Ȗ��O�̃I�u�W�F�N�g�̔z�u�f�[�^���擾����
	/// </summary>
	/// <param name="name">���O</param>
	/// <param name="sign">�L��</param>
	/// <param name="groupNum">�O���[�v�ԍ�</param>
	/// <param name="num">�ԍ�</param>
	/// <returns>�z�u�f�[�^</returns>
	ObjectInfo GetSpecialNameObjectInfo(const std::string& name, const std::string& sign, int groupNum, int num);

	////////////////Setter////////////////

	/// <summary>
	/// �����̃G���x�[�^�[�̃X�e�[�^�X��ݒ肷��
	/// </summary>
	void SetInitState();

private:

	/// <summary>
	/// �G���x�[�^�[���ړ�������
	/// </summary>
	/// <returns>�㏸�x�N�g��</returns>
	float Move();

	/// <summary>
	/// �ړ���̃|�W�V�������擾����
	/// </summary>
	void TargetPosition();

private:

	float moveVecY_ = 0.0f;								//Y���̈ړ����F�N�g��
	float elapsedTime_ = 0.0f;							//�o�ߎ���

	bool isDeparture_ = false;							//�G���x�[�^�[���o�����Ă��邩�ǂ���
	bool isPlaySound_ = false;							//�T�E���h���Đ�����
	bool isStopping_ = false;							//�G���x�[�^�[�͒�~���Ă��邩

	VECTOR targetPos_ = {};								//���݌������Ă���|�W�V����

	ElevatorState state_ = {};							//�G���x�[�^�[�̏��

	std::unordered_map<ElevatorState, VECTOR> stopPos_;	//�X�g�b�v�|�W�V����

	std::shared_ptr<Lever> movingLever_;				//���o�[�N���X�̃X�}�[�g�|�C���^

	std::vector<std::shared_ptr<Lever>> levers_;		//���o�[�N���X�̃X�}�[�g�|�C���^��Vector�z��
};

