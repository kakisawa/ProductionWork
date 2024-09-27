#pragma once

class SceneManager;

class SceneBase abstract
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	SceneBase(SceneManager& manager) : manager_(manager){}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneBase() {};
	
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() abstract;

	/// <summary>
	/// 終了
	/// </summary>
	virtual void End() abstract;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() abstract;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() abstract;
protected:
	SceneManager& manager_;			//シーンマネージャーの参照
};

