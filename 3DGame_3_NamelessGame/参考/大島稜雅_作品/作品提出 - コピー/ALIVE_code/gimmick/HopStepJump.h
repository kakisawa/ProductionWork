#pragma once
#include "GimmickBase.h"

class HopStepJump final : public GimmickBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="materialType">�}�e���A���̃^�C�v</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	HopStepJump(const int handle, const Material materialType, const ObjectInfo& objInfo);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~HopStepJump();

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
	/// ���݂̔ԍ���ς���
	/// </summary>
	void ChangeCurrentNum();

	/// <summary>
	/// �A���t�@�l��ύX����
	/// </summary>
	void ChangeAlphaValue();

	/// <summary>
	/// �}�e���A���̐F��ς���
	/// </summary>
	void ChangeMaterialColor();

private:

	int currentNum_ = 0;			//���݂̔ԍ�
	int materialNum_ = 0;			//�}�e���A���̐�

	float elapseddTime_ = 0.0f;		//�o�ߎ���
	float alphaValue_ = 0.0f;		//���ߒl

};

