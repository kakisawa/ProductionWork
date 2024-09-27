#pragma once
#include "UI.h"

/// <summary>
/// �o�g�����ɕ\������UI���Ǘ�����
/// </summary>
class UIBattle : public UI
{
public:
	UIBattle() = delete;
	UIBattle(float maxHp, int charType);
	virtual ~UIBattle();
	void UpdateHpBar();												 // HP�\�����X�V����
	void SetDamageTimer();											 // �_���[�W���󂯂��ۂɃ^�C�}�[���Z�b�g����
	void OnDamage(float damage);									 // �󂯂��_���[�W�ʂ��v�Z����
	void ResetStartProduction();									 // �X�^�[�g���o�����Z�b�g����
	void DrawStartProduction(int time, int matchNum, int maxMatch);  // �X�^�[�g���o��\��
	void ResetClearProduction();									 // �N���A���o�����Z�b�g����
	void DrawClearProduction(int time);								 // �N���A���o��\��
	void DrawGameoverProduciton();									 // �Q�[���I�[�o�[���o��\��
	void DrawPlayerName();											 // �v���C���[�̖��O��\��
	void DrawPlayerHP(float currentHp);								 // �v���C���[��HP�o�[��\��
	void DrawPlayerGauge(float currentGauge, float MaxGauge);		 // �v���C���[�̃Q�[�W�o�[��\��
	void DrawEnemyName(int charType);								 // �G�̖��O��\��
	void DrawEnemyHp(float currentHp);								 // �G��HP�o�[��\��
	void DrawPlayerSilhouette();									 // �V���G�b�g��\��
	void DrawEnemySilhouette(int charType);							 // �V���G�b�g��\��
	void ResetSpecialAttack();										 // �K�E�Z�̕\�������Z�b�g����
	void DrawSpecialAttack(float currentGauge, float maxGauge);		 // �K�E�Z�̃e�L�X�g�\��
	void DrawOperation();											 // ���������\��
	void DrawTutoButtonText();										 // �`���[�g���A����ʂ̃{�^���\��

	void SetEnemyKind(int enmeyKind){m_currentEnemy = enmeyKind; }   // ���݂̓G���Z�b�g����

private:
	float m_decreaseHp;				// �_���[�W���󂯂����HP
	float m_currentHp;				// ���݂�HP
	float m_maxHp;					// �ő�HP
	float m_damage;					// �󂯂��_���[�W��
	int m_intervalTime;				// HP�o�[����������܂ł̎���
	int m_currentEnemy;				// ���݂̓G
	float m_enemyNameScale;			// �G���O�̊g�嗦
	float m_gekihaTextScale;		// "���j"�e�L�X�g�̊g�嗦
	float m_specialTextScale;		// "�K�E�Z"�e�L�X�g�̊g�嗦
	std::vector<int> m_handle;		// �n���h��

	// �摜�̎��
	enum HandleKind
	{
		kTutoName = 1,	 // �`���[�g���A���G�̖��O
		kNinjaName = 2,	 // �E�҂̖��O
		kCiefName = 3,	 // �V�F�t�̖��O
		kOldManName = 4, // �V�l�̖��O
		kNameBack,	     // ���O�o�[
		kGaugeBar,		 // �Q�[�W�o�[
		kSilhouette,	 // �L�����N�^�[�̃V���G�b�g
		kFightText,		 // "Fight!"�̃e�L�X�g
		kGekihaText,	 // "���j"�̃e�L�X�g
		kClearBg,		 // �N���A���̔w�i
		kGameoverBg,	 // �Q�[���I�[�o�[���̔w�i
		kNumText,		 // ����
		kHandleNum		 // �n���h���̎��
	};

	// ��������ŕ\�����鏇��
	enum OperationOrder
	{
		kMove,			// �ړ�
		kCameraMove,	// �J�����ړ�
		kPunch,			// �p���`
		kKick,			// �L�b�N
		kSpecialAttack, // �K�E�Z
		kAvoid,			// ���
		kGuard,			// �K�[�h
		kRockon,		// ���b�N�I��
		kPause,			// �|�[�Y
	};
};
