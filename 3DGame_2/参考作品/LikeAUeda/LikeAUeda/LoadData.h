#pragma once
#include <vector>

/// <summary>
/// �O���t�@�C���ǂݍ���
/// </summary>
class LoadData
{
public:
	LoadData();
	~LoadData();
	void Draw();

	struct PlayerData
	{
		float maxHP;		// �ő�HP
		float maxHeatGauge;	// �ő�q�[�g�Q�[�W��
		float attack;		// �U����
		float maxMoveSpeed; // �ő�ړ����x
		float acceleration; // �����x
		float deceleration;	// �����x
	};

private:
	std::vector<float> m_data;
};