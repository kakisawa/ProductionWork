#pragma once
#include <list>
#include <memory>
#include <DxLib.h>

class Model;
class Shot;
class Player;

class ShotManager
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ShotManager();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~ShotManager();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �e���v���C���[�ƏՓ˂������`�F�b�N����
	/// </summary>
	/// <param name="player"></param>
	void Hit(Player& player);

	/// <summary>
	/// �e������
	/// </summary>
	/// <param name="framePos">�����|�W�V����</param>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	/// <param name="height">�v���C���[�̍���</param>
	void Fire(const VECTOR& framePos, const VECTOR& playerPos, const float height);

private:
	std::list<std::shared_ptr<Shot>> shots_;		//���f���|�C���^�̃��X�g
};

