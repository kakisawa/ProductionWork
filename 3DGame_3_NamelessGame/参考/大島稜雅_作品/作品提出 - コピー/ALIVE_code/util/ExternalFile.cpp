#include "ExternalFile.h"
#include "Util.h"
#include "InputState.h"
#include "../object/ObjectData.h"

#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <cassert>

using json = nlohmann::json;

//�R���X�g���N�^
ExternalFile::ExternalFile()
{
}

//�f�X�g���N�^
ExternalFile::~ExternalFile()
{
	RewritePlayerInfo();
}

//�t�@�C�����܂Ƃ߂ēǂݍ���
void ExternalFile::LoadFile()
{
	for (const auto& file : fileData_)
	{
		LoadFileHandle(file);
	}
}

//�z�u�f�[�^��ǂݍ���
void ExternalFile::LoadArrangementData()
{
	LoadPlayerInfo();
	LoadSaveDataInfo("saveData");
	//�}�b�v�Ɋi�[����
	LoadObjectData(fileData_[static_cast<int>(File::UIpos)], loadUIInfo_);
	LoadObjectData(fileData_[static_cast<int>(File::StartPos)], loadStartPos_);
	LoadObjectData(fileData_[static_cast<int>(File::Tutorial)], loadTutorialInfo_);
	LoadObjectData(fileData_[static_cast<int>(File::MainSpecialObj)], loadSpecialInfo_);
	LoadObjectData(fileData_[static_cast<int>(File::CameraPosition)], loadCameraPosInfo_);
	LoadObjectData(fileData_[static_cast<int>(File::TitleSpecialObj)], loadSpecialInfo_);
	//�}�b�v�̃��X�g�Ɋi�[����
	LoadObjectDataList(fileData_[static_cast<int>(File::Enemy)], loadEnemyInfo_);
	LoadObjectDataList(fileData_[static_cast<int>(File::End)], loadEndStageObjInfo_);
	LoadObjectDataList(fileData_[static_cast<int>(File::Gimmick)], loadGimmickInfo_);
	LoadObjectDataList(fileData_[static_cast<int>(File::Main)], loadMainStageObjInfo_);
	LoadObjectDataList(fileData_[static_cast<int>(File::Room)], loadOpeningStageObjInfo_);
	LoadObjectDataList(fileData_[static_cast<int>(File::CameraGimmick)], loadCameraGimmickInfo_);
}

//�t�@�C����ǂݍ���
void ExternalFile::LoadFileHandle(const std::string& name)
{
	//�t�@�C���p�X�̐���
	std::string filepath = "data/objData/";
	filepath = filepath + name + ".pos";

	//�t�@�C���̃��[�h
	loadFile_[name] = FileRead_open(filepath.c_str());
}

//�Z�[�u�f�[�^�̏����o��
void ExternalFile::SaveDataRewriteInfo(const int num)
{
	json saveData = 
	{
		{"name","saveData"},
		{"pastTotalDeath",pastTotalDeathNum_},
	};

	std::string filename = saveData["name"];
	std::string extension = ".json";
	filename += extension;
	filename = "data/jsonFile/" + filename;

	std::ofstream writeing_file;
	writeing_file.open(filename, std::ios::out);
	writeing_file << saveData.dump() << std::endl;
	writeing_file.close();
}

//����̃M�~�b�N�̔z�u�����擾����
ObjectInfo ExternalFile::GetSpecifiedGimmickInfo(const std::string& name)
{

	ObjectInfo info = {};

	//�ԍ��ƋL���𔲂������O���擾����
	std::string gimmickName = StrUtil::GetStringWithPartsAfterTheSymbolDeleted(name, ".");

	//�����̖��O��loadGimmickInfo_�̔z��̒��Ŗ��O����v������̂�T��
	//��v�����z��̏����擾����
	for (const auto& specifiedObj : loadGimmickInfo_[gimmickName])
	{
		if (specifiedObj.name != name)
		{
			continue;
		}
		info = specifiedObj;
	}

	//�����o��������Ԃ�
	return info;
}

//�J����������ȓ������s����(�I�u�W�F�N�g)��
// �ǂ��ɂ��邩�̔z�u�f�[�^���擾����
ObjectInfo ExternalFile::GetCameraGimmickInfo(const VECTOR& playerPos, const std::string& name)
{

	float minDistance = 10000.0f;
	ObjectInfo info = {};

	for (const auto& data : loadCameraGimmickInfo_[name])
	{
		float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(data.pos, playerPos);
		if (minDistance > distanceSize)
		{
			minDistance = distanceSize;
			info = data;
		}
	}
	return info;
}

//�^�C�g���̎w�肵�����O�̃I�u�W�F�N�g�z�u�f�[�^��Ԃ�
ObjectInfo ExternalFile::GetTitleSpecifiedInfo(const std::string& name)
{
	ObjectInfo info = {};
		
	for (const auto& obj : loadOpeningStageObjInfo_)
	{
		if (obj.first == name) 
		{
			info = loadOpeningStageObjInfo_[name].front();
		}
	}

	return info;
}

