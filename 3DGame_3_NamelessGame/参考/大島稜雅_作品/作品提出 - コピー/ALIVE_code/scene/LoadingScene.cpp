#include "LoadingScene.h"
#include "DebugScene.h"
#include "SceneTitle.h"
#include "SceneManager.h"

#include "../util/game.h"
#include "../util/ModelManager.h"
#include "../util/FontsManager.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"
#include "../util/GraphManager.h"
#include "../util/EffectManager.h"
#include "../util/UIItemManager.h"

namespace
{
	//文字列表示の座標補正X
	constexpr float correction_x = 230.0f;

	//文字列表示の座標補正X
	constexpr float correction_y = 80.0f;
}

//コンストラクタ
LoadingScene::LoadingScene(SceneManager& manager):SceneBase(manager)
{
}

//デストラクタ
LoadingScene::~LoadingScene()
{
}

//初期化
void LoadingScene::Init()
{
	//短縮化
	auto& font = FontsManager::GetInstance();

	//インスタンス化
	UIManager_ = std::make_shared<UIItemManager>();

	//UI画像作成
	UIManager_->AddingMenuWithSplitStr(Game::screen_width - correction_x, Game::screen_height - correction_y, Game::screen_width, Game::screen_height, "Loading...", font.GetFontHandle("ピグモ 0042"));

	//非同期初期の開始
	SetUseASyncLoadFlag(true);

	//外部ファイルのロード
	ExternalFile::GetInstance().LoadFile();
	
	//モデルファイルパスを読み込む
	ModelManager::GetInstance().LoadModelFilePath();

	//モデルのロード
	ModelManager::GetInstance().LoadModel();
	
	//サウンドファイルのロード
	SoundManager::GetInstance().LoadSound();
	
	//画像のファイルパスを読み込む
	GraphManager::GetInstance().LoadGraphFilePath();

	//画像のロード
	GraphManager::GetInstance().Load();

	//分割画像のファイルパスを読み込む
	EffectManager::GetInstance().LoadDivGraphFilePath();

	//エッフェクト画像のロード
	EffectManager::GetInstance().Load();

	//非同期処理を終了する
	SetUseASyncLoadFlag(false);

	loadingFile_ = true;
}

//終了
void LoadingScene::End()
{
}

//更新
void LoadingScene::Update()
{
	//非同期読み込み中の数を取得
	aSyncLoadNum_ = GetASyncLoadNum();

	if (loadingFile_)
	{
		if (aSyncLoadNum_ < 1) 
		{
			loadingFile_ = false;

			//データのロード
			ExternalFile::GetInstance().LoadArrangementData();
		}
	}
	else {
		if (aSyncLoadNum_ < 1) 
		{
#ifdef _DEBUG
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<DebugScene>(manager_)));
#else
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
#endif
		}
	}
}

//描画
void LoadingScene::Draw()
{
	UIManager_->ChangePosDraw(Game::screen_width - correction_x, Game::screen_height - correction_y);
}
