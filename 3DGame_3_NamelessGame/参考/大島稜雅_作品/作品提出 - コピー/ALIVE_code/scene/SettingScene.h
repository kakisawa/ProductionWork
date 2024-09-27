#pragma once
#include "SceneBase.h"
#include <memory>
#include <string>

class InputState;
class UIItemManager;

class SettingScene final : public SceneBase
{
private:
	//アップデートを行う項目
	enum class UpdateItem
	{
		WindowMode,
		BGM,
		SE,

		Max,
	};
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	SettingScene(SceneManager& manager);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SettingScene();

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

	/// <summary>
	/// ウィンドウモードの描画
	/// </summary>
	/// <param name="alphaValue">アルファ値</param>
	void DrawWindowMode(int alphaValue);

	/// <summary>
	/// SE調整バーの描画
	/// </summary>
	/// <param name="alphaValue">アルファ値</param>
	void DrawSEBar(int alphaValue);

	/// <summary>
	/// BGM調整バーの描画
	/// </summary>
	/// <param name="alphaValue">アルファ値</param>
	void DrawBGMBar(int alphaValue);

	/// <summary>
	/// 通常時の更新
	/// </summary>
	void NormalUpdate();

	/// <summary>
	/// ガウス処理を施したフェードイン
	/// </summary>
	void GaussFadeInUpdate();

	/// <summary>
	/// ガウス処理を施したフェードアウト
	/// </summary>
	void GaussFadeOutUpdate();

	/// <summary>
	/// BGMの音量を変更する
	/// </summary>
	void BGMUpdate();

	/// <summary>
	/// SEの音量を変更する
	/// </summary>
	void SEUpdate();

	/// <summary>
	/// ウィンドウモードを変更する
	/// </summary>
	void ChangeWindowUpdate();

	/// <summary>
	/// updateFuncの中身を変更する
	/// </summary>
	void ChangeUpdateFunc();

private:

	//ここも消すかもしれない
	static constexpr int fadeInterval_ = 30;					//インターバル
	int fadeTimer_ = 0;											//フェードする時間
	int fadeValue_ = 0;											//フェードの透過値
	int fadeColor_ = 0x000000;									//背景

	int makeScreenHandle_ = -1;									//スクリーンハンドル
	int volumeBGM_ = 0;											//BGMの音量
	int volumeSE_ = 0;											//SEの音量
	int selectNum_ = 0;											//選択番号

	bool windowMode_ = true;									//ウィンドウモード

	std::string windowModeText_ = "≪  ウィンドウモード  ≫";	//表示するウィンドウモードのテキスト

	std::shared_ptr<SceneBase> nextScene_;						//変更する次のシーン
	std::shared_ptr<UIItemManager> UIManager_;					//UIマネージャー

	void(SettingScene::* updateFunc_)();						//メンバ関数ポインタ

};

