#pragma once
#include "EnemyBase.h"

class ThrowEnemy final : public EnemyBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="materialType">�}�e���A���̃^�C�v</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	ThrowEnemy(const int handle, const Material materialType, const ObjectInfo& objInfo);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~ThrowEnemy();

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
	/// �e��������
	/// </summary>
	/// <param name="shotManager">�e���Ǘ�����N���X�̃|�C���^</param>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	/// <param name="height">�v���C���[�̍���</param>
	void Shot(const std::shared_ptr<ShotManager>& shotManager,const VECTOR& playerPos, const float height);

private:
	bool isFakeThrow_ = false;						//������ӂ�����邩
};

