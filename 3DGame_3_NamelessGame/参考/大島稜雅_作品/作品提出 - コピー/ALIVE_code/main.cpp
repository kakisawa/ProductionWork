#include "DxLib.h"
#include "EffekseerForDXLib.h"

#include "Scene/SceneManager.h"
#include "scene/LoadingScene.h"

#include "util/game.h"
#include "util/InputState.h"
#include "util/SoundManager.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//windowモードの設定
	ChangeWindowMode(Game::window_mode);

	//画面モード変更時グラフィックシステムやグラフィックハンドルをリセットするか
	//(true：する、false：しない)
	SetChangeScreenModeGraphicsSystemResetFlag(false);

	//window名設定
	SetMainWindowText(Game::title_text);

	//画面サイズの設定
	SetGraphMode(Game::screen_width,Game::screen_height, Game::screen_depth);

	//サウンドのインスタンス生成
	SoundManager::GetInstance();

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	//ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	SceneManager manager;

	manager.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<LoadingScene>(manager)));

	while (ProcessMessage() == 0) {

		LONGLONG time = GetNowHiPerformanceCount();

		Effekseer_Sync3DSetting();

		//画面のクリア
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

		//裏画面を表画面に切り替える
		ScreenFlip();

		//escを押したら終了する
		if (CheckHitKey(KEY_INPUT_ESCAPE))break;

		//fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667) {

		}

		if (manager.End()) {
			break;
		}

	}

	//EffekseerManager::GetInstance().End();

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}