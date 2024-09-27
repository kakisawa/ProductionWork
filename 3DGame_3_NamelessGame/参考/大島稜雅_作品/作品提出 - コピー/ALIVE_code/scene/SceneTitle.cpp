#include "SceneTitle.h"
#include "GameMain.h"
#include "ScenePause.h"
#include "SceneManager.h"
#include "SelectChapterScene.h"
#include "SettingSceneForSceneTitle.h"

#include "../object/Camera.h"
#include "../object/LightBulb.h"
#include "../object/ObjectManager.h"

#include "../util/game.h"
#include "../util/util.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/FontsManager.h"
#include "../util/SoundManager.h"
#include "../util/ExternalFile.h"
#include "../util/ModelManager.h"
#include "../util/GraphManager.h"
#include "../util/UIItemManager.h"

#include <algorithm>

namespace
{
	//アニメーションを変更するのにかかる時間
	constexpr int anim_change_time = 10;

	//画面の分割数
	constexpr int screen_split_num = 128;

	//画面の半分
	constexpr int screen_half = 2;

	//各画像の画面の分割配置ポジション
	constexpr int up_split_num = 92;
	constexpr int down_split_num = 87;
	constexpr int left_split_num = 92;
	constexpr int right_split_num = 97;
	constexpr int decision_split_num = 112;
	constexpr int move_str_split_num = 100;
	constexpr int decision_str_split_num = 116;

	//フェードの最大値
	constexpr int max_fade_value = 255;

	//UI配置ポジションX
	constexpr int UI_pos_x = 320;

	//UI配置ポジションY
	constexpr int UI_pos_y = 100;

	//タイトルのビルボートのサイズ
	constexpr float title_bill_board_size = 300.0f;

	//ビルボートのサイズ
	constexpr float bill_board_size = 200.0f;

	//中心位置
	constexpr float center_pos_x = 0.5f;
	constexpr float center_pos_y = 0.5f;

	//キー画像のサイズ
	constexpr float key_graph_size = 0.8f;

	//UI画像の高さの補正
	constexpr int str_UI_correction_heignt = 55;
	constexpr float UI_correction_height = 40.0f;
	constexpr float up_UI_correction_height = 90.0f;

	//UImenuのY座標の初期位置
	constexpr float UI_menu_init_pos_y = 120.0f;

	//UImenuの隙間サイズ
	constexpr float UI_menu_gap_size = 40.0f;

	//スポットライトの外側の角度
	constexpr float spot_light_out_angle = 90.0f;

	//スポットライトの内側の角度
	constexpr float spot_light_in_angle = 30.0f;

	//スポットライトの光が届く範囲
	constexpr float stop_light_range = 600.0f;

	//距離関係なく減衰するパラメーター
	constexpr float attenuation_regardless_of_distance = 0.0f;

	//距離に比例して減衰するパラメーター
	constexpr float attenuation_proportional_to_distance = 0.003f;

	//距離の2乗に比例して減衰するパラメーター
	constexpr float attenuation_proportional_to_the_square_of_the_distance = 0.0f;

	//カメラの初期位置
	constexpr VECTOR camera_init_pos = { 0, 140, 0 };

	//通常時のカメラの上ベクトル
	constexpr VECTOR normal_up_vec = { 0,1,0 };

	//本を見る時の上ベクトル
	constexpr VECTOR book_up_vec = { 0,0,1 };

	//ディレクションライトの角度タイプ1
	constexpr VECTOR direction_light_diagonally_lower_left = { -33.0f,-33.0f ,-33.0f };

	//ディレクションライトの角度タイプ2
	constexpr VECTOR direction_light_diagonally_lower_right = { 33.0f,-33.0f ,33.0f };

	//ディレクションライトの色
	constexpr COLOR_F direction_light_color = { 0.41f, 0.41f, 0.41f, 1.0f };

	//スポットライトの色
	constexpr COLOR_F spot_light_color = { 0.87f, 0.72f, 0.52f, 1.0f };
}

