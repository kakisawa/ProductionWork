#pragma once
#include "GimmickBase.h"
#include <memory>

class Switch;

class TransparentObject final : public GimmickBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="materialType">�}�e���A���̃^�C�v</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	TransparentObject(const int handle, const Material materialType, const ObjectInfo& objInfo);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~TransparentObject();

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
	/// �X�C�b�`���f���Ǝ��̂̏Փ˔�����s��
	/// </summary>
	/// <param name="deadPerson">���̂̃|�C���^</param>
	void UpdateForCorpse(const std::shared_ptr<ObjectBase>& corpse)override;

	/// <summary>
	/// �Փ˔�����s�����f���̒ǉ�
	/// </summary>
	/// <returns>���f���|�C���^</returns>
	virtual std::shared_ptr<Model> AddCollModel() const override;

private:

	int materialNum_ = 0;								//���̃N���X�ŃC���X�^���X�������3D���f���̃}�e���A���̐���ۊǂ���

	float alphaValue_ = 0.0f;							//���ߒl��ۊǂ���

	std::shared_ptr<Switch> switch_;					//�X�C�b�`�̃|�C���^

};

