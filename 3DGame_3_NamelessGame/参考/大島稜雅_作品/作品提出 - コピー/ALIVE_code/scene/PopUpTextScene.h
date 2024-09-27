#pragma once
#include "SceneBase.h"

class InputState;

class PopUpTextScene final : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	PopUpTextScene(SceneManager& manager);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PopUpTextScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// デストラクタ
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
};

