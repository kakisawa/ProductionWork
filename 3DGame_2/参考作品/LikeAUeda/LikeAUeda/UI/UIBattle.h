#pragma once

/// <summary>
/// �o�g�����ɕ\������UI
/// </summary>
class UIBattle
{
public:
	UIBattle();
	~UIBattle();
	void Init();
	void Update();
	void DrawPlayerHP(float currentHp, float MaxHp);			// �v���C���[��HP�o�[��\��
	void DrawPlayerGauge(float currentGauge, float MaxGauge);	// �v���C���[�̃Q�[�W�o�[��\��
};

