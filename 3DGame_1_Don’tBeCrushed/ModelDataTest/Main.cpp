#include "DxLib.h"

namespace {
	const char* const kModelEnemy = "data/model/book.mv1";
	const int size = 300;
}

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �ꕔ�̊֐���DxLib_Init()�̑O�Ɏ��s����K�v������
	ChangeWindowMode(true);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	SetDrawScreen(DX_SCREEN_BACK);

	VECTOR StartPos, EndPos;
	MV1_COLL_RESULT_POLY HitPoly;

	int m_model;
	m_model = MV1LoadModel(kModelEnemy);
	MV1SetPosition(m_model, VGet(300,300,0));
	MV1SetScale(m_model, VGet(size, size, size));

	MV1SetupCollInfo(m_model, -1, 8, 8, 8);

	// �Q�[�����[�v
	while (ProcessMessage() != -1)
	{
		// ���̃t���[���̊J�n�������o���Ă���
		LONGLONG start = GetNowHiPerformanceCount();

		// �`����s���O�ɉ�ʂ��N���A����
		ClearDrawScreen();

		// �Q�[���̏���

		MV1DrawModel(m_model);

		StartPos = VGet(250.0f, 300.0f, 600.0f);
		EndPos = VGet(1000.0f, 300.0f, 600.0f);
		HitPoly = MV1CollCheck_Line(m_model, -1, StartPos, EndPos);


		DrawLine3D(StartPos, EndPos, GetColor(255, 255, 0));

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

	MV1DeleteModel(m_model);
	m_model = -1;

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}