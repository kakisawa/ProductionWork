#include "DxLib.h"
#include "Scene/SceneManager.h"

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �ꕔ�̊֐���DxLib_Init()�̑O�Ɏ��s����K�v������
	ChangeWindowMode(true);
	// �E�C���h�E���ݒ�
	//SetMainWindowText("Switching! S&B fight");

	/*int x, y;
	GetWindowSize(&x, &y);*/
	// ��ʃT�C�Y�̐ݒ�
	SetGraphMode(1920, 1080, 32);
	SetWindowSize(1920, 1080);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	// Z�o�b�t�@�̐ݒ�
	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);
	SetUseBackCulling(true);

	SetDrawScreen(DX_SCREEN_BACK);

	// �V�[���Ǘ�
	std::shared_ptr<SceneManager> m_pScene = std::make_shared<SceneManager>();
	m_pScene->Init();

	// �Q�[�����[�v
	while (ProcessMessage() != -1)
	{
		// ���̃t���[���̊J�n�������o���Ă���
		LONGLONG start = GetNowHiPerformanceCount();

		// �`����s���O�ɉ�ʂ��N���A����
		ClearDrawScreen();

		// �Q�[���̏���
		m_pScene->Update();
		m_pScene->Draw();
		

		// ��ʂ��؂�ւ��̂�҂�
		ScreenFlip();

		// esc�L�[�ŃQ�[���I��
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}

		// FPS60�ɌŒ肷��
		while (GetNowHiPerformanceCount() - start < 16667)
		{
			// 16.66�~���b(16667�}�C�N���b)�o�߂���܂ő҂�
		}
	}

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}