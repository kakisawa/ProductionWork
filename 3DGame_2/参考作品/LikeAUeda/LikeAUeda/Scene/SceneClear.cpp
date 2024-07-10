#include "DxLib.h"
#include "Input.h"
#include "SceneTitle.h"
#include "SceneClear.h"

/// <summary>
/// コンストラクタ
/// </summary>
SceneClear::SceneClear()
{
}


/// <summary>
/// デストラクタ
/// </summary>
SceneClear::~SceneClear()
{
}


/// <summary>
/// 初期化
/// </summary>
void SceneClear::Init()
{
}


/// <summary>
/// 更新
/// </summary>
/// <param name="input">入力状態</param>
/// <returns></returns>
std::shared_ptr<SceneBase> SceneClear::Update(Input& input)
{
	if (input.IsTriggered("OK"))
	{
		return std::make_shared<SceneTitle>();
	}
	return shared_from_this();	// 自身のshared_ptrを返す
}


/// <summary>
/// 描画
/// </summary>
void SceneClear::Draw()
{
#ifdef _DEBUG	// デバッグ表示
	// 現在のシーン
	DrawString(0, 0, "クリア画面", 0xffffff);
#endif
}
