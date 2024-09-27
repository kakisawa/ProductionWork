#pragma once

#include "ObjectData.h"

#include <DxLib.h>
#include <memory>

class Model;
class Player;

class ObjectBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="materialType">�}�e���A���̃^�C�v</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	ObjectBase(const int handle, const Material materialType, const ObjectInfo& objInfo);
	
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~ObjectBase();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="player">�v���C���[�̎Q��</param>
	virtual void Update(Player& player);
	
	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// ���̂ɑ΂��Ă̍X�V
	/// </summary>
	/// <param name="pointer">���̂̃|�C���^</param>
	virtual void UpdateForCorpse(const std::shared_ptr<ObjectBase>& pointer);

	/// <summary>
	/// �Փ˔�����s�����f����ǉ�����
	/// </summary>
	/// <returns>���f���|�C���^</returns>
	virtual std::shared_ptr<Model> AddCollModel() const;


	//////////////GETTER//////////////

	/// <summary>
	/// ���f���̃X�}�[�g�|�C���^���擾����
	/// </summary>
	/// <returns>���f���|�C���^</returns>
	const std::shared_ptr<Model>& GetModelPointer()const;

	/// <summary>
	/// �|�W�V�����̎擾
	/// </summary>
	/// <returns>�|�W�V����</returns>
	const VECTOR& GetPos() const { return pos_; }

	/// <summary>
	/// ���݁A���̃I�u�W�F�N�g���L����������
	/// </summary>
	/// <returns></returns>
	bool GetIsEnabled() const { return isEnable_; };

	/// <summary>
	/// �Փ˔�����s�����擾����
	/// </summary>
	/// <returns>�Փ˔�����s����</returns>
	bool GetIsCollCheck() const { return isCollCheck_; }

	/// <summary>
	/// �I�u�W�F�N�g���^��ł��邩�ǂ����擾����
	/// </summary>
	/// <returns>�I�u�W�F�N�g���^��ł��邩</returns>
	bool GetIsTransit() const { return isTransit_; }


	//////////////SETTER//////////////

	/// <summary>
	/// �����^�ђ��t���O�̐ݒ�
	/// </summary>
	/// <param name="isTransit">true�F�^��ł���@false�F�^��ł��Ȃ�</param>
	void SetIsTransit(const bool isTransit) { isTransit_ = isTransit; }

	/// <summary>
	/// ���݁A���̃I�u�W�F�N�g���L����������
	/// </summary>
	/// <param name="isEnable">true�F�L���@false�F����</param>
	void SetIsEnable(const bool isEnable) { isEnable_ = isEnable; }

protected:

	bool isEnable_ = false;						//�L����������
	bool isTransit_ = false;					//�����^�ђ���
	bool isCollCheck_ = false;					//�Փ˔�����s����
	bool isUpdateColl_ = false;					//�Փ˔�����s���ہA�Փ˔�����s�����f���t���[���̍X�V���K�v��

	VECTOR pos_ = {};							//�|�W�V����
	VECTOR rot_ = {};							//��]��
	VECTOR scale_ = {};							//�g�k��

	std::shared_ptr<Model> model_;				//���f���N���X�̃X�}�[�g�|�C���^
};

