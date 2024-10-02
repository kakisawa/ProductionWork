#pragma once
#include "Object/Player.h"
#include <map>
#include <string>

class Player;

/// <summary>
/// �V���O���g�����g�p����CSV�t�@�C���ǂݍ��݂��s��
/// </summary>
class LoadCsv
{
public:
	// �R�s�[�R���X�g���N�^�̋֎~
	LoadCsv(const LoadCsv&) = delete;
	// ���[�u�R���X�g���N�^�̋֎~
	LoadCsv(LoadCsv&&) = delete;
	// ������Z�q�̋֎~
	LoadCsv& operator= (const LoadCsv&) = delete;
	LoadCsv& operator= (LoadCsv&&) = delete;

	// �C���X�^���X���擾����
	static LoadCsv& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new LoadCsv;
		}
		return *m_instance;
	}

	/// <summary>
	/// csv�t�@�C���̃��[�h
	/// </summary>
	void LoadData(Player::CharaData& charaData);
	
private:
	LoadCsv() = default;
	virtual ~LoadCsv() = default;

private:
	static LoadCsv* m_instance;				   // ���b�Z�[�W�̃C���X�^���X
	std::map<std::string, std::string> m_data; // ���b�Z�[�W�̃f�[�^
};