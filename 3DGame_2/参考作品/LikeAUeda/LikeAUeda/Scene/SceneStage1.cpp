#include "DxLib.h"
#include "Player.h"
#include "Camera.h"
#include "Input.h"
#include "Stage.h"
#include "Shader.h"
#include "SceneClear.h"
#include "SceneGameover.h"
#include "SceneStage1.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneStage1::SceneStage1(std::shared_ptr<Player> pPlayer, std::shared_ptr<Camera> pCamera, std::shared_ptr<Stage> pStage):
	m_pPlayer(pPlayer),
	m_pCamera(pCamera),
	m_pStage(pStage)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneStage1::~SceneStage1()
{
	m_pShader->UnLoad();
}


/// <summary>
/// ������
/// </summary>
void SceneStage1::Init()
{
	//m_pShader->Init();
	m_pPlayer->Init(m_pShader);
	m_pCamera->Init();
}


/// <summary>
/// �X�V
/// </summary>
/// <param name="input">���͏��</param>
/// <returns></returns>
std::shared_ptr<SceneBase> SceneStage1::Update(Input& input)
{
#ifdef _DEBUG	// �f�o�b�O���[�h
	// P�L�[�Ń|�[�Y�A�|�[�Y����P�ŃR�}����
	if (m_debugState == DebugState::Normal && input.IsTriggered("debug_pause"))
	{
		m_debugState = DebugState::Pause;
	}
	if (m_debugState == DebugState::Pause && input.IsTriggered("debug_enter"))
	{
		m_debugState = DebugState::Normal;
	}
	if (m_debugState != DebugState::Pause || input.IsTriggered("debug_pause"))
#endif
	{
		// �V�[���J��
		if (input.IsTriggered("debug_clear"))
		{
			return std::make_shared<SceneClear>();
		}
		else if (input.IsTriggered("debug_gameover"))
		{
			return std::make_shared<SceneGameover>();
		}

		//m_pShader->Update();
		m_pPlayer->Update(input, *m_pCamera, *m_pStage);
		m_pCamera->Update(input, *m_pPlayer);
	}

	return shared_from_this();	// ���g��shared_ptr��Ԃ�
}


/// <summary>
/// �`��
/// </summary>
void SceneStage1::Draw()
{
	// �V�F�[�_�̕`��
	//m_pShader->Draw();
	// �X�e�[�W�`��
	m_pStage->Draw();
	// �v���C���[�`��
	m_pPlayer->Draw();


#ifdef _DEBUG	// �f�o�b�O�\��
	// ���݂̃V�[��
	DrawString(0, 0, "�X�e�[�W1", 0xffffff);
#endif
}