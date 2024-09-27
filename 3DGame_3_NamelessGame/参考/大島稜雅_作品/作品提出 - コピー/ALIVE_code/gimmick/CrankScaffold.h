#pragma once
#include "GimmickBase.h"
#include <memory>

class ManualCrank;

class CrankScaffold final : public GimmickBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="materialType">�}�e���A���̃^�C�v</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	CrankScaffold(const int handle, const Material materialType, const ObjectInfo& objInfo);
	
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CrankScaffold();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="player">�v���C���[�̎Q��</param>
	void Update(Player& player)override;
	
	/// <summary>
	/// �`��
	/// </summary>
	void Draw()override;;

	/// <summary>
	/// �T�E���h��炷
	/// </summary>
	void PlayStopCrankSound();

	/// <summary>
	/// �T�E���h��炷���Ƃ��o���邩�̏�������֐�
	/// </summary>
	bool CanSound();

	/// <summary>
	/// ���f���̈ړ�
	/// </summary>
	void MoveModel();

	////////////////Getter////////////////

	/// <summary>
	/// ����̏���Ɖ����̋����̃T�C�Y���擾����
	/// </summary>
	/// <param name="num">�ԍ�</param>
	/// <returns>�����T�C�Y</returns>
	float GetUpperAndLowerDistanceSize(int num);

	/// <summary>
	/// ����̏㏸�x�N�g�����擾����
	/// </summary>
	/// <param name="num">�ԍ�</param>
	/// <returns>�㏸�x�N�g��</returns>
	float GetUpVec(int num);
private:

	float oldRotZ_ = 0.0f;						//�Â�Z�̉�]
	float upVecY_ = 0.0f;						//Y���W�̏㏸�x�N�g��

	VECTOR initPos_ = {};						//�����|�W�V����

	std::shared_ptr<ManualCrank> crank_;		//�N�����N�N���X�̃X�}�[�g�|�C���^

};

