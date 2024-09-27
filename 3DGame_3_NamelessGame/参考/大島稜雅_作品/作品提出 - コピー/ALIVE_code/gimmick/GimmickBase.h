#pragma once
#include "../object/OrnamentBase.h"

class GimmickBase : public OrnamentBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="materialType">�}�e���A���̃^�C�v</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	GimmickBase(const int handle, const Material materialType, const ObjectInfo& objInfo);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~GimmickBase();

	/// <summary>
	/// �w��̃I�u�W�F�N�g�̔z�u�f�[�^���擾����
	/// </summary>
	/// <param name="name">�擾�������I�u�W�F�N�g�̖��O</param>
	/// <param name="num">�ԍ�</param>
	/// <returns>�z�u�f�[�^</returns>
	ObjectInfo GetLoadObjectInfo(const std::string& name, int num);
};

