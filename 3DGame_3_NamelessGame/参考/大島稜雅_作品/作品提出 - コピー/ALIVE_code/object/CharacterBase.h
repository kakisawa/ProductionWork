#pragma once
#include "DxLib.h"
#include "ObjectBase.h"
#include <memory>

class CharacterBase : public ObjectBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="materialType">�}�e���A���̃^�C�v</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	CharacterBase(const int handle, const Material materialType, const ObjectInfo objInfo);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CharacterBase() {};
};

