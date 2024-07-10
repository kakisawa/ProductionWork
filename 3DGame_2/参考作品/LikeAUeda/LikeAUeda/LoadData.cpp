#include "DxLib.h"
#include "LoadData.h"
#include <fstream>
#include <sstream>

// �萔
namespace
{
	const char* const kFileName = "data/data.csv";
}


/// <summary>
/// �R���X�g���N�^
/// </summary>
LoadData::LoadData()
{
	std::ifstream file(kFileName);
	m_data.clear();
	std::string line;

	// �t�@�C���̓��͎擾
	// std::getline(�ǂݎ��t�@�C���̕ϐ�, ���͕�������i�[����ϐ�);
	while (std::getline(file, line))
	{
		std::istringstream stream(line);
		std::string field;

		// �����񕪊�
		// getline(istringstream�^�̕ϐ�, ����������������i�[����ϐ�, '���ŕ������邩')
		while (getline(stream, field, ','))
		{
			// �������float�ɕϊ�����m_data�ɒǉ�����
			try
			{
				m_data.push_back(std::stof(field));
			}
			catch (const std::invalid_argument& e)
			{
				// �����ȕ�������X�L�b�v
			}
		}
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
	for (const auto& data : m_data)
	{
		DrawFormatString(0, y, 0xffffff, "%f:", data);
		y += 20;
	}
#endif

}
