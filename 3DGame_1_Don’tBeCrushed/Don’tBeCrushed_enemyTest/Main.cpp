#include "DxLib.h"
#include "Scene/SceneGame.h"
#include "Game.h"
#include <cmath>
#include <memory>

using namespace std;

namespace {
	const char* const kModelPlayer = "data/model/Player.mv1";
	const char* const kBg = "data/floor2.png";

	// カメラ情報
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

	// X+-,Z+-の方向が分かりやすいように表示を追加する
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

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 一部の関数はDxLib_Init()の前に実行する必要がある
	ChangeWindowMode(true);
	SetGraphMode(kScreenWidth, kScreenHeight - 76, kColorDepth);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	SetDrawScreen(DX_SCREEN_BACK);

	// モデル
	int PlayerModel = MV1LoadModel(kModelPlayer);
	float PlayerScele = 3.0f;
	MV1SetScale(PlayerModel, VGet(PlayerScele, PlayerScele, PlayerScele));

	// プレイヤー
	VECTOR m_pos;
	m_pos = VGet(0.0f, 1.0f, 0.0f);
	MV1SetPosition(PlayerModel, m_pos);


	std::shared_ptr<SceneGame> m_pScene = make_shared<SceneGame>();
	m_pScene->Init();

	int bgGraph = LoadGraph(kBg);

	// カメラ情報
	float cameraAngle = -DX_PI_F / 2;

	// ゲームループ
	while (ProcessMessage() != -1)
	{
		// このフレームの開始時刻を覚えておく
		LONGLONG start = GetNowHiPerformanceCount();

		// 描画を行う前に画面をクリアする
		ClearDrawScreen();

		DrawExtendGraph(0, 0, kScreenWidth, kScreenHeight, bgGraph, false);

		// ゲームの処理
		m_pScene->Update();
		m_pScene->Draw();

		// モデル
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

		// 正規化

		//VECTOR m_dir = VNorm(m_pos);
		//m_pos = VAdd(m_pos, m_dir);

		DrawGrid();

		// カメラ
		SetCameraNearFar(kCameraNear, kCameraFar);
		VECTOR cameraPos;
		cameraPos.x = cosf(cameraAngle) * kCameraDist;
		cameraPos.y = kCameraHeight;
		cameraPos.z = sinf(cameraAngle) * kCameraDist;
		cameraPos = VAdd(cameraPos, VGet(0, 50.0f, 0));

		SetCameraPositionAndTarget_UpVecY(cameraPos, VGet(0.0f, 2.0f, 0.0f));

		ChangeLightTypeDir(VGet(0.0f, -10.0f, 0.0f));

		// 画面が切り替わるのを待つ
		ScreenFlip();

		// escキーでゲーム終了
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}

		// FPS60に固定する
		while (GetNowHiPerformanceCount() - start < 16667)
		{
			// 16.66ミリ秒(16667マイクロ秒)経過するまで待つ
		}
	}

	MV1DeleteModel(PlayerModel);
	
	PlayerModel = -1;

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}