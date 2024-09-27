#pragma once
#include "ObjectData.h"
#include <list>
#include <string>
#include <memory>
#include <DxLib.h>
#include <unordered_map>

class ObjectBase;
class Model;
class Player;
class ShotManager;

class ObjectManager
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ObjectManager();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~ObjectManager();

	/// <summary>
	/// ���C���X�e�[�W�̃I�u�W�F�N�g����
	/// </summary>
	void MainStageObjectGenerator();

	/// <summary>
	/// �I�[�v�j���O�V�[���̃I�u�W�F�N�g�𐶐�����
	/// </summary>
	void OpeningStageObjectGenerator();

	/// <summary>
	/// �G���f�B���O�V�[���̃I�u�W�F�N�g�𐶐�����
	/// </summary>
	void EndStageObjectGenerator();

	/// <summary>
	/// ���̐���
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="objInfo">�I�u�W�F�N�g�̔z�u�f�[�^��</param>
	void CorpseGenerator(const int handle, const ObjectInfo& objInfo);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="player">�v���C���[�̎Q��</param>
	/// <param name="shotManager">�V���b�g�}�l�[�W���[�̃X�}�[�g�|�C���^</param>
	void Update(Player& player,const std::shared_ptr<ShotManager>& shotManager);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �Փ˔�����s�����f����ǉ�����
	/// </summary>
	void AddCheckCollModel();

	/// <summary>
	/// �~����̃|�W�V�������擾����
	/// </summary>
	/// <param name="info"></param>
	/// <param name="loadObjPos"></param>
	void CircumferencePosition(const float angle,VECTOR& infoPos, const VECTOR& playerPos);

	/// <summary>
	/// �G����
	/// </summary>
	/// <param name="deathCount">���܂łɃv���C���[�����S������</param>
	/// <param name="info">�z�u�f�[�^</param>
	void EnemyGenerator(const int deathCount, const ObjectInfo& info);

	////////////////Getter////////////////

	/// <summary>
	/// �Փ˔���Ɏg�p���郂�f�����擾����
	/// </summary>
	/// <returns>�Փ˔�����s�����f���X�}�[�g�|�C���^�̃��X�g</returns>
	const std::list<std::shared_ptr<Model>>& GetAllCheckCollModel();

	/// <summary>
	/// ����̃��f���|�C���^���擾����
	/// </summary>
	/// <param name="type">�擾�������I�u�W�F�N�g�^�C�v</param>
	/// <returns>����̃��f���|�C���^</returns>
	std::shared_ptr<Model> GetSpecificModel(const ObjectType type);

	/// <summary>
	/// ����̃I�u�W�F�N�g�x�[�X�|�C���^���擾����
	/// </summary>
	/// <param name="type">�擾�����I�u�W�F�N�g�^�C�v</param>
	/// <returns>����̃I�u�W�F�N�g�x�[�X�|�C���^</returns>
	std::list<std::shared_ptr<ObjectBase>> GetSpecificObject(const ObjectType type);

private:
	/// <summary>
	/// ���񂾉񐔕���������
	/// </summary>
	/// <param name="deathCount">���܂łɃv���C���[�����S������</param>
	/// <param name="info">�z�u�f�[�^</param>
	void GeneratedForTheNumberOfTimesYouDie(const int deathCount,ObjectInfo info);

	/// <summary>
	/// ����̉񐔕���������
	/// </summary>
	/// <param name="deathCount">���܂łɃv���C���[�����S������</param>
	/// <param name="str">������</param>
	/// <param name="info">�z�u�f�[�^</param>
	void GeneratePredeterminedNumberOfTimes(const int deathCount, const std::string& str, const ObjectInfo& info);

	/// <summary>
	/// ���񂾉񐔂ɂ���ĕ⏕����𐶐�����
	/// </summary>
	/// <param name="deathCount">���񂾉�</param>
	/// <param name="info">�z�u�f�[�^</param>
	void GenerateCorpseMountain(const int deathCount, const ObjectInfo& info);

	/// <summary>
	/// �u�������@
	/// </summary>
	/// <param name="name">�I�u�W�F�N�g�̖��O</param>
	/// <param name="objType">�I�u�W�F�N�g�̃^�C�v</param>
	/// <param name="materialType">�I�u�W�F�N�g�̃^�C�v</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	void OrnamentGenerator(const std::string& name, const ObjectType objType, const Material materialType, const ObjectInfo& objInfo);

	/// <summary>
	/// �M�~�b�N�����@
	/// </summary>
	/// <param name="name">�I�u�W�F�N�g�̖��O</param>
	/// <param name="objType">�I�u�W�F�N�g�̃^�C�v</param>
	/// <param name="materialType">�I�u�W�F�N�g�̃^�C�v</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	void GimmickObjectGenerator(const std::string& name, const ObjectType objType, const Material materialType, const ObjectInfo& objInfo);
private:

	std::list<std::shared_ptr<Model>> checkCollList_;											//�Փ˔�����s�����f���̃��X�g
	std::unordered_map<std::string, bool>  usedEnemyList_;										//�g�p�ς݃G�l�~�[�̃��X�g
	std::unordered_map<std::string, bool>  usedCorpseMtList_;									//�g�p�ς݂̎��̂̃��X�g

	std::unordered_map<ObjectType, std::list<std::shared_ptr<ObjectBase>>> objects_;			//�I�u�W�F�N�g��objectType���L�[��list�ł���
};

