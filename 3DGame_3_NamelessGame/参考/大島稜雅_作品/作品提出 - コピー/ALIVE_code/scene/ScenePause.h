#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>
#include <string>
#include <DxLib.h>

class UIItemManager;

class ScenePause final : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	ScenePause(SceneManager& manager);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ScenePause();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 終了
	/// </summary>
	void End();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	int selectNum_ = 0;						//選択番号

	std::shared_ptr<UIItemManager> UI_;		//UIマネージャのスマートポインタ
	std::vector<std::string> menuName_;		//ポーズメニューの名前用
};

