#pragma once
#include <vector>

class CharacterBase;

/// <summary>
/// csv�t�@�C���ǂݍ���
/// </summary>
class LoadData
{
public:
	LoadData() {};
	LoadData(CharacterBase& data, int charType);
	~LoadData();
	void Draw();

	// �L�����N�^�[�̃f�[�^��ǂݍ���
	void LoadCharaData(CharacterBase& data, int charType);
	// �A�j���[�V�������x�̃f�[�^��ǂݍ���
	void LoadAnimSpeedData(CharacterBase& data, int charType);
	// �����蔻��̃f�[�^��ǂݍ���
	void LoadColData(CharacterBase& data, int charType);
	// �G�̃f�[�^��ǂݍ���
	void LoadEnemyData(CharacterBase& data, int charType);

private:
	std::vector<float> m_charData;		// �L�����N�^�[�̃X�e�[�^�X���
	std::vector<float> m_animSpeedData;	// �A�j���[�V�������x���
	std::vector<float> m_colData;		// �����蔻����
	std::vector<float> m_enemyData;		// �G���
};