#pragma once
#include "../object/PlayerData.h"
#include "../object/ObjectData.h"
#include <list>
#include <deque>
#include <string>
#include <DxLib.h>
#include <unordered_map>

class ExternalFile
{
private:
	//�t�@�C���̎��
	enum class File
	{
		End,
		Main,
		Room,
		UIpos,
		Enemy,
		Gimmick,
		StartPos,
		Tutorial,
		CameraGimmick,
		CameraPosition,
		MainSpecialObj,
		TitleSpecialObj,

		Max,
	};

	//�t�@�C���̖��O
	std::string fileData_[static_cast<int>(File::Max)]
	{
		"End",
		"Main",
		"Room",
		"UIpos",
		"Enemy",
		"Gimmick",
		"StartPos",
		"Tutorial",
		"CameraGimmick",
		"CameraPosition",
		"MainSpecialObj",
		"TitleSpecialObj",
	};
public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ExternalFile();

	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	/// <returns></returns>
	static ExternalFile& GetInstance() {
		static ExternalFile instance;
		return instance;
	}

	/// <summary>
	/// �t�@�C�����܂Ƃ߂ēǂݍ���
	/// </summary>
	void LoadFile();

	/// <summary>
	/// �z�u�f�[�^��ǂݍ���
	/// </summary>
	void LoadArrangementData();

	/// <summary>
	/// �t�@�C����ǂݍ���
	/// </summary>
	void LoadFileHandle(const std::string& name);

	/// <summary>
	/// �Z�[�u�f�[�^�̏����o��
	/// </summary>
	/// <param name="num">���S��</param>
	void SaveDataRewriteInfo(const int num);


	////////////////Getter////////////////

	/// <summary>
	/// ����̃M�~�b�N�̔z�u�����擾����
	/// </summary>
	/// <param name="name">�擾�������I�u�W�F�N�g�̖��O</param>
	/// <returns></returns>
	ObjectInfo GetSpecifiedGimmickInfo(const std::string& name);

	/// <summary>
	/// �J����������ȓ������s����(�I�u�W�F�N�g)��
	/// �ǂ��ɂ��邩�̔z�u�f�[�^���擾����
	/// </summary>
	/// <returns></returns>
	ObjectInfo GetCameraGimmickInfo(const VECTOR& playerPos, const std::string& name);

	/// <summary>
	/// �^�C�g���̎w�肵�����O�̃I�u�W�F�N�g�z�u�f�[�^��Ԃ�
	/// </summary>
	/// <param name="name">�擾�������I�u�W�F�N�g�̖��O</param>
	/// <returns>�I�u�W�F�N�g�̃f�[�^</returns>
	ObjectInfo GetTitleSpecifiedInfo(const std::string& name);

	/// <summary>
	/// �Q�[�����C���w�肵�����O�̃I�u�W�F�N�g�z�u�f�[�^��Ԃ�
	/// </summary>
	/// <param name="name">�擾�������I�u�W�F�N�g�̖��O</param>
	/// <returns>�I�u�W�F�N�g�̃f�[�^</returns>
	ObjectInfo GetMainSpecifiedInfo(const std::string& name);

	/// <summary>
	/// �G���f�B���O�V�[���̎w�肵�����O�̃I�u�W�F�N�g�z�u�f�[�^��Ԃ�
	/// </summary>
	/// <param name="name">�擾�������I�u�W�F�N�g�̖��O</param>
	/// <returns>�I�u�W�F�N�g�̃f�[�^</returns>
	ObjectInfo GetEndSpecifiedInfo(const std::string& name);

	/// <summary>
	/// �G�l�~�[�̔z�u�f�[�^���擾����
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	/// <returns>�z�u�f�[�^</returns>
	std::list<ObjectInfo> GetEnemyInfo(const VECTOR& playerPos);

