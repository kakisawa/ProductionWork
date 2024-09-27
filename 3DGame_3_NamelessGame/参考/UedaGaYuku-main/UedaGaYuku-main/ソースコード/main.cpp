#include "DxLib.h"
#include "EffekseerForDXLib.h"
#include "Input.h"
#include "Game.h"
#include "Font.h"
#include "Sound.h"
#include "SceneManager.h"
#include "Ranking.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetMainWindowText("Ueda������");

	// window���[�h�ݒ�
	ChangeWindowMode(true);
	// ��ʃT�C�Y�ύX
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);

	if (DxLib_Init() == -1)	// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	// Effekseer�̏�����
	Effekseer_Init(8000);

	// �t���X�N���[���؂�ւ����ɂ��������Ȃ�Ȃ��悤��
	SetChangeScreenModeGraphicsSystemResetFlag(false);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// Z�o�b�t�@�̐ݒ�
	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);
	SetUseBackCulling(true);

	// �_�u���o�b�t�@���[�h
	SetDrawScreen(DX_SCREEN_BACK);

	// ���͏�Ԃ��擾
	Input input;
	Font::Load();		// �t�H���g�̓ǂݍ���
	Sound::Load();		// �T�E���h�̓ǂݍ���

	// SceneManager�𐶐�
	std::shared_ptr<SceneManager> pScene = std::make_shared<SceneManager>();
	pScene->Init();

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// ��ʂ̃N���A
		ClearDrawScreen();

		// �X�V
		input.Update();
		pScene->Update(input);

		// �`��
		pScene->Draw();

		//����ʂ�\��ʂ����ւ���
		ScreenFlip();

		// fps��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}


#ifdef _DEBUG
		// �f�o�b�O����ESC�L�[�ŏI���ł���悤�ɂ���
		if (input.IsTriggered("end"))
		{
			Font::UnLoad();
			Sound::UnLode();
			Effkseer_End();
			DxLib_End();
		}
#endif
	}

	Font::UnLoad();	 // �t�H���g�̉��
	Sound::UnLode(); // �T�E���h�̉��
	Effkseer_End();	 // Effekseer�̏I������
	DxLib_End();	 // Dx���C�u�����g�p�̏I������

	return 0;		// �\�t�g�̏I�� 
}