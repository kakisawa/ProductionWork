#pragma once

#include "PlayerData.h"
#include "CharacterBase.h"

#include <list>
#include <memory>
#include <unordered_map>

class Aster;
class ShotManager;

class EnemyBase : public CharacterBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="materialType">�}�e���A���̃^�C�v</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	EnemyBase(const int handle, const Material materialType, const ObjectInfo& objInfo);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~EnemyBase();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="player">�v���C���[�̎Q��</param>
	void Update(Player& player) override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw()override;

	/// <summary>
	/// �v���C���[��ǐՂ���
	/// </summary>
	/// <param name="playerPos">�v���C���[�̍��W</param>
	void TrackingUpdate(const VECTOR& playerPos);

	/// <summary>
	/// �v���C���[�����G����
	/// </summary>
	/// <param name="playerPos">�v���C���[�̍��W</param>
	void SearchForPlayer(const VECTOR& playerPos);

	/// <summary>
	/// ���[�g�ʂ�Ɉړ�����
	/// </summary>
	/// <param name="playerPos">�v���C���[�̍��W</param>
	void RoutingUpdate(const VECTOR& playerPos);

	/// <summary>
	/// �G����v���C���[�̒��������ɃI�u�W�F�N�g�����邩
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	/// <returns>�I�u�W�F�N�g�����邩</returns>
	bool IsThereAnObject(const VECTOR& playerPos);

	/// <summary>
	/// �G���e��������
	/// </summary>
	/// <param name="shotManager">�e���Ǘ�����N���X�̃|�C���^</param>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	/// <param name="height">�v���C���[�̍���</param>
	void Shot(const std::shared_ptr<ShotManager>& shotManager, const VECTOR& playerPos, const float height);

	/// <summary>
	/// ��]�s��Ɗg�k�s�����Z�����s����擾����
	/// </summary>
	/// <returns>��]�s��Ɗg�k�s�����Z�����s��</returns>
	MATRIX CombiningRotAndScallMat(const VECTOR& distance);

	/// <summary>
	/// �����e�̒��_�̃|�W�V�������擾����
	/// </summary>
	/// <param name="angle">�p�x</param>
	/// <returns>�|�W�V����</returns>
	VECTOR VertexPosition(const float angle);

	/// <summary>
	/// �����e���ǂ��̕`��
	/// </summary>
	void DrawPolygon3D();
protected:

	bool isDetection_ = false;						//�v���C���[�����m������
	bool isThrow_ = false;							//�����郂�[�V��������

	VECTOR initFrontVec_ = {};						//�������ʃx�N�g��
	VECTOR frontVec_ = {};							//�G�̐��ʃx�N�g��������
	VECTOR pushVec_ = {};							//�v���C���[���G�ɂԂ������Ƃ��ɉ����x�N�g��

	std::shared_ptr<Aster> Aster_;					//Aster�̃|�C���^
};

