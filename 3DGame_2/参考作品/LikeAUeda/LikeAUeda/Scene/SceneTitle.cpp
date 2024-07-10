#include "DxLib.h"
#include "Input.h"
#include "Player.h"
#include "Camera.h"
#include "Stage.h"
#include "SceneStage1.h"
#include "SceneTitle.h"

/// <summary>
/// コンストラクタ
/// </summary>
SceneTitle::SceneTitle()
{
}


/// <summary>
/// デストラクタ
/// </summary>
SceneTitle::~SceneTitle()
{
}


/// <summary>
/// 初期化
/// </summary>
void SceneTitle::Init()
{
}


/// <summary>
/// 更新
/// </summary>
/// <param name="input">入力状態</param>
/// <returns></returns>
std::shared_ptr<SceneBase> SceneTitle::Update(Input& input)
{
	// シーン遷移
	if (input.IsTriggered("OK"))
	{
		std::shared_ptr<Player> pPlayer = std::make_shared<Player>();
		std::shared_ptr<Camera> pCamera = std::make_shared<Camera>();
		std::shared_ptr<Stage> pStage = std::make_shared<Stage>();
		return std::make_shared<SceneStage1>(pPlayer, pCamera, pStage);
	}

	return shared_from_this();	// 自身のshared_ptrを返す
}


/// <summary>
/// 描画
/// </summary>
void SceneTitle::Draw()
{
#ifdef _DEBUG	// デバッグ表示
	// 現在のシーン
	DrawString(0, 0, "タイトル画面", 0xffffff);
#endif
}
