#pragma once
#include <list>
#include <string>
#include <DxLib.h>
#include <unordered_map>

class ModelManager
{
public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ModelManager();

	/// <summary>
	/// �C���X�^���X�쐬
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static ModelManager& GetInstance() {
		static ModelManager instance;
		return instance;
	}

	/// <summary>
	///	���f���t�@�C���p�X��ǂݍ���
	/// </summary>
	void LoadModelFilePath();

	/// <summary>
	/// ���f�������[�h����
	/// </summary>
	void LoadModel();

	/// <summary>
	/// 3D���f���n���h�����擾����
	/// </summary>
	/// <returns>���f���n���h��</returns>
	int GetModelHandle(const std::string& name);
private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ModelManager() = default;

	ModelManager(const ModelManager&) = delete;
	void operator = (const ModelManager&) = delete;
private:
	std::unordered_map<std::string, std::list<std::string>>	modelFilePathInfo_;			//���f���t�@�C���p�X���܂Ƃ߂�
	std::unordered_map<std::string, int> modelHandle_;									//���f���n���h���e�[�u��
};

