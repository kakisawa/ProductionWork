#include "Dxlib.h"
#include "Input.h"
#include "SceneTitle.h"
#include "SceneGameover.h"


/// <summary>
/// コンストラクタ
/// </summary>
SceneGameover::SceneGameover()
{
}


/// <summary>
/// デストラクタ
/// </summary>
SceneGameover::~SceneGameover()
{
}


/// <summary>
/// 初期化
/// </summary>
void SceneGameover::Init()
{
}


/// <summary>
/// 更新
/// </summary>
/// <param name="input">入力状態</param>
/// <returns></returns>
std::shared_ptr<SceneBase> SceneGameover::Update(Input& input)
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
void SceneGameover::Draw()
{
#ifdef _DEBUG	// デバッグ表示
	// 現在のシーン
	DrawString(0, 0, "ゲームオーバー画面", 0xffffff);
#endif
}
