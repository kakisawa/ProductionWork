#include "DxLib.h"
#include "Game.h"
#include <cmath>

namespace {
	const char* const kModelPlayer = "data/model/Player.mv1";

	// �J�������
	constexpr float kCameraDist = 3.5f;
	constexpr float kCameraHeight = 2.2f;

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
		DrawString(dispPos.x, dispPos.y, "X+", 0xffffff);
	}
	dispPos = ConvWorldPosToScreenPos(VGet(-2, 0, 0));
	if (dispPos.z >= 0.0f && dispPos.z <= 1.0f)
	{
		DrawString(dispPos.x, dispPos.y, "X-", 0xffffff);
	}

	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, 2));
	if (dispPos.z >= 0.0f && dispPos.z <= 1.0f)
	{
		DrawString(dispPos.x, dispPos.y, "Z+", 0xffffff);
	}
	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, -2));
	if (dispPos.z >= 0.0f && dispPos.z <= 1.0f)
	{
		DrawString(dispPos.x, dispPos.y, "Z-", 0xffffff);
	}
}

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �ꕔ�̊֐���DxLib_Init()�̑O�Ɏ��s����K�v������
	ChangeWindowMode(false);
	SetGraphMode(kScreenWidth, kScreenHeight, kColorDepth);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	SetDrawScreen(DX_SCREEN_BACK);

	// ���f��
	int PlayerModel = MV1LoadModel(kModelPlayer);
	int PlayerScele = 3;
	MV1SetScale(PlayerModel, VGet(PlayerScele, PlayerScele, PlayerScele));

	VECTOR m_pos;
	m_pos = VGet(0.0f, 1.0f, 0.0f);
	MV1SetPosition(PlayerModel, m_pos);
	

	// �J�������
	float cameraAngle = -DX_PI_F / 2;

	// �Q�[�����[�v
	while (ProcessMessage() != -1)
	{
		// ���̃t���[���̊J�n�������o���Ă���
		LONGLONG start = GetNowHiPerformanceCount();

		// �`����s���O�ɉ�ʂ��N���A����
		ClearDrawScreen();

		// �Q�[���̏���


		// ���f��
		MV1SetPosition(PlayerModel, m_pos);
		MV1DrawModel(PlayerModel);

		int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);

		float speed = 0.5f;
		if (pad &PAD_INPUT_RIGHT)
		{
			m_pos = VAdd(m_pos, VGet(speed, 0.0f, 0.0f));
		}
		if (pad & PAD_INPUT_LEFT)
		{
			m_pos = VAdd(m_pos, VGet(-speed, 0.0f, 0.0f));
		}
		if (pad & PAD_INPUT_UP)
		{
			m_pos = VAdd(m_pos, VGet(0.0f, 0.0f, speed));
		}
		if (pad & PAD_INPUT_DOWN)
		{
			m_pos = VAdd(m_pos, VGet(0.0f, 0.0f, -speed));
		}

		// ���K��
		
		/*VECTOR m_dir = VNorm(m_pos);
		m_pos = VAdd(m_pos, m_dir);*/

		DrawGrid();

		// �J����
		SetCameraNearFar(kCameraNear, kCameraFar);
		VECTOR cameraPos;
		cameraPos.x = cosf(cameraAngle) * kCameraDist;
		cameraPos.y = kCameraHeight;
		cameraPos.z = sinf(cameraAngle) * kCameraDist;
		cameraPos=VAdd(cameraPos, VGet(0, 50.0f, 0));

		SetCameraPositionAndTarget_UpVecY(cameraPos, VGet(0.0f,2.0f, 0.0f));
	
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

	MV1DeleteModel(PlayerModel);
	PlayerModel = -1;

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}