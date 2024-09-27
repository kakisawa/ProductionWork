#include "GameMain.h"
#include "GameEnd.h"
#include "ScenePause.h" 
#include "SceneManager.h"

#include "../gimmick/Switch.h"
#include "../gimmick/GimmickBase.h"

#include "../object/Player.h"
#include "../object/Camera.h"
#include "../object/ObjectData.h"
#include "../object/ShotManager.h"
#include "../object/CharacterBase.h"
#include "../object/ObjectManager.h"

#include "../util/game.h"
#include "../util/Util.h"
#include "../util/model.h"
#include "../util/Tutorial.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"
#include "../util/ModelManager.h"
#include "../util/EffectManager.h"
#include "../util/CheckCollisionModel.h"

#include <algorithm>

namespace
{
	//フェードの最大値
	constexpr int max_fade_value = 255;

	//ガウス処理で使用するピクセル幅
	constexpr int pixel_width = 32;

	//ぼかしのパラメーター
	constexpr int pixel_param = 800;

	//カプセルの半径
	constexpr float capsel_radius = 600.0f;

	//スカイドームのサイズ
	constexpr float skydorm_scale = 30.0f;

	//3Dサウンドの1mの値
	constexpr float sound_3D_1m_value = 1.0f;

	//カメラの初期位置
	constexpr VECTOR camera_init_pos = { 0, 300, -550 };

	//スカイドームの初期位置
	constexpr VECTOR skydorm_init_pos = { 0, 200, 0 };

	//ディレクションライトの角度
	constexpr VECTOR direction_light_dir = { 33.0f, -33.0f, 33.0f };
}

//コンストラクタ
GameMain::GameMain(SceneManager& manager) : SceneBase(manager),updateFunc_(&GameMain::FadeInUpdate)
{
	//短縮化
	auto& file = ExternalFile::GetInstance();

	//インスタンス化
	player_ = std::make_shared<Player>(file.GetMainSpecialObjectInfo("Player"));
	tutorial_ = std::make_shared<Tutorial>();
	shotManager_ = std::make_shared<ShotManager>();
	objManager_ = std::make_shared<ObjectManager>();
	checkCollisionModel_ = std::make_shared<CheckCollisionModel>();

	//カメラのポジション
	VECTOR pos = VAdd(player_->GetStatus().pos, camera_init_pos);
	camera_ = std::make_shared<Camera>(pos, player_->GetStatus().pos);

	//ディレクションライトに設定する
	ChangeLightTypeDir(direction_light_dir);

	//ライトのディフィーズカラーを設定する
	SetLightDifColor(GetColorF(1.0f, 1.0f, 1.0f, 1.0f));

	//ゲームオブジェクトの生成
	objManager_->MainStageObjectGenerator();
}

//デストラクタ
GameMain::~GameMain()
{
	//ゲームメインが終わるときにプレイ中に死んだ回数と
	//saveDataに記録されている死亡回数を足す
	ExternalFile::GetInstance().SaveDataRewriteInfo(totalDeathNum_);
}

//初期化
void GameMain::Init()
{
	//短縮化
	auto& model = ModelManager::GetInstance();

	//スクリーン画像の作成
	makeScreenHandle_ = MakeScreen(Game::screen_width, Game::screen_height, true);

	//1mの範囲を設定する
	Set3DSoundOneMetre(sound_3D_1m_value);

	//スカイドームの設定
	skyHandle_ = model.GetModelHandle("SkyDorm");
	MV1SetScale(skyHandle_, VGet(skydorm_scale, skydorm_scale, skydorm_scale));
	MV1SetPosition(skyHandle_, skydorm_init_pos);
}

//終了
void GameMain::End()
{
	DeleteGraph(makeScreenHandle_);
	MV1DeleteModel(skyHandle_);
}

//更新
void GameMain::Update()
{
	(this->*updateFunc_)();
}