	/// <summary>
	/// �v���C���[�̊J�n�ʒu�̃f�[�^���擾����
	/// </summary>
	/// <param name="name">�J�n�ʒu�̖��O</param>
	/// <returns>�z�u�f�[�^</returns>
	VECTOR GetStartPos(const std::string& name);

	/// <summary>
	/// �`���[�g���A����\������|�C���g�̔z�u�f�[�^���擾����
	/// </summary>
	/// <param name="pos">�v���C���[�̃|�W�V����</param>
	/// <returns>�z�u�f�[�^</returns>
	ObjectInfo GetTutorialObjInfo(const VECTOR& pos);

	/// <summary>
	/// �J�����̍��W�f�[�^���擾����
	/// </summary>
	/// <param name="name">���O</param>
	/// <returns>���W�f�[�^</returns>
	VECTOR GetCameraTargetPos(const std::string& name);

	/// <summary>
	/// �w��UI�̔z�u���W���擾����
	/// </summary>
	/// <param name="name">�擾������UI�̖��O</param>
	/// <returns>���W</returns>
	VECTOR GetUIPos(const std::string& name);

	/// <summary>
	/// �v���C���[�Ɋւ�������擾����
	/// </summary>
	/// <returns>playerData</returns>
	PlayerInfo GetPlayerInfo() const { return player_; }

	/// <summary>
	/// ���C���X�e�[�W�̓���ȃI�u�W�F�N�g�̔z�u�f�[�^���擾����
	/// </summary>
	/// <param name="name">���O</param>
	/// <returns>�z�u�f�[�^</returns>
	ObjectInfo GetMainSpecialObjectInfo(const std::string& name) { return loadSpecialInfo_[name]; }

	/// <summary>
	/// �J�n�ꏊ�̖��O���擾����
	/// </summary>
	/// <returns>�J�n�ꏊ�̖��O</returns>
	const std::string& GetStartName()const { return startPosName_; }

	/// <summary>
	/// savedata���擾����
	/// </summary>
	/// <returns>�ߋ��̎��S��</returns>
	const std::deque<int>& GetTotalDeathNum() const { return pastTotalDeathNum_; }

	/// <summary>
	/// �I�u�W�F�N�g�̔z�u�����擾
	/// </summary>
	/// <returns>�z�u�f�[�^</returns>
	const std::unordered_map<std::string, std::list<ObjectInfo>>& GetGimmickInfo() const { return loadGimmickInfo_; }

	/// <summary>
	/// ���C���X�e�[�W�I�u�W�F�N�g�̔z�u�A��]���A�g�k�����擾����
	/// </summary>
	/// <returns>�I�u�W�F�N�g�̔z�u�f�[�^�Ȃǂ��܂Ƃ߂��ϐ�</returns>
	const std::unordered_map<std::string, std::list<ObjectInfo>>& GetLoadMainStageObjectInfo()const { return loadMainStageObjInfo_; }

	/// <summary>
	/// �I�[�v�j���O�X�e�[�W�I�u�W�F�N�g�̔z�u�A��]���A�g�k�����擾����
	/// </summary>
	/// <returns>�I�u�W�F�N�g�̔z�u�f�[�^�Ȃǂ��܂Ƃ߂��ϐ�</returns>
	const std::unordered_map<std::string, std::list<ObjectInfo>>& GetLoadOpeningStageObjectInfo() const { return loadOpeningStageObjInfo_; }

	/// <summary>
	/// �G���f�B���O�V�[���I�u�W�F�N�g�̔z�u�A��]���A�g�k�����擾����
	/// </summary>
	/// <returns>�I�u�W�F�N�g�̔z�u�f�[�^�Ȃǂ��܂Ƃ߂��ϐ�</returns>
	const std::unordered_map<std::string, std::list<ObjectInfo>>& GetLoadEndingStageObjectInfo()const { return loadEndStageObjInfo_; }


	////////////////Setter////////////////

