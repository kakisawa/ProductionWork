#include "DxLib.h"

namespace {
	const char* const kModelEnemy = "data/model/book.mv1";
	const int size = 300;
}

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 一部の関数はDxLib_Init()の前に実行する必要がある
	ChangeWindowMode(true);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	SetDrawScreen(DX_SCREEN_BACK);

	VECTOR StartPos, EndPos;
	MV1_COLL_RESULT_POLY HitPoly;

	int m_model;
	m_model = MV1LoadModel(kModelEnemy);
	MV1SetPosition(m_model, VGet(300,300,0));
	MV1SetScale(m_model, VGet(size, size, size));

	MV1SetupCollInfo(m_model, -1, 8, 8, 8);

	// ゲームループ
	while (ProcessMessage() != -1)
	{
		// このフレームの開始時刻を覚えておく
		LONGLONG start = GetNowHiPerformanceCount();

		// 描画を行う前に画面をクリアする
		ClearDrawScreen();

		// ゲームの処理

		MV1DrawModel(m_model);

		StartPos = VGet(250.0f, 300.0f, 600.0f);
		EndPos = VGet(1000.0f, 300.0f, 600.0f);
		HitPoly = MV1CollCheck_Line(m_model, -1, StartPos, EndPos);


		DrawLine3D(StartPos, EndPos, GetColor(255, 255, 0));

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

	MV1DeleteModel(m_model);
	m_model = -1;

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}