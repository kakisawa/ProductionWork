#include "DxLib.h"
#include "CharacterBase.h"
#include "LoadData.h"
#include <fstream>
#include <sstream>
#include <cassert>

// �萔
namespace
{
	const char* const kCharStatusFileName = "data/csv/charaStatus.csv";	// �L�����N�^�[�f�[�^�̃t�@�C����
	const char* const kAnimSpeedFileName = "data/csv/animSpeed.csv";	// �A�j���[�V�������x�f�[�^�̃t�@�C����
	const char* const kColFileName = "data/csv/collision.csv";			// �����蔻��f�[�^�̃t�@�C����
	const char* const kEnemyInfoFileName = "data/csv/enemyInfo.csv";	// �G�f�[�^�̃t�@�C����

	constexpr int kStatusNum = 20;		// 1�L�����N�^�[�̃X�e�[�^�X��
	constexpr int kAnimNum = 17;		// 1�L�����N�^�[�̃A�j���[�V�����̐�
	constexpr int kColNum = 16;			// 1�L�����N�^�[�̓����蔻��̏��
	constexpr int kEnemyInfoNum = 14;	// 1�L�����N�^�[�̏��
}


/// <summary>
/// �������R���X�g���N�^
/// </summary>
/// <param name="data">�L�����N�^�[�Q��</param>
/// <param name="charType">�L�����N�^�[�̎��</param>
LoadData::LoadData(CharacterBase& data, int charType)
{
	LoadCharaData(data, charType);
	LoadAnimSpeedData(data, charType);
	LoadColData(data, charType);
	if (charType != 0)
	{
		LoadEnemyData(data, charType);
	}

}


/// <summary>
/// �f�X�g���N�^
/// </summary>
LoadData::~LoadData()
{
}


/// <summary>
/// �`��
/// </summary>
void LoadData::Draw()
{
#ifdef _DEBUG
	// �ǂݍ��񂾃f�[�^��\������
	int y = 0;
	for (const auto& data : m_charData)
	{
		DrawFormatString(0, 100 + y, 0xffffff, "%f:", data);
		y += 20;
	}
#endif
}


/// <summary>
/// �L�����N�^�[�̃f�[�^��ǂݍ���
/// </summary>
/// <param name="data">�L�����N�^�[�Q��</param>
/// <param name="charCharType">�L�����N�^�[�̎��</param>
void LoadData::LoadCharaData(CharacterBase& data, int charType)
{
	std::ifstream file(kCharStatusFileName);
	m_charData.clear();
	std::string line;

	// �t�@�C���̓��͎擾
	// std::getline(�ǂݎ��t�@�C���̕ϐ�, ���͕�������i�[����ϐ�);
	while (std::getline(file, line))
	{
		std::istringstream stream(line);
		std::string field;

		// �����񕪊�
		// getline(istringstream�^�̕ϐ�, ����������������i�[����ϐ�, ',�ŕ���')
		while (getline(stream, field, ','))
		{
			try
			{
				// �������float�ɕϊ����Ēǉ�����
				m_charData.push_back(std::stof(field));
			}
			catch (const std::invalid_argument&)
			{
				// �����ȕ�������X�L�b�v
			}
		}
	}

	// �O���t�@�C���̏�������
	data.m_status.maxHp = m_charData[0 + charType * kStatusNum];
	data.m_status.punchReceptionTime = static_cast<int>(m_charData[1 + charType * kStatusNum]);
	data.m_status.punchCoolTime = static_cast<int>(m_charData[2 + charType * kStatusNum]);
	data.m_status.kickCoolTime = static_cast<int>(m_charData[3 + charType * kStatusNum]);
	data.m_status.punchTime = static_cast<int>(m_charData[4 + charType * kStatusNum]);
	data.m_status.kickTime = static_cast<int>(m_charData[5 + charType * kStatusNum]);
	data.m_status.punchPower = m_charData[6 + charType * kStatusNum];
	data.m_status.secondPunchPower = m_charData[7 + charType * kStatusNum];
	data.m_status.thirdPunchPower = m_charData[8 + charType * kStatusNum];
	data.m_status.kickPower = m_charData[9 + charType * kStatusNum];
	data.m_status.specialAttackPower = m_charData[10 + charType * kStatusNum];
	data.m_status.maxMoveSpeed = m_charData[11 + charType * kStatusNum];
	data.m_status.fightWalkSpeed = m_charData[12 + charType * kStatusNum];
	data.m_status.acceleration = m_charData[13 + charType * kStatusNum];
	data.m_status.deceleration = m_charData[14 + charType * kStatusNum];
	data.m_status.avoidDist = m_charData[15 + charType * kStatusNum];
	data.m_status.maxAvoidCount = static_cast<int>(m_charData[16 + charType * kStatusNum]);
	data.m_status.avoidCoolTime = static_cast<int>(m_charData[17 + charType * kStatusNum]);
	data.m_status.guardAnimTime = m_charData[18 + charType * kStatusNum];
	data.m_status.backMove = m_charData[19 + charType * kStatusNum];
}


