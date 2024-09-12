#include "SceneGame.h"
#include "SceneGameClear.h"
#include "SceneGameOver.h"
#include "SceneTitle.h"
#include "../Object/GameMap.h"
#include "../Object/Camera.h"
#include "../Object/Player/Player.h"
#include "../Object/Enemy/EnemyRight.h"
#include "../Object/Enemy/EnemyLeft.h"
#include "../Util/Pad.h"
#include "../Util/Fade.h"
#include <cassert>

namespace {
	constexpr int kExplanationInitPosX = 620;	// ��������摜�̏����ʒuX
	constexpr int kExplanationInitPosY = 130;	// ��������摜�̏����ʒuY
	constexpr int kExplanationPosX = 1472;		// ��������摜�̍ŏI�ʒuX
	constexpr int kExplanationPosY = 0;			// ��������摜�̍ŏI�ʒuY

	constexpr int kExplanationSizeX = 448;		// ��������摜�̃T�C�YX
	constexpr int kExplanationSizeY = 530;		// ��������摜�̃T�C�YY
	constexpr int kExplanationAddWidth = 16;	// ��������摜��X���ړ���
	constexpr int kExplanationSubHeight = 9;	// ��������摜��Y���ړ���

	constexpr int kGameFinishCount = 60;		// �N���A������A�V�[���J�ڂւ̑ҋ@����
}

SceneGame::SceneGame() :
	m_explanationGraph(-1),
	m_waitCount(kGameFinishCount),
	m_isGameClearFlag(false),
	m_isExplanationFinishFlag(false),
	m_isExplanationSet(false),
	m_isExplanationView(true),
	m_isPause(false)
{
	// �t�F�[�h
	m_pFade = std::make_shared<Fade>();
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	// ����������
	m_pMap->Init();
	m_pPlayer->Init(m_pMap);
	m_pEnemyRight->Init(m_pMap);
	m_pEnemyLeft->Init(m_pMap);
	m_pCamera->Init();
	m_pSound->Init();	// �T�E���h�̏�����

	// �g�p�T�E���h�̃��[�h
	m_pSound->LoadBGM(SoundManager::BGM_Type::kGameBGM);	// �T�E���h�̓ǂݍ���
	m_pSound->PlayBGM(SoundManager::BGM_Type::kGameBGM, DX_PLAYTYPE_LOOP);

	// �摜�̃��[�h
	m_explanationGraph = LoadGraph("data/Image/GameScene/UI/�������.png");

	// ��������摜�̈ʒu�ݒ�
	size.m_width = kExplanationInitPosX;
	size.m_height = kExplanationInitPosY;
	size.m_widthMax = kExplanationInitPosX + 800;
	size.m_heightMax = kExplanationInitPosY + 800;
}

