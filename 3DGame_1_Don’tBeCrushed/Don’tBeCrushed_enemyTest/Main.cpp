#include "DxLib.h"
#include "Scene/SceneGame.h"
#include "Game.h"
#include <cmath>
#include <memory>

using namespace std;

namespace {

	const char* const kBg = "data/floor2.png";

	// �J�������
	constexpr float kCameraDist = 3.5f;
	constexpr float kCameraHeight = 10;

	constexpr float kCameraNear = 0.5f;
	constexpr float kCameraFar = 180.0f;
}

void DrawGrid()
{
	for (int x = -30; x <= 30; x += 10)
	{
		DrawLine3D(VGet(static_cast<float>(x), 0, -20), VGet(static_cast<float>(x), 0, 20), 0xffffff);
	}
	for (int z = -20; z <= 20; z += 10)
	{
		DrawLine3D(VGet(-30, 0, static_cast<float>(z)), VGet(30, 0, static_cast<float>(z)), 0xffffff);
	}

	// X+-,Z+-�̕�����������₷���悤�ɕ\����ǉ�����
	VECTOR dispPos = ConvWorldPosToScreenPos(VGet(2, 0, 0));
	if (dispPos.z >= 0.0f && dispPos.z <= 1.0f)
	{
		DrawStringF(dispPos.x, dispPos.y, "X+", 0xffffff);
	}
	dispPos = ConvWorldPosToScreenPos(VGet(-2, 0, 0));
	if (dispPos.z >= 0.0f && dispPos.z <= 1.0f)
	{
		DrawStringF(dispPos.x, dispPos.y, "X-", 0xffffff);
	}

	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, 2));
	if (dispPos.z >= 0.0f && dispPos.z <= 1.0f)
	{
		DrawStringF(dispPos.x, dispPos.y, "Z+", 0xffffff);
	}
	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, -2));
	if (dispPos.z >= 0.0f && dispPos.z <= 1.0f)
	{
		DrawStringF(dispPos.x, dispPos.y, "Z-", 0xffffff);
	}
}

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �ꕔ�̊֐���DxLib_Init()�̑O�Ɏ��s����K�v������
	ChangeWindowMode(true);
	SetGraphMode(kScreenWidth, kScreenHeight - 76, kColorDepth);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	SetDrawScreen(DX_SCREEN_BACK);

	// �V�[���Ǘ�
	std::shared_ptr<SceneGame> m_pScene = make_shared<SceneGame>();
	m_pScene->Init();

	int bgGraph = LoadGraph(kBg);

	// �J�������
	float cameraAngle = -DX_PI_F / 2;

	// �Q�[�����[�v
	while (ProcessMessage() != -1)
	{
		// ���̃t���[���̊J�n�������o���Ă���
		LONGLONG start = GetNowHiPerformanceCount();

		// �`����s���O�ɉ�ʂ��N���A����
		ClearDrawScreen();

		DrawExtendGraph(0, 0, kScreenWidth, kScreenHeight, bgGraph, false);
		DrawGrid();


		// �Q�[���̏���
		m_pScene->Update();
		m_pScene->Draw();

		

		// �J����
		SetCameraNearFar(kCameraNear, kCameraFar);
		VECTOR cameraPos;
		cameraPos.x = cosf(cameraAngle) * kCameraDist;
		cameraPos.y = kCameraHeight;
		cameraPos.z = sinf(cameraAngle) * kCameraDist;
		cameraPos = VAdd(cameraPos, VGet(0, 50.0f, 0));

		SetCameraPositionAndTarget_UpVecY(cameraPos, VGet(0.0f, 2.0f, 0.0f));

		ChangeLightTypeDir(VGet(0.0f, -10.0f, 0.0f));

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