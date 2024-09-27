#pragma once
#include "SceneBase.h"
#include <memory>

class UIItemManager;

class LoadingScene final : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	LoadingScene(SceneManager& manager);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~LoadingScene();

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

	int aSyncLoadNum_ = 0;							//非同期読み込み中の処理の数

	bool loadingFile_ = false;						//ロード中  true:途中　false:完了

	std::shared_ptr<UIItemManager> UIManager_;		//uiメニューのマネージャーのスマートポインタ

};