//描画
void GameMain::Draw()
{
	SetDrawScreen(makeScreenHandle_);
	ClearDrawScreen();

	//カメラの初期化
	//SetDrawScreenを行うとカメラの情報がリセットされるために
	camera_->Init(camera_->GetTarget());
	camera_->Update(player_->GetStatus().pos, player_->GetStatus().height);

	//プレイヤーの描画
	player_->Draw();

	//オブジェクトの描画
	objManager_->Draw();

	//エフェクトの描画
	EffectManager::GetInstance().Draw();

	//スカイドームの描画
//	MV1SetPosition(skyHandle_, player_->GetStatus().pos);
//	MV1DrawModel(skyHandle_);

	//弾の描画
	shotManager_->Draw();

	//チュートリアルの描画
	tutorial_->Draw();

#ifdef _DEBUG
	VECTOR pos = player_->GetStatus().pos;
	DrawFormatString(0, 48, 0xffffff, "%.2f,%.2f,%.2f", pos.x, pos.y, pos.z);
	int num = GetEnableLightHandleNum();
	DrawFormatString(0, 64, 0xffffff, "%d", num);
#endif // _DEBUG

	SetDrawScreen(DX_SCREEN_BACK);

	//フィルター処理を行うか
	if (isFilterOn_) 
	{
		GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, pixel_width, pixel_param);
	}

	//makescreenHandleに書き込んだ内容を描画する
	DrawGraph(0, 0, makeScreenHandle_, true);

	//画面全体を真っ黒に塗りつぶす
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//ゴールしているかを判定する
void GameMain::GoalCheck()
{
	//短縮化
	auto& file = ExternalFile::GetInstance();

	//ゴールの配置データを取得
	auto info = file.GetMainSpecifiedInfo("GOAL");

	//衝突判定
	auto result = MV1CollCheck_Capsule( player_->GetModelPointer()->GetModelHandle(),
										player_->GetModelPointer()->GetColFrameIndex(),
										VGet(info.pos.x, info.pos.y - info.scale.y, info.pos.z),
										VGet(info.pos.x, info.pos.y + info.scale.y, info.pos.z), capsel_radius);
	//終了判定
	if (result.HitNum > 0)
	{
		totalDeathNum_ += player_->GetDeathCount();
		file.SetDeathCount(totalDeathNum_);
		updateFunc_ = &GameMain::FadeOutUpdate;
	}

	//衝突判定の削除
	MV1CollResultPolyDimTerminate(result);
}

//TODO：別のフェードインが出来次第消去
void GameMain::FadeInUpdate()
{
	fadeValue_ = static_cast <int>(max_fade_value * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) 
	{
		updateFunc_ = &GameMain::NormalUpdate;
		fadeValue_ = 0;
	}
}

//更新
void GameMain::NormalUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();
	auto& file = ExternalFile::GetInstance();

	//フィルター処理を行わない用にする
	isFilterOn_ = false;

	//プレイヤーの更新
	player_->Update();

	//オブジェクトの更新
	objManager_->Update(*player_,shotManager_);

	//エフェクトの更新
	EffectManager::GetInstance().Update();

	//プレイヤーとその他オブジェクトとの衝突判定
	checkCollisionModel_->CheckCollision(player_,objManager_);

	//弾の更新
	shotManager_->Update();

	//弾とプレイヤーの衝突判定
	shotManager_->Hit(*player_);

	//チュートリアル
	tutorial_->Update(player_->GetStatus().pos);

	//ごーるしているかを判定する
	GoalCheck();

	//リスナーの位置と方向を設定
	//今回は、プレイヤーではなくカメラの座標にしている
	SoundManager::GetInstance().Set3DSoundListenerInfo(camera_->GetPos(), camera_->GetTarget());

	//ポーズシーンを開く
	if (input.IsTriggered(InputType::Pause)) 
	{
		isFilterOn_ = true;
		manager_.PushFrontScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
	}
}

//TODO：別のフェードインが出来次第消去
void GameMain::FadeOutUpdate()
{
	fadeValue_ = static_cast <int>(max_fade_value * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) 
	{
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameEnd>(manager_)));
		return;
	}
}