//�Q�[�����C���̎w�肵�����O�̃I�u�W�F�N�g�z�u�f�[�^��Ԃ�
ObjectInfo ExternalFile::GetMainSpecifiedInfo(const std::string& name)
{
	ObjectInfo info = {};

	for (const auto& obj : loadMainStageObjInfo_)
	{
		if (obj.first == name)
		{
			info = loadMainStageObjInfo_[name].front();
		}
	}

	return info;
}

//�G���f�B���O�V�[���̎w�肵�����O�̃I�u�W�F�N�g�z�u�f�[�^��Ԃ�
ObjectInfo ExternalFile::GetEndSpecifiedInfo(const std::string& name)
{
	ObjectInfo info = {};

	for (const auto& obj : loadEndStageObjInfo_)
	{
		if (obj.first == name)
		{
			info = loadEndStageObjInfo_[name].front();
		}
	}

	return info;
}

//�G�l�~�[�̔z�u�f�[�^���擾����
std::list<ObjectInfo> ExternalFile::GetEnemyInfo(const VECTOR& playerPos)
{
	std::list<ObjectInfo> info = {};
	float distanceSize = 0.0f;
	float minSize = 3000.0f;

	for (const auto& list : loadEnemyInfo_)
	{
		for (const auto& enemy : list.second)
		{
			distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(enemy.pos, playerPos);
			if (distanceSize < minSize) 
			{
				info.push_back(enemy);
			}
		}
	}

	return info;
}

//�v���C���[�̊J�n�ʒu�̃f�[�^���擾����
VECTOR ExternalFile::GetStartPos(const std::string& name)
{
	if (static_cast<int>(name.size()) == 0)
	{
		return loadStartPos_["Chapter0"].pos;
	}

	return loadStartPos_[name].pos;
}

//�`���[�g���A����\������|�C���g�̔z�u�f�[�^���擾����
ObjectInfo ExternalFile::GetTutorialObjInfo(const VECTOR& pos)
{

	ObjectInfo info = {};
	float distanceSize = 0.0f;
	float min = 10000.0f;

	for (const auto& tutorial : loadTutorialInfo_)
	{
		distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(tutorial.second.pos, pos);
		if (min > distanceSize) 
		{
			min = distanceSize;
			info = tutorial.second;
		}
	}

	return info;
}


//�J�����̍��W�f�[�^���擾����
VECTOR ExternalFile::GetCameraTargetPos(const std::string& name)
{
	VECTOR pos = {};

	for (const auto& data : loadCameraPosInfo_)
	{
		auto& keyName = data.first;
		if (keyName == name) 
		{
			pos = data.second.pos;
		}
	}

	return pos;
}

//�w��UI�̔z�u���W���擾����
VECTOR ExternalFile::GetUIPos(const std::string& name)
{
	VECTOR pos = {};

	for (const auto& data : loadUIInfo_)
	{
		auto& keyName = data.first;
		if (keyName == name) 
		{
			pos = data.second.pos;
		}
	}

	return pos;
}

//�J�n�ꏊ�̖��O��ݒ肷��
void ExternalFile::SetStartName(const std::string& name)
{
	startPosName_ = name;
}

//���񂾉񐔂��Z�b�g����
void ExternalFile::SetDeathCount(const int num)
{
	pastTotalDeathNum_.pop_front();
	pastTotalDeathNum_.push_back(num);
}

//�v���C���[�̃X�e�[�^�X�������o��
void ExternalFile::RewritePlayerInfo()
{
	//�v���C���[�̃X�e�[�^�X���܂Ƃ߂�
	json player = 
	{
	   {"name","player"},
	   {"jumpPower",10.0f},
	   {"runningJumpPower",8.0f},
	   {"rotSpeed",15.0f},
	   {"walkSpeed",5.0f},
	   {"runningSpeed",10.0f},
	};

	//�����o���ꏊ�̎w��
	std::string filename = player["name"];
	std::string extension = ".json";
	filename += extension;
	filename = "data/jsonFile/" + filename;

	//�o��
	std::ofstream writeing_file;
	writeing_file.open(filename, std::ios::out);
	writeing_file << player.dump() << std::endl;
	writeing_file.close();
}

//�v���C���[�̃X�e�[�^�X����ǂݍ���
void ExternalFile::LoadPlayerInfo()
{
	//�t�@�C���p�X
	std::string path = "data/jsonFile/player.json";

	//�t�@�C�����J��
	std::ifstream ifs(path.c_str());
	assert(ifs);

	//�悭�킩��Ȃ�
	json json_;
	ifs >> json_;

	//�v���C���[�̃X�e�[�^�X�̎擾
	player_.jumpPower			= json_["jumpPower"];
	player_.runningJumpPower	= json_["runningJumpPower"];
	player_.rotSpeed			= json_["rotSpeed"];
	player_.walkSpeed			= json_["walkSpeed"];
	player_.runningSpeed		= json_["runningSpeed"];

	//����
	ifs.close();
}

