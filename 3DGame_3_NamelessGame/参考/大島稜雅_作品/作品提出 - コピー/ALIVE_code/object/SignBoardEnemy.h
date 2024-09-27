#pragma once
#include "EnemyBase.h"
#include <DxLib.h>

class SignBoardEnemy final : public EnemyBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="materialType">�}�e���A���̃^�C�v</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	SignBoardEnemy(const int handle, const Material materialType, const ObjectInfo& objInfo);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SignBoardEnemy();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="player">�v���C���[�̎Q��</param>
	void Update(Player& player)override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �����̕ϊ��s����s���A�t���[���ԍ���Ԃ�
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="displayFrameName">�\���������ꏊ�̃t���[����</param>
	/// <param name="number">�����̕�����</param>
	/// <returns>�t���[���ԍ�</returns>
	int NumericTransformationMatrix(int handle,const std::string& displayFrameName, const std::string& number);
private:
	int numModel_[2] = {};							//���̃��f��
	int deathCount_ = 0;							//���񂾉�

	VECTOR numScale_ = { 80.0f,80.0f, 80.0f };		//���̊g�k��
	VECTOR numRot_	 = { 0.0f,0.0f, -15.0f };		//���̉�]��
};

