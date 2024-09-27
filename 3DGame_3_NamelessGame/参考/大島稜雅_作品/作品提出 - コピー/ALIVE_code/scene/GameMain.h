#pragma once
#include "SceneBase.h"
#include <DxLib.h>
#include <memory>
#include <list>
#include <unordered_map>

class Camera;
class Player;
class CheckCollisionModel;
class ObjectManager;
class ShotManager;
class Tutorial;

class GameMain final : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	GameMain(SceneManager& manager);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~GameMain();

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
	/// ゴールしているかを判定する
	/// </summary>
	void GoalCheck();

	//消すかもしれない
	void FadeInUpdate();
	void NormalUpdate();
	void FadeOutUpdate();

private:
	//ここも消すかもしれない
	static constexpr int fadeInterval_ = 60;					//フェードする間隔
	int fadeTimer_ = fadeInterval_;								//タイマー
	int fadeValue_ = 255;										//フェードの値
	int fadeColor_ = 0x000000;									//背景の色

	//ライトのハンドル
	int lightHandle_ = -1;										//ライトのハンドル

	int skyHandle_ = -1;										//スカイドームのハンドル
	int makeScreenHandle_ = -1;									//作成したスクリーン画像
	int totalDeathNum_ = 0;										//ゲーム開始からの総死亡数

	bool isFilterOn_ = false;									//フィルターを使用するか

	std::shared_ptr<Camera> camera_;							//カメラのシェアードポインタ
	std::shared_ptr<Player> player_;							//プレイヤーのシェアードポインタ
	std::shared_ptr<ObjectManager> objManager_;					//objectManagerのシェアードポインタ
	std::shared_ptr<ShotManager> shotManager_;					//shotManagerのシェアードポインタ
	std::shared_ptr<Tutorial> tutorial_;						//チュートリアルのシェアードポインタ
	
	std::shared_ptr<CheckCollisionModel> checkCollisionModel_;	//衝突判定を行うクラスのポインタ

	void (GameMain::* updateFunc_)();							//メンバ関数ポインタ
};