//コンストラクタ
SceneTitle::SceneTitle(SceneManager& manager): SceneBase(manager)
{
	//短縮化
	auto& file  = ExternalFile::GetInstance();
	auto& model = ModelManager::GetInstance();

	//インスタンス化
	UI_					= std::make_shared<UIItemManager>();
	objManager_			= std::make_shared<ObjectManager>();
	subPlayerModel_		= std::make_shared<Model>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Player)].name),Material::Other);
	mainPlayerModel_	= std::make_shared<Model>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Player)].name),Material::Other);
	camera_				= std::make_shared<Camera>(file.GetCameraTargetPos("start"),file.GetCameraTargetPos("startTargetPos"));
	lightBulb_			= std::make_shared<LightBulb>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::LightBulb)].name), file.GetMainSpecialObjectInfo("LightBulb"));

	//プレイヤーのモデルの初期化
	PlayerModelInit();

	//ライトの設定
	LightSetting();

	//カメラの配置等の設定
	CameraPositionDataSetting();

	//オブジェクトの生成
	objManager_->OpeningStageObjectGenerator();

	//UIの設定
	UISetting();
}

//デストラクタ
SceneTitle::~SceneTitle()
{
	DeleteLightHandle(spotLightHandle_);
	DeleteLightHandle(directionLightHandle_);
}

//初期化
void SceneTitle::Init()
{
	updateFunc_ = &SceneTitle::FadeInUpdate;

	oldSelectScene_ = currentSelectScene_;
	currentSelectScene_ = SelectScene::NewGame;

	CameraTargetUpdate();

	fadeTimer_ = 0;
	fadeValue_ = 0;
}

//終了
void SceneTitle::End()
{
}

//更新
void SceneTitle::Update()
{
	//カメラの更新
	camera_->EasingMoveCamera();

	//電球の更新
	lightBulb_->Update();

	//ライトの更新
	SetLightDirectionHandle(spotLightHandle_,lightBulb_->GetRotVec());
	SetLightPositionHandle (spotLightHandle_,lightBulb_->GetFramePos());

	//ライトの方向設定
	SetLightDirectionHandle(directionLightHandle_, directionLightDir_);
	
	(this->*updateFunc_)();
}