	/// <summary>
	/// �J�n�ꏊ�̖��O��ݒ肷��
	/// </summary>
	/// <param name="name">�J�n�ꏊ�̖��O</param>
	void SetStartName(const std::string& name);

	/// <summary>
	/// ���񂾉񐔂��Z�b�g����
	/// </summary>
	/// <param name="num"></param>
	void SetDeathCount(const int num);
private:

	/// <summary>
	/// �v���C���[�̃X�e�[�^�X���������o�� 
	/// </summary>
	void RewritePlayerInfo();

	/// <summary>
	/// �v���C���[�̃X�e�[�^�X�Ɋւ������ǂݍ���
	/// </summary>
	void LoadPlayerInfo();

	/// <summary>
	/// �Z�[�u�f�[�^��ǂݍ���
	/// </summary>
	/// <param name="filename">�t�@�C���̖��O</param>
	void LoadSaveDataInfo(const std::string& filename);

	/// <summary>
	/// �I�u�W�F�N�g�̔z�u����ǂݍ���list�Ɋi�[����
	/// </summary>
	/// <param name="filename">�t�@�C���̖��O</param>
	/// <param name="dataTable">�f�[�^�e�[�u��</param>
	void LoadObjectDataList(const std::string& name,std::unordered_map<std::string, std::list<ObjectInfo>>& dataTable);

	/// <summary>
	/// �I�u�W�F�N�g�̔z�u����ǂݍ���
	/// </summary>
	/// /// <param name="filename">�t�@�C���̖��O</param>
	/// <param name="dataTable">�f�[�^�e�[�u��</param>
	void LoadObjectData(const std::string& name,std::unordered_map<std::string, ObjectInfo>& dataTable);
private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ExternalFile();

	ExternalFile(const ExternalFile&) = delete;
	void operator = (const ExternalFile&) = delete;
private:
	PlayerInfo player_ = {};																//�v���C���[�̃X�e�[�^�X���

	std::string startPosName_ = {};															//�J�n�ʒu�̖��O

	std::deque<int> pastTotalDeathNum_ = {};												//�ߋ�5��̎��S��

	std::unordered_map<std::string, int> loadFile_;											//���[�h�����t�@�C��

	std::unordered_map<std::string, std::list<ObjectInfo>>	loadMainStageObjInfo_;		//���C���X�e�[�W�I�u�W�F�N�g�̔z�u�f�[�^
	std::unordered_map<std::string, std::list<ObjectInfo>>	loadOpeningStageObjInfo_;	//�I�[�v�j���O�X�e�[�W�I�u�W�F�N�g�̔z�u�f�[�^
	std::unordered_map<std::string, std::list<ObjectInfo>>	loadEndStageObjInfo_;		//�I�[�v�j���O�X�e�[�W�I�u�W�F�N�g�̔z�u�f�[�^
	std::unordered_map<std::string, std::list<ObjectInfo>>	loadGimmickInfo_;			//�M�~�b�N�̔z�u�f�[�^	
	std::unordered_map<std::string, std::list<ObjectInfo>>	loadCameraGimmickInfo_;		//�J�����M�~�b�N�̔z�u�f�[�^		
	std::unordered_map<std::string, std::list<ObjectInfo>>	loadEnemyInfo_;				//�G�̔z�u�f�[�^
	
	std::unordered_map<std::string, ObjectInfo>				loadUIInfo_;				//UI�̔z�u�f�[�^	
	std::unordered_map<std::string, ObjectInfo>				loadStartPos_;				//�J�n�ʒu�̃f�[�^	
	std::unordered_map<std::string, ObjectInfo>				loadSpecialInfo_;			//����ȃI�u�W�F�N�g�̔z�u�f�[�^	
	std::unordered_map<std::string, ObjectInfo>				loadTutorialInfo_;			//�`���[�g���A���|�C���g�̔z�u�f�[�^	
	std::unordered_map<std::string, ObjectInfo>				loadCameraPosInfo_;			//�J�����̔z�u�f�[�^	
};