/// <summary>
/// �A�j���[�V�������x�̃f�[�^��ǂݍ���
/// </summary>
/// <param name="data">�L�����N�^�[�Q��</param>
/// <param name="charType">�L�����N�^�[�̎��</param>
void LoadData::LoadAnimSpeedData(CharacterBase& data, int charType)
{
	std::ifstream file(kAnimSpeedFileName);
	m_animSpeedData.clear();
	std::string line;

	// �t�@�C���̓��͎擾
	// std::getline(�ǂݎ��t�@�C���̕ϐ�, ���͕�������i�[����ϐ�);
	while (std::getline(file, line))
	{
		std::istringstream stream(line);
		std::string field;

		// �����񕪊�
		// getline(istringstream�^�̕ϐ�, ����������������i�[����ϐ�, ',�ŕ���')
		while (getline(stream, field, ','))
		{
			try
			{
				// �������float�ɕϊ�����m_data�ɒǉ�����
				m_animSpeedData.push_back(std::stof(field));
			}
			catch (const std::invalid_argument&)
			{
				// �����ȕ�������X�L�b�v
			}
		}
	}

	// �O���t�@�C���̏�������
	data.m_animSpeed.none = m_animSpeedData[0+ charType * kAnimNum];
	data.m_animSpeed.avoid = m_animSpeedData[1 + charType * kAnimNum];
	data.m_animSpeed.down = m_animSpeedData[2 + charType * kAnimNum];
	data.m_animSpeed.fightIdle = m_animSpeedData[3 + charType * kAnimNum];
	data.m_animSpeed.fightWalk = m_animSpeedData[4 + charType * kAnimNum];
	data.m_animSpeed.gettingUp = m_animSpeedData[5 + charType * kAnimNum];
	data.m_animSpeed.grab = m_animSpeedData[6 + charType * kAnimNum];
	data.m_animSpeed.guard = m_animSpeedData[7 + charType * kAnimNum];
	data.m_animSpeed.kick = m_animSpeedData[8 + charType * kAnimNum];
	data.m_animSpeed.punch1 = m_animSpeedData[9 + charType * kAnimNum];
	data.m_animSpeed.punch2 = m_animSpeedData[10 + charType * kAnimNum];
	data.m_animSpeed.punch3 = m_animSpeedData[11 + charType * kAnimNum];
	data.m_animSpeed.specialAttack = m_animSpeedData[12 + charType * kAnimNum];
	data.m_animSpeed.receive = m_animSpeedData[13 + charType * kAnimNum];
	data.m_animSpeed.run = m_animSpeedData[14 + charType * kAnimNum];
	data.m_animSpeed.stand = m_animSpeedData[15 + charType * kAnimNum];
	data.m_animSpeed.Stumble = m_animSpeedData[16 + charType * kAnimNum];
}