//描画
void SceneTitle::Draw()
{
	//短縮化
	auto& input = InputState::GetInstance();
	auto& file = ExternalFile::GetInstance();

	//カメラの初期化
	camera_->Init(camera_init_pos);

	//オブジェクトの描画
	objManager_->Draw();

	//電球の描画
	SetUseLighting(false);
	lightBulb_->Draw();
	SetUseLighting(true);

	//モデルの描画
	mainPlayerModel_->Draw();
	subPlayerModel_->Draw();

	//UIの描画
	UI_->DrawBillBoard(menuDrawPos_,UIfadeValue_, bill_board_size);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, UIfadeValue_);

	if (input.LastInputDevice()) 
	{
		//キー画像描画
		input.DrawKeyGraph(static_cast<int>(InputType::Up)		, Game::screen_width / screen_split_num * up_split_num		, Game::screen_height - up_UI_correction_height, key_graph_size);
		input.DrawKeyGraph(static_cast<int>(InputType::Down)	, Game::screen_width / screen_split_num * left_split_num	, Game::screen_height - UI_correction_height, key_graph_size);
		input.DrawKeyGraph(static_cast<int>(InputType::Left)	, Game::screen_width / screen_split_num * down_split_num	, Game::screen_height - UI_correction_height, key_graph_size);
		input.DrawKeyGraph(static_cast<int>(InputType::Right)	, Game::screen_width / screen_split_num * right_split_num	, Game::screen_height - UI_correction_height, key_graph_size);
		input.DrawKeyGraph(static_cast<int>(InputType::Space)	, Game::screen_width / screen_split_num * decision_split_num, Game::screen_height - UI_correction_height, key_graph_size);
	}
	else 
	{
		input.DrawPadGraph(static_cast<int>(InputType::Up)		, Game::screen_width / screen_split_num * up_split_num		, Game::screen_height - up_UI_correction_height, key_graph_size);
		input.DrawPadGraph(static_cast<int>(InputType::Down)	, Game::screen_width / screen_split_num * left_split_num	, Game::screen_height - UI_correction_height, key_graph_size);
		input.DrawPadGraph(static_cast<int>(InputType::Left)	, Game::screen_width / screen_split_num * down_split_num	, Game::screen_height - UI_correction_height, key_graph_size);
		input.DrawPadGraph(static_cast<int>(InputType::Right)	, Game::screen_width / screen_split_num * right_split_num	, Game::screen_height - UI_correction_height, key_graph_size);
		input.DrawPadGraph(static_cast<int>(InputType::Space)	, Game::screen_width / screen_split_num * decision_split_num, Game::screen_height - UI_correction_height, key_graph_size);
	}	

	//キーの役割りの文字列の描画
	DrawStringToHandle(Game::screen_width / screen_split_num * move_str_split_num, Game::screen_height - str_UI_correction_heignt, "移動", 0xffffff, fontHandle_);
	input.DrawName(static_cast<int>(InputType::Space), Game::screen_width / screen_split_num * decision_str_split_num, Game::screen_height - str_UI_correction_heignt, 0xffffff, fontHandle_, true, true, "/");

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//fadeValue_の値によって透過具合が変化するタイトルの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, UIfadeValue_);
	DrawBillboard3D(menuDrawPos_["タイトル"], center_pos_x, center_pos_y, title_bill_board_size, 0.0f, GraphManager::GetInstance().GetGraph("title"), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//黒いフェード用boxの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//プレイヤーのモデルの初期化
void SceneTitle::PlayerModelInit()
{
	//短縮化
	auto& file = ExternalFile::GetInstance();

	//メインプレイヤーモデルの配置データをセットする
	auto mainPlayerInfo = file.GetMainSpecialObjectInfo("MainPlayer");
	mainPlayerModel_->SetScale(mainPlayerInfo.scale);
	mainPlayerModel_->SetPos(mainPlayerInfo.pos);
	mainPlayerModel_->SetRot(mainPlayerInfo.rot);
	mainPlayerModel_->SetAnimation(static_cast<int>(PlayerAnimType::StopTimer), false, true);

	//サブプレイヤーモデルの配置データをセットする
	auto subPlayerInfo = file.GetMainSpecialObjectInfo("SubPlayer");
	subPlayerModel_->SetScale(subPlayerInfo.scale);
	subPlayerModel_->SetPos(subPlayerInfo.pos);
	subPlayerModel_->SetRot(subPlayerInfo.rot);
	subPlayerModel_->SetAnimation(static_cast<int>(PlayerAnimType::Jump), false, true);
}

//UIの設定
void SceneTitle::UISetting()
{
	//短縮化
	auto& file = ExternalFile::GetInstance();

	//UIを表示する座標を取得
	menuDrawPos_["SE"]				= file.GetUIPos("SEUIPos");
	menuDrawPos_["BGM"]				= file.GetUIPos("BGMUIPos");
	menuDrawPos_["終了"]			= file.GetUIPos("endDrawPos");
	menuDrawPos_["設定"]			= file.GetUIPos("settingDrawPos");
	menuDrawPos_["戻る"]			= file.GetUIPos("backUIPos");
	menuDrawPos_["モード"]			= file.GetUIPos("windowModeUIPos");
	menuDrawPos_["タイトル"]		= file.GetUIPos("titleDrawPos");
	menuDrawPos_["操作設定"]		= file.GetUIPos("advancedSettingUIPos");
	menuDrawPos_["シーン選択"]		= file.GetUIPos("continueDrawPos");
	menuDrawPos_["ニューゲーム"]	= file.GetUIPos("startDrawPos");

	//UI文字列の作成
	menuName_.push_back("ニューゲーム");
	menuName_.push_back("シーン選択");
	menuName_.push_back("設定");
	menuName_.push_back("終了");

	//UI画像の作成
	fontHandle_ = FontsManager::GetInstance().GetFontHandle("ピグモ 0033");

	//UImenuの作成
	float y = UI_menu_init_pos_y;
	for (const auto& menu : menuName_)
	{
		UI_->AddMenu(static_cast<float>(Game::screen_width / screen_half), static_cast<float>(Game::screen_height / screen_half) + y, UI_pos_x, UI_pos_y, menu.c_str(), fontHandle_);
		y += UI_menu_gap_size;
	}
}

//ライトの設定
void SceneTitle::LightSetting()
{
	SetUseLighting(true);

	//ライトのディフューズカラーの設定
	SetLightDifColor(direction_light_color);

	//ディレクションライト角度取得
	VECTOR dir = direction_light_diagonally_lower_left;
	dir = MathUtil::VECTORDegreeToRadian(dir);

	//ディレクションライトの作成
	directionLightHandle_ = CreateDirLightHandle(dir);
	directionLightDir_ = dir;
	SetLightDifColorHandle(directionLightHandle_, direction_light_color);

	//スポットライトの作成
	outAngle_ = MathUtil::DegreeToRadian(spot_light_out_angle);
	inAngle_ = MathUtil::DegreeToRadian(spot_light_in_angle);

	spotLightHandle_ = CreateSpotLightHandle(lightBulb_->GetFramePos(), lightBulb_->GetRotVec(),
											 outAngle_, inAngle_,
											 stop_light_range, 
											 attenuation_regardless_of_distance,
											 attenuation_proportional_to_distance,
											 attenuation_proportional_to_the_square_of_the_distance);

	SetLightDifColorHandle(spotLightHandle_, spot_light_color);
}


//カメラの設定
void SceneTitle::CameraPositionDataSetting()
{
	//短縮化
	auto& file = ExternalFile::GetInstance();

	//カメラの初期化
	camera_->Init(camera_init_pos);

	//カメラのポジションと見る位置の設定
	camera_->SetCameraTargetPosAndView(file.GetCameraTargetPos("start"), file.GetCameraTargetPos("startTargetPos"), normal_up_vec);

	//カメラ情報の設定
	CameraInfo info = {};

	//設定オブジェクトを見るカメラ
	info.targetPos = file.GetCameraTargetPos("setting");
	info.targetView = file.GetCameraTargetPos("settingTargetPos");
	info.upVec = normal_up_vec;

	cameraInfo_.push_back(info);

	//ベッドを見るカメラ
	info.targetPos = file.GetCameraTargetPos("start");
	info.targetView = file.GetCameraTargetPos("startTargetPos");
	info.upVec = normal_up_vec;

	cameraInfo_.push_back(info);

	//本を見るカメラ
	info.targetPos = file.GetCameraTargetPos("continue");
	info.targetView = file.GetCameraTargetPos("continueTargetPos");
	info.upVec = book_up_vec;

	cameraInfo_.push_back(info);

	//扉を見るカメラ
	info.targetPos = file.GetCameraTargetPos("end");
	info.targetView = file.GetCameraTargetPos("endTargetPos");
	info.upVec = normal_up_vec;

	cameraInfo_.push_back(info);
}

//カメラの目標座標の更新
void SceneTitle::CameraTargetUpdate()
{
	//カメラの目標座標の更新
	camera_->SetCameraTargetPosAndView(	cameraInfo_[static_cast<int>(currentSelectScene_)].targetPos,
										cameraInfo_[static_cast<int>(currentSelectScene_)].targetView,
										cameraInfo_[static_cast<int>(currentSelectScene_)].upVec);
	//経過時間のリセット
	camera_->ResetElapsedTime();
}

//選択番号の更新
void SceneTitle::SelectSceneUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//キーが押されたか
	bool isTriggerUp	= input.IsTriggered(InputType::Up);
	bool isTriggerDown	= input.IsTriggered(InputType::Down);
	bool isTriggerLeft	= input.IsTriggered(InputType::Left);
	bool isTriggerRight = input.IsTriggered(InputType::Right);

	//指定の方向に移動可能か
	bool canMoveUp		= selectData_[static_cast<int>(currentSelectScene_)].up;
	bool canMoveDown	= selectData_[static_cast<int>(currentSelectScene_)].down;
	bool canMoveLeft	= selectData_[static_cast<int>(currentSelectScene_)].left;
	bool canMoveRight	= selectData_[static_cast<int>(currentSelectScene_)].rignt;

	//上
	if (isTriggerUp && canMoveUp)
	{
		oldSelectScene_ = currentSelectScene_;
		currentSelectScene_ = selectData_[static_cast<int>(currentSelectScene_)].upScene;
		CameraTargetUpdate();
	}
	//下
	else if (isTriggerDown && canMoveDown)
	{
		oldSelectScene_ = currentSelectScene_;
		currentSelectScene_ = selectData_[static_cast<int>(currentSelectScene_)].downScene;
		CameraTargetUpdate();
	}
	//左
	else if (isTriggerLeft && canMoveLeft)
	{
		oldSelectScene_ = currentSelectScene_;
		currentSelectScene_ = selectData_[static_cast<int>(currentSelectScene_)].leftScene;
		CameraTargetUpdate();
	}
	//右
	else if (isTriggerRight && canMoveRight)
	{
		oldSelectScene_ = currentSelectScene_;
		currentSelectScene_ = selectData_[static_cast<int>(currentSelectScene_)].rigntScene;
		CameraTargetUpdate();
	}
}

