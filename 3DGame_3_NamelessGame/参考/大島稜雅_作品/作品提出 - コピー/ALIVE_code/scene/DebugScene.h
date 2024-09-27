#pragma once
#include "SceneBase.h"
#include <list>
#include <memory>
#include <vector>
#include <string>

class DebugScene final : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	DebugScene(SceneManager& manager);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~DebugScene();

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

	int selectNum_ = 0;					//選択番号

	std::list<std::string> sceneName_;	//シーンの名前
};

