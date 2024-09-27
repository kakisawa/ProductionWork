#include "DxLib.h"
#include "EffekseerForDXLib.h"

#include "Scene/SceneManager.h"
#include "scene/LoadingScene.h"

#include "util/game.h"
#include "util/InputState.h"
#include "util/SoundManager.h"

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//window���[�h�̐ݒ�
	ChangeWindowMode(Game::window_mode);

	//��ʃ��[�h�ύX���O���t�B�b�N�V�X�e����O���t�B�b�N�n���h�������Z�b�g���邩
	//(true�F����Afalse�F���Ȃ�)
	SetChangeScreenModeGraphicsSystemResetFlag(false);

	//window���ݒ�
	SetMainWindowText(Game::title_text);

	//��ʃT�C�Y�̐ݒ�
	SetGraphMode(Game::screen_width,Game::screen_height, Game::screen_depth);

	//�T�E���h�̃C���X�^���X����
	SoundManager::GetInstance();

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	//�_�u���o�b�t�@���[�h
	SetDrawScreen(DX_SCREEN_BACK);

	SceneManager manager;

	manager.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<LoadingScene>(manager)));

	while (ProcessMessage() == 0) {

		LONGLONG time = GetNowHiPerformanceCount();

		Effekseer_Sync3DSetting();

		//��ʂ̃N���A
		ClearDrawScreen();

		InputState::GetInstance().Update();

		manager.Update();
		manager.Draw();

		auto fps = GetFPS();
		auto DC = GetDrawCallCount();

#ifdef _DEBUG
		DrawFormatString(Game::screen_width - 200, 0, 0x448844, "FPS : %2.2f", fps);
		DrawFormatString(Game::screen_width - 200, 16, 0x448844, "DC : %d", DC);
#endif

		//����ʂ�\��ʂɐ؂�ւ���
		ScreenFlip();

		//esc����������I������
		if (CheckHitKey(KEY_INPUT_ESCAPE))break;

		//fps��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667) {

		}

		if (manager.End()) {
			break;
		}

	}

	//EffekseerManager::GetInstance().End();

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}