//フェードインの更新
void SceneTitle::FadeInUpdate()
{
	//フェードイン
	float timer = (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_));
	UIfadeValue_ = static_cast <int>(max_fade_value * timer);
	if (++fadeTimer_ == fadeInterval_)
	{
		updateFunc_ = &SceneTitle::UIUpdate;
		UIfadeValue_ = max_fade_value;
		return;
	}
}

//UIの更新
void SceneTitle::UIUpdate()
{
	//短縮化
	auto& file = ExternalFile::GetInstance();
	auto& input = InputState::GetInstance();

	if (camera_->GetMoving()) 
	{
		return;
	}

	if (oldSelectScene_ != currentSelectScene_)
	{
		SceneChange();
		oldSelectScene_ = currentSelectScene_;
	}

	//選択
	SelectSceneUpdate();

	if (input.IsTriggered(InputType::Space))
	{
		NewGameOrGameEndChange();
	}
}

//オープニングの更新
void SceneTitle::OpeningUpdate()
{
	//短縮化
	auto& sound = SoundManager::GetInstance();

	//アニメーション番号チェック
	bool checkWakeUpAnim = PlayerAnimType::WakeUp == static_cast<PlayerAnimType>(mainPlayerModel_->GetCurrentAnimNo());
	bool checkStopTimerAnim = PlayerAnimType::StopTimer == static_cast<PlayerAnimType>(mainPlayerModel_->GetCurrentAnimNo());
	bool checkStopTimerCancelAnim = PlayerAnimType::StopTimerCancel == static_cast<PlayerAnimType>(mainPlayerModel_->GetCurrentAnimNo());

	//モデルの更新
	mainPlayerModel_->Update();

	//時計をとめるアニメーションの後仰向けに戻る
	if (mainPlayerModel_->IsAnimEnd() && checkStopTimerAnim)
	{
		sound.StopSE("alarm");
		sound.PlaySE("stopAlarm");
		mainPlayerModel_->ChangeAnimation(static_cast<int>(PlayerAnimType::StopTimerCancel), false, false, anim_change_time);
	}

	//仰向けになるアニメーションが終わった後
	if (mainPlayerModel_->IsAnimEnd() && checkStopTimerCancelAnim)
	{
		mainPlayerModel_->ChangeAnimation(static_cast<int>(PlayerAnimType::WakeUp), false, false, anim_change_time);
	}

	//起き上がるアニメーションが終わったら
	if (mainPlayerModel_->IsAnimEnd() && checkWakeUpAnim)
	{
		updateFunc_ = &SceneTitle::SceneTitleFadeOutUpdate;
	}
}

