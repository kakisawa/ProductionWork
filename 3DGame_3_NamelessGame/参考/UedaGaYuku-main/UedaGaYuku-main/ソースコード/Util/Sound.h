#pragma once
#include <vector>

/// <summary>
/// �T�E���h�Ǘ�
/// </summary>
namespace Sound
{
	//BGM�̎��
	enum class BgmKind
	{
		kTitle,			// �^�C�g��BGM
		kStageSelect,	// �X�e�[�W�I��BGM
		kStage1,		// �X�e�[�W1BGM
		kStage2,		// �X�e�[�W2BGM
		kClear,			// �N���ABGM
		kGameover,		// �Q�[���I�[�o�[BGM
		kBgmNum			// BGM�̐�
	};

	// SE�̎��
	enum class SeKind
	{
		kTitleDisp,			// �^�C�g���\��
		kCursor,			// �J�[�\���ړ�
		kSelect,			// ����
		kWalk,				// ����
		kRun,				// ����
		kAttack,			// �U��
		kGuardAttack,		// �K�[�h���U��
		kAccumulateGauge,	// �K�E�Z�Q�[�W�����܂�����
		kSpecialAttack,		// �K�E�Z
		kAvoid,				// ���
		kBattleStart,		// �o�g���J�n��
		kLastAttack,		// �Ō�ɍU��������
		kBattleEnd,			// �o�g���I����
		kClearCheers,		// �N���A���̊���
		kSeNum				// SE�̐�
	};

	void Load();
	void UnLode();
	void ChangeBgmVol(Input& input); // BGM�̉��ʂ�ύX����
	void ChangeSeVol(Input& input);  // SE�̉��ʂ�ύX����
	int GetBgmVol();				 // BGM�̉��ʂ��擾
	int GetSeVol();					 // SE�̉��ʂ��擾

	extern std::vector<int> m_bgmHandle;
	extern std::vector<int> m_seHandle;
};

