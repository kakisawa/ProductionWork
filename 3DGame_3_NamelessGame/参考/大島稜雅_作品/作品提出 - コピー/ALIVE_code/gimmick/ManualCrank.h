#pragma once
#include "../object/ObjectData.h"
#include <DxLib.h>
#include <memory>

class Model;
class InputState;
class Player;

class ManualCrank
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="info">�z�u�f�[�^</param>
	ManualCrank(const ObjectInfo& objInfo);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~ManualCrank();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �N�����N�̉����Đ�
	/// </summary>
	void PlayCrankSound();

	/// <summary>
	/// �T�E���h���Đ����邱�Ƃ��o���邩
	/// </summary>
	/// <returns></returns>
	bool CanPlaySound() const;

	/// <summary>
	/// �v���C���[�ƃ��f���̓����蔻����s��
	/// </summary>
	/// <param name="player">�v���C���[�̎Q��</param>
	/// <returns>�v���C���[�ƏՓ˂�����</returns>
	bool HitCollPlayer(Player& player) const;

	////////////////Getter////////////////

	/// <summary>
	/// Z���̍ő��]�����擾����
	/// </summary>
	/// <returns>Z���̍ő��]��</returns>
	float GetMaxRotZ() const;

	/// <summary>
	/// �w��̃I�u�W�F�N�g�̔z�u�f�[�^���擾����
	/// </summary>
	/// <param name="name">�擾�������I�u�W�F�N�g�̖��O</param>
	/// <param name="num">�ԍ�</param>
	/// <returns>�z�u�f�[�^</returns>
	ObjectInfo GetLoadObjectInfo(const std::string& name, int num);

	/// <summary>
	/// ���݂�Z���̉�]�����擾����
	/// </summary>
	/// <returns>Z���̉�]��</returns>
	float GetRotZ() const { return rotZ_; }

	/// <summary>
	/// ���f���|�C���^���擾
	/// </summary>
	/// <returns>���f���̃|�C���^</returns>
	const std::shared_ptr<Model>& GetModelPointer() const { return model_; }

	/// <summary>
	/// �N�����N���񂷍ۂ̗����ʒu�̎擾
	/// </summary>
	/// <returns>�|�W�V����</returns>
	const VECTOR& GetStandingPosition() const { return standingPos_; }


	////////////////Setter////////////////

	/// <summary>
	/// ��]����ݒ肷��
	/// </summary>
	/// <param name="rotZ">Z���̉�]��</param>
	void SetRotZ(float rotZ) { rotZ_ = rotZ; }

private:

	float rotZ_ = 0.0f;					//Z���̉�]��
	float radian_ = 0.0f;				//���W�A��
	float oldRotZ_ = 0.0f;				//�O�t���[����Z���̉�]��

	VECTOR pos_ = {};					//�|�W�V����
	VECTOR initPos_ = {};				//�����|�W�V����
	VECTOR standingPos_ = {};			//�N�����N���񂷗����ʒu

	std::shared_ptr<Model> model_;		//���f���N���X�̃X�}�[�g�|�C���^

};