//UIのフェードアウト
void SceneTitle::UIFadeOutUpdate()
{
	//UIのフェードアウト
	UIfadeValue_ = static_cast <int>(max_fade_value * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0)
	{
		UIfadeValue_ = 0;
		updateFunc_ = &SceneTitle::OpeningUpdate;
		SoundManager::GetInstance().PlaySE("alarm");
		return;
	}
}

//タイトルのフェードアウト
void SceneTitle::SceneTitleFadeOutUpdate()
{
	//フェードアウト
	fadeValue_ = static_cast <int>(max_fade_value * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_)
	{
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_)));
		return;
	}
}

//シーンを切り替える
void SceneTitle::SceneChange()
{
	if (currentSelectScene_ == SelectScene::Setting)
	{
		Init();
		manager_.PushFrontScene(std::shared_ptr<SceneBase>(std::make_shared<SettingSceneForSceneTitle>(manager_)));
		return;
	}

	if (currentSelectScene_ == SelectScene::SelectChapter)
	{
		Init();
		manager_.PushFrontScene(std::shared_ptr<SceneBase>(std::make_shared<SelectChapterScene>(manager_,objManager_->GetSpecificModel(ObjectType::Book))));
		return;
	}
}

//ニューゲーム又はゲームオーバーが選択された時の変更
void SceneTitle::NewGameOrGameEndChange()
{

	if (currentSelectScene_ == SelectScene::NewGame)
	{
		updateFunc_ = &SceneTitle::UIFadeOutUpdate;
	}

	if (currentSelectScene_ == SelectScene::End)
	{
		manager_.SetEndFlag(true);
		return;
	}
}