//�Z�[�u�f�[�^��ǂݍ���
void ExternalFile::LoadSaveDataInfo(const std::string& filename)
{
	std::string path = "data/jsonFile/";
	path += filename;
	path += ".json";

	std::ifstream ifs(path.c_str());
	assert(ifs);

	json json;
	ifs >> json;

	for (int i = 0; i < 5; i++)
	{
		pastTotalDeathNum_.push_back(json["pastTotalDeath"][i]);
	}

	ifs.close();
}

//�I�u�W�F�N�g�̃|�W�V������ǂݍ���
void ExternalFile::LoadObjectDataList(const std::string& name,std::unordered_map<std::string, std::list<ObjectInfo>>& dataTable)
{
	//�ǂݍ��񂾃f�[�^�̃n���h�����擾
	auto dataHandle = loadFile_[name.c_str()];

	//�f�[�^���̎擾
	int dataNum = 0;
	int result = FileRead_read(&dataNum, sizeof(dataNum), dataHandle);
	assert(result != -1);

	for (int i = 0; i < dataNum; i++)
	{

		ObjectInfo info = {};

		//���O�̕����񐔂�ǂݎ��
		uint8_t nameSize = 0;
		result = FileRead_read(&nameSize, sizeof(nameSize), dataHandle);
		assert(result != -1);

		//���O���̂��̂�ǂݎ��
		info.name.resize(nameSize);
		result = FileRead_read(info.name.data(), sizeof(char) * nameSize, dataHandle);
		assert(result != -1);

		//���W�f�[�^��info.pos���ǂݎ��
		result = FileRead_read(&info.pos, sizeof(info.pos), dataHandle);
		assert(result != -1);

		//��]����info.rot���ǂݎ��
		result = FileRead_read(&info.rot, sizeof(info.rot), dataHandle);
		assert(result != -1);

		//�g�k����info.scale���ǂݎ��
		result = FileRead_read(&info.scale, sizeof(info.scale), dataHandle);
		assert(result != -1);

		std::string keyName = StrUtil::GetStringWithPartsAfterTheSymbolDeleted(info.name, ".");

		//�ǉ�
		dataTable[keyName].push_back(info);
	}

	//�t�@�C�������
	FileRead_close(dataHandle);

	//�ǂݎ�����f�[�^�̉�]����x���@����ʓx�@�ւƕϊ�����
	for (auto& obj : dataTable)
	{
		for (auto& objSecond : obj.second)
		{
			objSecond.rot = MathUtil::VECTORDegreeToRadian(objSecond.rot);
		}
	}
}

//�I�u�W�F�N�g�̔z�u����ǂݍ���
void ExternalFile::LoadObjectData(const std::string& name,std::unordered_map<std::string, ObjectInfo>& dataTable)
{
	//�ǂݍ��񂾃f�[�^�̃n���h�����擾
	auto dataHandle = loadFile_[name.c_str()];

	//�f�[�^���̎擾
	int dataNum = 0;
	int result = FileRead_read(&dataNum, sizeof(dataNum), dataHandle);
	assert(result != -1);

	for (int i = 0; i < dataNum; i++) 
	{
		ObjectInfo info = {};

		//���O�̕����񐔂�ǂݎ��
		uint8_t nameSize = 0;
		result = FileRead_read(&nameSize, sizeof(nameSize), dataHandle);
		assert(result != -1);

		//���O���̂��̂�ǂݎ��
		info.name.resize(nameSize);
		result = FileRead_read(info.name.data(), sizeof(char) * nameSize, dataHandle);
		assert(result != -1);

		//���W�f�[�^��info.pos���ǂݎ��
		result = FileRead_read(&info.pos, sizeof(info.pos), dataHandle);
		assert(result != -1);

		//��]����info.rot���ǂݎ��
		result = FileRead_read(&info.rot, sizeof(info.rot), dataHandle);
		assert(result != -1);

		//�g�k����info.scale���ǂݎ��
		result = FileRead_read(&info.scale, sizeof(info.scale), dataHandle);
		assert(result != -1);

		std::string keyName = StrUtil::GetStringWithPartsAfterTheSymbolDeleted(info.name, ".");

		//�ǉ�
		dataTable[keyName] = info;
	}

	//�t�@�C�������
	FileRead_close(dataHandle);

	//�ǂݎ�����f�[�^�̉�]����x���@����ʓx�@�ւƕϊ�����
	for (auto& obj : dataTable) 
	{
		obj.second.rot = MathUtil::VECTORDegreeToRadian(obj.second.rot);
	}
}

