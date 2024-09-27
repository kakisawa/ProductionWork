#pragma once
#include <map>
#include <list>
#include <vector>
#include <string>
#include <memory>
#include <DxLib.h>

class Effect2D;

class EffectManager
{
private:
	//�����摜�̍\����
	struct DivGraphData
	{
		std::string name = {};	//���O

		int divXNum = 0;		//�摜��X�̕�����
		int divYNum = 0;		//�摜��Yno������

		int divXSize = 0;		//�摜��X�̕����T�C�Y
		int divYSize = 0;		//�摜��Y�̕����T�C�Y
	};
public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~EffectManager();

	/// <summary>
	/// �C���X�^���X�̍쐬
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static EffectManager& GetInstance() {
		static EffectManager instance;
		return instance;
	}

	/// <summary>
	/// �����摜�̃t�@�C���p�X��ǂݍ���
	/// </summary>
	void LoadDivGraphFilePath();

	/// <summary>
	/// �摜�̓ǂݍ���
	/// </summary>
	void Load();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �G�t�F�N�g�̒ǉ�
	/// </summary>
	/// <param name="name">�G�t�F�N�g��</param>
	/// <param name="size">�`��T�C�Y</param>
	/// <param name="pos">���W</param>
	void AddEffect(const std::string& name,const float size,const VECTOR& pos);

private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	EffectManager();

	EffectManager(const EffectManager&) = delete;
	void operator = (const EffectManager&) = delete;
private:
	int arrayHandle_[30] = {};													//�摜�̔z��

	std::map<std::string, std::vector<int>> handle_ = {};						//�摜�e�[�u��
	std::map<std::string, std::list<DivGraphData>>	divGraphFilePathInfo_;		//�����摜�t�@�C���p�X���܂Ƃ߂�
	std::list<std::shared_ptr<Effect2D>> effect2D_ = {};						//effect
};

