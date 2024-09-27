#pragma once
#include <vector>

// �t�H���g�֘A
namespace Font
{
	// �t�H���g�̎��
	enum class FontId
	{
		// �t�H���g�T�C�Y�A����
		kSize96_4,
		kSize64_4,
		kSize50_4,
		kSize50_4_EDGE,
		kSize48_4,
		kSize48_4_EDGE,
		kSize42_4,
		kSize42_4_EDGE,
		kSize40_4,
		kSize36_4_EDGE,
		kSize30_4_EDGE,
		kSize28_4,
		kSize24_4,
		kSize24_4_EDGE,
		kSize16_4,
		kNormal,
		kNum,		// �t�H���g�̎�ސ�

		/*�g���t�H���g���w�肷��*/
		// SceneTitle
		
		/*SceneSelectStage*/
		kSelectStage = kSize48_4,		// �X�e�[�W�I���Ŏg�������T�C�Y
		kIntroductionText = kSize48_4,	// �X�e�[�W�Љ�̕����T�C�Y
		/*SceneOption*/
		kOption = kSize48_4,			// �I�v�V�����Ŏg�p���镶���T�C�Y
		/* ScenePause*/
		kPause = kSize42_4_EDGE,		// �|�[�Y��ʂŎg�p���镶���T�C�Y
		// SceneGameover
		kGameover = kSize48_4_EDGE,		// �Q�[���I�[�o�[�Ŏg�������T�C�Y
		/*SceneClear*/
		kClearTime = kSize42_4_EDGE,	// ���ԕ\���Ŏg�������T�C�Y
		/*�����L���O*/
		kRanking = kSize42_4_EDGE,
		kRankingText = kSize50_4_EDGE,
		/*�E���ɕ\������{�^���\���̕����T�C�Y*/
		kButtonText = kSize42_4_EDGE,
		kTitleButtonText = kSize42_4_EDGE,
		kClearButtonText = kSize36_4_EDGE,
		/*�`���[�g���A��*/ 
		kTutorial = kSize36_4_EDGE,
		kTutoButton = kSize30_4_EDGE,
		/*�L�����N�^�[��*/
		kCharaName = kSize42_4,
		/*�������*/
		kOperation = kSize24_4_EDGE,
		/*�K�E�Z*/
		kSpecialAttack = kSize36_4_EDGE,
	};

	// �t�H���g�̃��[�h�A�A�����[�h
	void Load();
	void UnLoad();

	// �t�H���g�̃n���h��
	// MEMO:extern���g�p���ĈقȂ�\�[�X�t�@�C���ŕϐ������L�ł���悤�ɂ���
	extern std::vector<int> m_fontHandle;
}

