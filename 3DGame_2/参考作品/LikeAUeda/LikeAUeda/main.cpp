#include "DxLib.h"
#include "Input.h"
#include "Game.h"
#include "LoadData.h"
#include "SceneManager.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetMainWindowText("Uedaが如く");

	// windowモード設定
	ChangeWindowMode(true);
	// 画面サイズ変更
	//SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);

	if (DxLib_Init() == -1)	// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	// Zバッファの設定
	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);
	SetUseBackCulling(true);

	// ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	// 入力状態を取得
	Input input;
	
	// 外部データのロード
	std::shared_ptr<LoadData> pLoadData = std::make_shared<LoadData>();

	// SceneManagerを生成
	std::shared_ptr<SceneManager> pScene = std::make_shared<SceneManager>();
	pScene->Init();

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// 画面のクリア
		ClearDrawScreen();

		// 更新
		input.Update();
		pScene->Update(input);

		// 描画
		pScene->Draw();
		//pLoadData->Draw();

		//裏画面を表画面を入れ替える
		ScreenFlip();

		// fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}

#ifdef _DEBUG
		if (input.IsTriggered("end"))
		{
			DxLib_End();
		}
#endif
	}

	DxLib_End(); // ＤＸライブラリ使用の終了処理

	return 0;	 // ソフトの終了 
}