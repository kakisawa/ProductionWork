#pragma once
#include "SceneBase.h"
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <DxLib.h>

class Model;
class Camera;
class LightBulb;
class UIItemManager;
class ObjectManager;

class SceneTitle final : public SceneBase
{
private:
	//選択できるシーン
	enum class SelectScene
	{
		Setting,
		NewGame,
		SelectChapter,
		End,
		None,
		max,
	};

	//シーン選択のデータ
	struct SelectSceneData
	{
		//指定方向に移動できるか
		bool up		= false;
		bool down	= false;
		bool left	= false;
		bool rignt	= false;

		//指定の方向のシーンUI
		SelectScene upScene		= SelectScene::None;
		SelectScene downScene	= SelectScene::None;
		SelectScene leftScene	= SelectScene::None;
		SelectScene rigntScene	= SelectScene::None;
	};

	//データ
	SelectSceneData selectData_[static_cast<int>(SelectScene::max)]
	{
		//指定方向に移動できるか
		false,
		true,
		false,
		true,
		//指定の方向のシーンUI
		SelectScene::None,
		SelectScene::NewGame,
		SelectScene::None,
		SelectScene::NewGame,

		//指定方向に移動できるか
		true,
		false,
		true,
		true,
		//指定の方向のシーンUI
		SelectScene::SelectChapter,
		SelectScene::None,
		SelectScene::Setting,
		SelectScene::End,

		//指定方向に移動できるか
		false,
		true,
		true,
		false,
		//指定の方向のシーンUI
		SelectScene::None,
		SelectScene::NewGame,
		SelectScene::NewGame,
		SelectScene::None,

		//指定方向に移動できるか
		false,
		true,
		true,
		false,
		//指定の方向のシーンUI
		SelectScene::None,
		SelectScene::NewGame,
		SelectScene::NewGame,
		SelectScene::None,

	};

	//計　36byte
	struct CameraInfo 
	{
		VECTOR targetPos;	//目的の場所		//12byte
		VECTOR targetView;	//見る場所			//12byte
		VECTOR upVec;		//カメラの上方向	//12byte
	};
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	SceneTitle(SceneManager& manager);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneTitle();

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
	/// <summary>
	/// プレイヤーのモデルの初期化
	/// </summary>
	void PlayerModelInit();

	/// <summary>
	/// UIの設定
	/// </summary>
	void UISetting();

	/// <summary>
	/// ライトの設定
	/// </summary>
	void LightSetting();

	/// <summary>
	/// カメラの設定
	/// </summary>
	void CameraPositionDataSetting();

	/// <summary>
	/// カメラの目標座標の更新
	/// </summary>
	void CameraTargetUpdate();

	/// <summary>
	/// 選択シーンの更新
	/// </summary>
	void SelectSceneUpdate();

	/// <summary>
	/// フェードインの更新
	/// </summary>
	void FadeInUpdate();

	/// <summary>
	/// UIの更新
	/// </summary>
	void UIUpdate();

	/// <summary>
	/// オープニングの更新
	/// </summary>
	void OpeningUpdate();

	/// <summary>
	/// UIのフェードアウト
	/// </summary>
	void UIFadeOutUpdate();

	/// <summary>
	/// タイトルのフェードアウト
	/// </summary>
	void SceneTitleFadeOutUpdate();

	/// <summary>
	/// シーンを切り替える
	/// </summary>
	void SceneChange();

	/// <summary>
	/// ニューゲーム又はゲームオーバーが選択された時の変更
	/// </summary>
	void NewGameOrGameEndChange();

private:

	static constexpr int fadeInterval_ = 30;				//インターバル
	int fadeTimer_ = 0;										//フェードする時間
	int fadeValue_ = 0;										//フェードの透過値
	int UIfadeValue_ = 0;									//UIのフェード透過値
	int fadeColor_ = 0x000000;								//黒

	int spotLightHandle_ = -1;								//スポットライトのハンドル
	int directionLightHandle_ = -1;							//ディレクションライトのハンドル
	int fontHandle_ = -1;									//フォントハンドル
	int titleHandle_ = -1;									//タイトル画像の保管変数

	float outAngle_ = 0.0f;									//スポットライトの外側の角度
	float inAngle_ = 0.0f;									//スポットライトの内側の角度

	SelectScene currentSelectScene_ = SelectScene::NewGame;	//現在の選択シーン
	SelectScene oldSelectScene_ = SelectScene::NewGame;		//ひとつ前の選択シーン

	VECTOR directionLightDir_ = { 0.0f, 0.0f, 0.0f };		//ディレクションライトの方向

	std::shared_ptr<Camera> camera_;						//カメラのスパートポインタ
	std::shared_ptr<UIItemManager> UI_;						//UIマネージャーのスマートポインタ
	std::shared_ptr<LightBulb> lightBulb_;					//電球
	std::shared_ptr<Model> subPlayerModel_;					//プレイヤーのスマートポインタ
	std::shared_ptr<Model> mainPlayerModel_;				//プレイヤーのスマートポインタ
	std::shared_ptr<ObjectManager> objManager_;				//オブジェクトマネージャーのスマートポインタ

	std::vector<std::string> menuName_;						//メニューの文字列
//	std::vector<int> lightHandle_;							//ライトハンドル
//	std::vector<VECTOR> lightDir_;							//ライトのディレクション(方向)
	std::vector<CameraInfo> cameraInfo_;					//カメラの情報

	std::map<std::string, VECTOR> menuDrawPos_;				//メニューを描画する座標

	void (SceneTitle::* updateFunc_)();						//メンバ関数ポインタ
};

