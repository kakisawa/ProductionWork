#pragma once
#include "GimmickBase.h"

class PenetrationScaffld final : public GimmickBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="materialType">�}�e���A���̃^�C�v</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	PenetrationScaffld(const int handle,const Material materialType,const ObjectInfo& objInfo);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~PenetrationScaffld();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw()override;

	/// <summary>
	/// �}�e���A���̐F��ύX����
	/// </summary>
	void ChangeMaterialColor();
private:

	int materialNum_ = 0;				//�}�e���A���̐�

	float alphaValue_ = 0.3f;			//�A���t�@�l

	bool alphaValueIncrease_ = false;	//�A���t�@�l�̑���
};

