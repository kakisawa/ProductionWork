#pragma once
#include "../object/ObjectData.h"
#include <memory>
#include <DxLib.h>

class Model;

class Lever
{
private:
	//�A�j���[�V�����^�C�v
	enum class AnimType {
		on,
		off,
		max,
	};

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="info">�z�u�f�[�^</param>
	Lever(const ObjectInfo& info);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Lever();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �Փ˔���
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	/// <returns>true : �������Ă���  false : �������Ă��Ȃ�</returns>
	bool CollCheck(const VECTOR& playerPos) const;

	/// <summary>
	/// ���o�[�̋N��
	/// </summary>
	void OnAnimation();

	/// <summary>
	/// ���o�[�̒�~
	/// </summary>
	void OffAnimation();

	/// <summary>
	/// ���o�[���N�����Ă��邩�ǂ����擾
	/// </summary>
	/// <returns>���o�[���N�����Ă��邩</returns>
	bool IsOn() const { return isOn_; }


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

	/// <summary>
	/// ���f���|�C���^�̎擾
	/// </summary>
	/// <returns>���f���̃|�C���^</returns>
	const std::shared_ptr<Model>& GetModelPointer() const { return model_; }

	/// <summary>
	/// ���o�[�������|�W�V�����̎擾
	/// </summary>
	/// <returns>���o�[�������|�W�V����</returns>
	const VECTOR& GetStandingPosition() const { return standingPos_; }


	////////////////Setter////////////////

	/// <summary>
	/// ���o�[�������|�W�V������ݒ肷��
	/// </summary>
	/// <param name="standingPos">���o�[�������ʒu</param>
	void SetStandingPos(const VECTOR& standingPos) { standingPos_ = standingPos; }

private:

	bool isOn_ = false;							//���o�[���N�����Ă��邩�ǂ���

	VECTOR pos_ = {};							//�|�W�V����
	VECTOR standingPos_ = {};					//�G���x�[�^�̒�~�|�W�V����

	std::shared_ptr<Model> model_;				//���f���N���X�̃X�}�[�g�|�C���^

};

