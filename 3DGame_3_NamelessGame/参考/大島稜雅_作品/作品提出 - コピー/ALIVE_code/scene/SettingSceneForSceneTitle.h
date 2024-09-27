#pragma once
#include "SceneBase.h"
#include <map>
#include <memory>
#include <string>
#include <DxLib.h>

class InputState;
class UIItemManager;

class SettingSceneForSceneTitle final : public SceneBase
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
	SettingSceneForSceneTitle(SceneManager& manager);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SettingSceneForSceneTitle();

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

	/// <summary>
	/// シーンを切り替える
	/// </summary>
	void SceneChange();
private:

	int makeScreenHandle_ = -1;									//作成したスクリーン画像
	int volumeBGM_ = 0;											//BGMのボリューム
	int volumeSE_ = 0;											//SEのボリューム
	int selectNum_ = 0;											//選択番号

	bool windowMode_ = true;									//ウィンドウモード

	//ここも消すかもしれない
	static constexpr int fadeInterval_ = 30;					//インターバル
	int fadeTimer_ = 0;											//フェードする時間
	int fadeValue_ = 0;											//フェードの透過値
	int fadeColor_ = 0x000000;									//背景

	std::string windowModeText_ = "≪  ウィンドウモード  ≫";	//ウィンドウモードの表示テキスト
	std::shared_ptr<UIItemManager> UIManager_;					//UIマネージャーのポインタ
	std::map<std::string, VECTOR> menuDrawPos_;					//メニューを描画する座標

	void(SettingSceneForSceneTitle::* updateFunc_)();			//メンバ関数ポインタ
};