/// <summary>
/// �����蔻�����ǂݍ���
/// </summary>
/// <param name="data">�L�����N�^�[�Q��</param>
/// <param name="charType">�L�����N�^�[�̎��</param>
void LoadData::LoadColData(CharacterBase& data, int charType)
{
	std::ifstream file(kColFileName);
	m_colData.clear();
	std::string line;

	// �t�@�C���̓��͎擾
	// std::getline(�ǂݎ��t�@�C���̕ϐ�, ���͕�������i�[����ϐ�);
	while (std::getline(file, line))
	{
		std::istringstream stream(line);
		std::string field;

		// �����񕪊�
		// getline(istringstream�^�̕ϐ�, ����������������i�[����ϐ�, ',�ŕ���')
		while (getline(stream, field, ','))
		{
			try
			{
				// �������float�ɕϊ�����m_data�ɒǉ�����
				m_colData.push_back(std::stof(field));
			}
			catch (const std::invalid_argument&)
			{
				// �����ȕ�������X�L�b�v
			}
		}
	}

	// �O���t�@�C���̏�������
	data.m_colInfo.bodyHeight = m_colData[0 + charType * kColNum];
	data.m_colInfo.bodyRadius = m_colData[1 + charType * kColNum];
	data.m_colInfo.aimRadius = m_colData[2 + charType * kColNum];
	data.m_colInfo.legRadius = m_colData[3 + charType * kColNum];
	data.m_colInfo.armStartPos.x = m_colData[4 + charType * kColNum];
	data.m_colInfo.armStartPos.y = m_colData[5 + charType * kColNum];
	data.m_colInfo.armStartPos.z = m_colData[6 + charType * kColNum];
	data.m_colInfo.armEndPos.x = m_colData[7 + charType * kColNum];
	data.m_colInfo.armEndPos.y = m_colData[8 + charType * kColNum];
	data.m_colInfo.armEndPos.z = m_colData[9 + charType * kColNum];
	data.m_colInfo.legStartPos.x = m_colData[10 + charType * kColNum];
	data.m_colInfo.legStartPos.y = m_colData[11 + charType * kColNum];
	data.m_colInfo.legStartPos.z = m_colData[12 + charType * kColNum];
	data.m_colInfo.legEndPos.x = m_colData[13 + charType * kColNum];
	data.m_colInfo.legEndPos.y = m_colData[14 + charType * kColNum];
	data.m_colInfo.legEndPos.z = m_colData[15 + charType * kColNum];
}


/// <summary>
/// �G����ǂݍ���
/// </summary>
/// <param name="data">�G�Q��</param>
void LoadData::LoadEnemyData(CharacterBase& data, int charType)
{
	std::ifstream file(kEnemyInfoFileName);
	m_enemyData.clear();
	std::string line;

	// �t�@�C���̓��͎擾
	// std::getline(�ǂݎ��t�@�C���̕ϐ�, ���͕�������i�[����ϐ�);
	while (std::getline(file, line))
	{
		std::istringstream stream(line);
		std::string field;

		// �����񕪊�
		// getline(istringstream�^�̕ϐ�, ����������������i�[����ϐ�, ',�ŕ���')
		while (getline(stream, field, ','))
		{
			try
			{
				// �������float�ɕϊ�����m_data�ɒǉ�����
				m_enemyData.push_back(std::stof(field));
			
			}
			catch (const std::invalid_argument&)
			{
				// �����ȕ�������X�L�b�v
			}
		}
	}

	// �O���t�@�C���̏�������
	// MEMO:�G�ԍ���1�Ԃ���n�܂邽�߁AcharType-1������
	data.m_enemyInfo.approachRange = m_enemyData[0 + (charType - 1) * kEnemyInfoNum];
	data.m_enemyInfo.attackRange = m_enemyData[1 + (charType - 1) * kEnemyInfoNum];
	data.m_enemyInfo.minStopTime = static_cast<int>(m_enemyData[2 + (charType - 1) * kEnemyInfoNum]);
	data.m_enemyInfo.maxStopTime = static_cast<int>(m_enemyData[3 + (charType - 1) * kEnemyInfoNum]);
	data.m_enemyInfo.maxProb = static_cast<int>(m_enemyData[4 + (charType - 1) * kEnemyInfoNum]);
	data.m_enemyInfo.punchProb = static_cast<int>(m_enemyData[5 + (charType - 1) * kEnemyInfoNum]);
	data.m_enemyInfo.kickProb = static_cast<int>(m_enemyData[6 + (charType - 1) * kEnemyInfoNum]);
	data.m_enemyInfo.avoidProb = static_cast<int>(m_enemyData[7 + (charType - 1) * kEnemyInfoNum]);
	data.m_enemyInfo.guardProb = static_cast<int>(m_enemyData[8 + (charType - 1) * kEnemyInfoNum]);
	data.m_enemyInfo.grabProb = static_cast<int>(m_enemyData[9 + (charType - 1) * kEnemyInfoNum]);
	data.m_enemyInfo.changeAngleProb = static_cast<int>(m_enemyData[10 + (charType - 1) * kEnemyInfoNum]);
	data.m_enemyInfo.changeAngleFrame = static_cast<int>(m_enemyData[11 + (charType - 1) * kEnemyInfoNum]);
	data.m_enemyInfo.guardTime = static_cast<int>(m_enemyData[12 + (charType - 1) * kEnemyInfoNum]);
	data.m_enemyInfo.stateIntervalTime = static_cast<int>(m_enemyData[13 + (charType - 1) * kEnemyInfoNum]);
}
