#pragma once
#include "OrnamentBase.h"

class StreetLight final : public OrnamentBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="materialType">�}�e���A���̃^�C�v</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	StreetLight(const int handle, const Material materialType, const ObjectInfo objInfo);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~StreetLight();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="player">�v���C���[�̎Q��</param>
	void Update(Player& player) override;

	/// <summary>
	/// �v���C���[�̈��͈͓��ɓ������烉�C�g��_������
	/// </summary>
	/// <param name="distanceSize">�v���C���[�ƊX���̋����̑傫��</param>
	void LightOn(float distanceSize);

private:
	int lightHandle_ = -1;		//���C�g�̃n���h��
};

