#pragma once
#include <deque>
#include <memory>

class SceneBase;
class InputState;

class SceneManager
{
public:
	/// <summary>
	/// シーン遷移
	/// </summary>
	/// <param name="scene">シーンのポインタ</param>
	void ChangeScene(const std::shared_ptr<SceneBase>& scene);

	/// <summary>
	/// メインのシーンを保ったまま別のシーンを前画面に表示する
	/// </summary>
	/// <param name="scene">シーンのポインタ</param>
	void PushFrontScene(const std::shared_ptr<SceneBase>& scene);

	/// <summary>
	/// シーンをすり替える
	/// </summary>
	/// <param name="scene">シーンのポインタ</param>
	void SwapScene(const std::shared_ptr<SceneBase>& scene);

	/// <summary>
	/// ひとつ前のシーンに戻る
	/// </summary>
	void PopFrontScene();	

	/// <summary>
	/// シーンの更新
	/// </summary>
	void Update();	

	/// <summary>
	/// シーンの描画
	/// </summary>
	void Draw();	

	/// <summary>
	/// メインwhile文を終了させるため
	/// </summary>
	/// <returns>ゲームを終了させるか true:終了 false:終了しない</returns>
	bool End() const { return isEnd_; };		

	////////////////Getter////////////////

	/// <summary>
	/// ウィンドウモードの取得
	/// </summary>
	/// <returns></returns>
	bool GetWindowMode() const { return windowMode_; }

	////////////////Setter////////////////

	/// <summary>
	/// 終了フラグを変更する
	/// </summary>
	/// <param name="flag">終了するか</param>
	void SetEndFlag(const bool flag) { isEnd_ = flag; }

	/// <summary>
	/// ウィンドウモードを変更する
	/// </summary>
	/// <param name="windowMode">true：windowMode　false：フルスクリーン</param>
	void SetChangeWindowMode(const bool windowMode);

private:
	int debugDrawTime_ = 0;								//描画にかかる時間(1フレーム当たり)
	int debugUpdateTime_ = 0;							//更新にかかる時間(1フレーム当たり)

	bool isEnd_ = false;								//終了フラグ
	bool windowMode_ = true;							//windowMode　true：windowMode　false：フルスクリーン

	std::deque<std::shared_ptr<SceneBase>> scenes_;		//シーン本体
};