shared_ptr<SceneBase> SceneGame::Update()
{
	// �X�^�[�g�{�^������������|�[�Y��Ԃɂ���
	if (Pad::IsTrigger(PAD_INPUT_R)) {	
		if (m_isPause) {
			m_isPause = false;
		}
		else {
			m_isPause = true;
		}
	}

	// �|�[�Y���Ƀo�b�N�{�^���������ƃ^�C�g����ʂɖ߂�
	if (Pad::IsTrigger(PAD_INPUT_L) && m_isPause) {
		return make_shared<SceneTitle>();	// �Q�[���N���A�֍s��
		
	}

	// �V�[���ɓ���������Ƀt�F�[�h�C�����s��
	m_pFade->FadeIn(m_pFade->GatFadeInFlag());

	// �|�[�Y��ԂɂȂ��Ă��Ȃ��ꍇ�̂݃Q�[����ʂ��X�V����
	if (!m_isPause) {
		m_pCamera->Update(*m_pPlayer);

		// ��������摜�̏Љ�I�������X�V���s��
		if (m_isExplanationFinishFlag) {
			// �V�[���؂�ւ������ɓ��Ă͂܂�����t�F�[�h�A�E�g���s��
			m_pFade->FadeOut(m_pFade->GatFadeOutFlag());	
			m_isNextSceneFlag = m_pFade->GatNextSceneFlag();

			m_pPlayer->Update(*m_pCamera, *m_pEnemyRight, *m_pEnemyLeft);
			m_pEnemyRight->Update(*m_pPlayer);
			m_pEnemyLeft->Update(*m_pPlayer);

			// �Q�[���N���A�֌W
			m_isGameClearFlag = m_pEnemyLeft->GetHp() <= 0 && m_pEnemyRight->GetHp() <= 0;	// �Q�[���N���A�����t���O

			if (m_isGameClearFlag){						// �Q�[���N���A������B��������
				m_waitCount--;							// �����ɃV�[���J�ڂ������Am_waitCount���҂�
				if (m_waitCount <= 0) {					// �J�E���g���[���ɂȂ�����t�F�[�h�A�E�g���s���t���O�𗧂Ă�
					m_pFade->SetFadeOutFlag(true);
					if (m_isNextSceneFlag){				// �t�F�[�h�A�E�g���I�������A�Q�[���N���A��ʂɑJ�ڂ���
						return make_shared<SceneGameClear>();
					}
				}
			}

			// �Q�[���I�[�o�[�֌W
			if (m_pPlayer->GetDeathFlag())
			{
				m_pFade->SetFadeOutFlag(true);			// �t�F�[�h�A�E�g���s���t���O�𗧂Ă�
				if (m_isNextSceneFlag){					// �t�F�[�h�A�E�g���I�������A�Q�[���I�[�o�[��ʂɑJ�ڂ���
					return make_shared<SceneGameOver>();
				}
			}

			// X�{�^������������A��������摜�̕\����ON/OFF�؂�ւ��
			if (Pad::IsTrigger(PAD_INPUT_C))
			{
				if (m_isExplanationView) {
					m_isExplanationView = false;
				}
				else {
					m_isExplanationView = true;
				}
			}
		}
		else {	// �Q�[���J�n�O����
			// A�{�^������������A��������摜������̈ʒu�ֈړ�����t���O�𗧂Ă�
			if (Pad::IsTrigger(PAD_INPUT_A)) {	
				m_isExplanationSet = true;
			}
			
			// ��������摜������̈ʒu�Ɉړ�������A�Q�[���J�n�O�������I������
			if (size.m_height <= 0 && size.m_width >= kExplanationPosX){
				m_isExplanationFinishFlag = true;
			}

			// ��������摜������̈ʒu�ֈړ�������
			if (m_isExplanationSet){
				size.m_width = min(size.m_width, kExplanationPosX);
				size.m_widthMax = min(size.m_widthMax, 1920);
				size.m_height = max(size.m_height, 0);
				size.m_heightMax = max(size.m_heightMax, kExplanationSizeY);

				size.m_height -=kExplanationSubHeight;
				size.m_heightMax -= kExplanationSubHeight;
				size.m_width += kExplanationAddWidth;
				size.m_widthMax += kExplanationAddWidth;
			}
		}
	}

#ifdef _DEBUG
	m_pCamera->Update(*m_pPlayer);

	if (Pad::IsTrigger(PAD_INPUT_L))		// LB�{�^������������
	{
		return make_shared<SceneGameClear>();	// �Q�[���N���A�֍s��
	}
	//if (Pad::IsTrigger(PAD_INPUT_R))		// RB�{�^������������
	//{
	//	return make_shared<SceneGameOver>();	// �Q�[���I�[�o�[�֍s��
	//}

#endif // _DEBUG

	return shared_from_this();
}

void SceneGame::Draw()
{
	m_pMap->Draw();
	m_pEnemyRight->Draw();
	m_pEnemyLeft->Draw();
	m_pPlayer->Draw();

#ifdef DEBUG
	m_pEnemyLeft->UIDraw();
	m_pEnemyRight->UIDraw();
#endif

	// ��������摜�̕`��
	if (m_isExplanationView) {
		DrawExtendGraph(size.m_width, size.m_height,
			size.m_widthMax, size.m_heightMax,
			m_explanationGraph, true);
	}
	
	m_pFade->Draw();

#ifdef _DEBUG
	DrawString(0, 120, "SceneGame", 0xffffff);
	DrawString(0, 140, "Please Press Button RB or LB", 0x00ffff);	
#endif
}

void SceneGame::End()
{
	m_pMap->End();
	m_pPlayer->End();
	m_pEnemyRight->End();
	m_pEnemyLeft->End();

	DeleteGraph(m_explanationGraph);
	SceneBase::End();
}