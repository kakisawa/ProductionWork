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
	SetMainWindowText("Uedaが往く");

	// windowモード設定
	ChangeWindowMode(true);
	// 画面サイズ変更
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);

	if (DxLib_Init() == -1)	// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	// Effekseerの初期化
	Effekseer_Init(8000);

	// フルスクリーン切り替え時におかしくならないように
	SetChangeScreenModeGraphicsSystemResetFlag(false);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// Zバッファの設定
	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);
	SetUseBackCulling(true);

	// ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	// 入力状態を取得
	Input input;
	Font::Load();		// フォントの読み込み
	Sound::Load();		// サウンドの読み込み

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

		//裏画面を表画面を入れ替える
		ScreenFlip();

		// fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}


#ifdef _DEBUG
		// デバッグ時はESCキーで終了できるようにする
		if (input.IsTriggered("end"))
		{
			Font::UnLoad();
			Sound::UnLode();
			Effkseer_End();
			DxLib_End();
		}
#endif
	}

	Font::UnLoad();	 // フォントの解放
	Sound::UnLode(); // サウンドの解放
	Effkseer_End();	 // Effekseerの終了処理
	DxLib_End();	 // Dxライブラリ使用の終了処理

	return 0;		// ソフトの終了 
}