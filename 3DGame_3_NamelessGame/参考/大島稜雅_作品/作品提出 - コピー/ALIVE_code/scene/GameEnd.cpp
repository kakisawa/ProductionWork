#include "GameEnd.h"
#include "SceneTitle.h"
#include "SceneManager.h"

#include "../object/Camera.h"
#include "../object/objectManager.h"

#include "../util/game.h"
#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/ModelManager.h"
#include "../util/ExternalFile.h"
#include "../util/FontsManager.h"
#include "../util/SoundManager.h"

#include <algorithm>

namespace 
{
	//半分
	constexpr int half = 2;

	//過去5回の記録を残すが1/5は違う処理をするため4回にする
	constexpr int old_result_num = 4;

	//補正値
	constexpr int correction = 3;

	//走るアニメーションの足音を鳴らすフレーム数
	constexpr int run_anim_foot_step_frame_type_1 = 6;
	constexpr int run_anim_foot_step_frame_type_2 = 45;

	//正面を向いた時のアニメーションフレーム数
	constexpr int front_anim_frame = 30;

	//進行方向を向いた時のアニメーションフレーム数
	constexpr int direction_of_travel_anim_frame = 40;

	//一回の動作で置く死体の数
	constexpr int number_placed_in_first_order_action = 5;

	//アニメーションを変更するのにかかる時間
	constexpr int anim_change_time = 10;

	//フェードの最大値
	constexpr int max_fade_value = 255;

	//色を変えるマテリアルの番号
	constexpr int change_material_num = 8;

	//棒グラフの横のサイズ
	constexpr int bar_graph_width = 20;

	//棒グラフの縦のサイズ
	constexpr int bar_graph_height = 10;

	//横の分割数
	constexpr int division_width = 10;

	//縦の分割数
	constexpr int division_height = 15;

	//棒グラフの1メモリのサイズ
	constexpr int bar_graph_1_memory_size = 12;

	//文字列の描画位置の高さ補正値
	constexpr int str_height_correction = 15;

	//文字列の描画位置の横の補正値
	constexpr int str_width_correction = 10;

	//ライトの外側の角度
	constexpr float light_outer_angle = 130.0f;

	//ライトの内側の角度
	constexpr float light_inner_angle = 60.0f;

	//ライトの範囲
	constexpr float light_range = 500.0f;

	//距離関係なく減衰するパラメーター
	constexpr float attenuation_regardless_of_distance = 0.0f;

	//距離に比例して減衰するパラメーター
	constexpr float attenuation_proportional_to_distance = 0.0009f;

	//距離の2乗に比例して減衰するパラメーター
	constexpr float attenuation_proportional_to_the_square_of_the_distance = 0.0f;

	//音が聞こえる範囲
	constexpr float sound_fange = 800.0f;

	//死体の高さの間隔
	constexpr float corpse_height_gap = 15.0f;

	//死体に正面を向かせる角度
	constexpr float corpse_front_angle = -90.0f;

	//死体を置く場所の補正値
	constexpr float corpse_put_pos = 50.0f;

	//初期カメラポジション
	constexpr VECTOR camera_init_pos = { 265, 314, -803 };

	//初期カメラのターゲットポジション
	constexpr VECTOR camera_init_target_pos = { 265, 314, 1777 };

	//スポットライトのポジション
	constexpr VECTOR spot_light_pos = { 235.0f,650.0f,-432.0f };

	//ライトの方向
	constexpr VECTOR light_dir = { 0.0f, -1.0f, 0.0f };

	//死体の色
	constexpr COLOR_F corpse_color = { 1.0f, 0.0f, 0.0f, 1.0f };
}

//コンストラクタ
GameEnd::GameEnd(SceneManager& manager) : SceneBase(manager),updateFunc_(&GameEnd::FadeInUpdate)
{
	//短縮化
	auto& model = ModelManager::GetInstance();
	auto& file = ExternalFile::GetInstance();
	auto& font = FontsManager::GetInstance();
	auto& sound = SoundManager::GetInstance();

	//インスタンス化
	objManager_ = std::make_shared<ObjectManager>();
	camera_ = std::make_shared<Camera>(camera_init_pos, camera_init_target_pos);
	playerModel_ = std::make_shared<Model>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Player)].name), Material::Other);
	boardModel_  = std::make_shared<Model>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::WhiteBoard)].name), Material::Iron);

	//モデルの設定
	ModelSetting();

	//オブジェクトの生成
	objManager_->EndStageObjectGenerator();

	//棒グラフの初期高さ
	for (int i = 0; i < file.GetTotalDeathNum().size(); i++)
	{
		barGraphHeight_.push_back(Game::screen_height / division_height * bar_graph_1_memory_size);
	}

	//死体を置く場所を画面を分割して決める
	//画面の分割数の取得
	divisionNum_ = file.GetTotalDeathNum().back() / static_cast<int>(file.GetTotalDeathNum().size()) + 1;
	if (file.GetTotalDeathNum().back() % file.GetTotalDeathNum().size() > 0)
	{
		divisionNum_++;
	}

	//フォントハンドルの取得
	pigumo21FontHandle_ = font.GetFontHandle("ピグモ 0021");
	pigumo42FontHandle_ = font.GetFontHandle("ピグモ 0042");

	//外角を弧度法に変換する
	float outerAngle = MathUtil::DegreeToRadian(light_outer_angle);

	//内角を弧度法に変換する
	float innerAngle = MathUtil::DegreeToRadian(light_inner_angle);

	//スポットライトハンドルの作成
	lightHandle_ = CreateSpotLightHandle(spot_light_pos, light_dir,
										 outerAngle, innerAngle,
										 light_range,
										 attenuation_regardless_of_distance,
										 attenuation_proportional_to_distance,
										 attenuation_proportional_to_the_square_of_the_distance);

	//3Dサウンドリスナーの情報を設定する
	sound.Set3DSoundListenerInfo(camera_->GetPos(), camera_->GetTarget());
}

//デストラクタ
GameEnd::~GameEnd()
{
}

//初期化
void GameEnd::Init()
{
}

//終了
void GameEnd::End()
{
}

//更新
void GameEnd::Update()
{
	(this->*updateFunc_)();
}

//描画
void GameEnd::Draw()
{
	//短縮化
	auto& file = ExternalFile::GetInstance();

	camera_->Init(camera_init_target_pos);

	//プレイヤーモデルの描画
	playerModel_->Draw();

	//ホワイトボードの描画
	boardModel_->Draw();

	//死体モデルの描画
	for (const auto& corpse : corpseModel_)
	{
		corpse.second->Draw();
	}

	//オブジェクトの描画
	objManager_->Draw();

	//プレイヤーの座標をスクリーン座標にする
	VECTOR screenPos = ConvWorldPosToScreenPos(boardModel_->GetPos());

	//int型のスクリーン座標
	int castScreenPosX = static_cast<int>(screenPos.x);

	//色
	int color = 0x0000ff;

	for (int i = 0; i < file.GetTotalDeathNum().size();i++)
	{

		barGraphHeight_[i] = (std::max)(barGraphHeight_[i] - 1, Game::screen_height / division_height * bar_graph_1_memory_size - file.GetTotalDeathNum()[i] * bar_graph_height);

		if (static_cast<int>(file.GetTotalDeathNum().size() - 1) - i == 0) 
		{
			color = 0xff0000;
		}

		//棒グラフの描画
		DrawBox(Game::screen_width / division_width * (i + correction) - bar_graph_width - (Game::screen_width / half - castScreenPosX),
				barGraphHeight_[i],
				Game::screen_width / division_width * (i + correction) + bar_graph_width - (Game::screen_width / half - castScreenPosX),
				Game::screen_height / division_height * bar_graph_1_memory_size,
				color, true);

		//フォントを適用した文字列のサイズを取得
		int size = GetDrawFormatStringWidthToHandle(pigumo42FontHandle_,"%d体", file.GetTotalDeathNum()[i]);

		//数字の描画
		DrawFormatStringToHandle(Game::screen_width / division_width * (i + correction) - size / half - (Game::screen_width / half - castScreenPosX),
								 Game::screen_height / division_height * bar_graph_1_memory_size - file.GetTotalDeathNum()[i] * bar_graph_height - size,
								 0x000000, pigumo42FontHandle_, "%d体", file.GetTotalDeathNum()[i]);

		if (old_result_num - i == 0)
		{
			DrawVStringToHandle(Game::screen_width / division_width * (i + correction) - str_width_correction - (Game::screen_width / half - castScreenPosX),
								Game::screen_height / division_height * bar_graph_1_memory_size + str_height_correction,
								"今回", 0xff0000, pigumo21FontHandle_);
			continue;
		}

		DrawFormatVStringToHandle(Game::screen_width / division_width * (i + correction) - str_width_correction - (Game::screen_width / half - castScreenPosX),
								  Game::screen_height / division_height * bar_graph_1_memory_size + str_height_correction,
								  0x000000,
								  pigumo21FontHandle_,
								  "%d回前", old_result_num - i);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//画面全体を真っ黒に塗りつぶす
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//モデルの設定
void GameEnd::ModelSetting()
{
	//短縮化
	auto& file = ExternalFile::GetInstance();
	auto& model = ModelManager::GetInstance();

	//モデルの設定
	auto playerInfo = file.GetEndSpecifiedInfo("player");
	playerModel_->SetPos(playerInfo.pos);
	playerModel_->SetRot(playerInfo.rot);
	playerModel_->SetScale(playerInfo.scale);
	//アニメーションの変更
	playerModel_->SetAnimation(static_cast<int>(PlayerAnimType::Run), true, true);

	//モデルの設定
	auto boardInfo = file.GetEndSpecifiedInfo("WhiteBoard");
	boardModel_->SetPos(boardInfo.pos);
	boardModel_->SetRot(boardInfo.rot);
	boardModel_->SetScale(boardInfo.scale);

	//プレイヤーの手の座標を取得
	VECTOR pos = playerModel_->GetFrameLocalPosition("hand.R_end");
	float height = 0.0f;

	//死体モデルの生成
	for (int i = 0; i < file.GetTotalDeathNum().back(); i++)
	{
		corpseModel_.push_back(std::make_pair(false, std::make_shared<Model>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Player)].name), Material::Other)));
		corpseModel_[i].second->SetPos({ pos.x,pos.y + height,pos.z });
		corpseModel_[i].second->SetRot(playerInfo.rot);
		corpseModel_[i].second->SetScale(playerInfo.scale);
		corpseModel_[i].second->SetAnimEndFrame(static_cast<int>(PlayerAnimType::Death));
		//マテリアルの色を変える
		MV1SetMaterialDifColor(corpseModel_[i].second->GetModelHandle(), change_material_num, corpse_color);
		height += corpse_height_gap;
	}
}

//死体の更新
void GameEnd::CorpseUpdate(const VECTOR& playerPos)
{
	//死体の高さの補正
	float height = 0.0f;

	//死体のモデルを回転させる
	if (isTurn_) 
	{
		for (int i = 0; i < corpseModel_.size(); i++) 
		{
			if (corpseModel_[i].first)
			{
				continue;
			}
			VECTOR rot = {};
			rot.y = MathUtil::DegreeToRadian(0);
			corpseModel_[i].second->SetRot(rot);
		}
	}

	//アニメーションが終了したら、足元に死体を置く
	if (playerModel_->IsAnimEnd()) 
	{
		for (int i = corpseNum_; i < corpseNum_ + number_placed_in_first_order_action; i++)
		{
			//現在の番号が死体の総数を超えたら以降の処理を行わない
			if (i > static_cast<int>(corpseModel_.size() - 1)) 
			{
				continue;
			}

			//死体の座標をプレイヤーの座標に設定する
			corpseModel_[i].second->SetPos({ playerPos.x,playerPos.y + height, playerPos.z });

			//フラグをtrueにする
			corpseModel_[i].first = true;

			//高さを変更する
			height += corpse_height_gap;
		}

		//まだ置かれていない死体のモデルを回転させる
		for (int i = 0; i < corpseModel_.size(); i++) 
		{
			if (corpseModel_[i].first) 
			{
				continue;
			}

			VECTOR rot = {};
			rot.y = MathUtil::DegreeToRadian(corpse_front_angle);
			corpseModel_[i].second->SetRot(rot);
		}

		//死体を5個足元に置いた後、まだ手元に死体が残っていたら
		//操作する死体の番号範囲を変える
		if ((static_cast<int>(corpseModel_.size()) - 1 - corpseNum_ + 1) / number_placed_in_first_order_action > 0)
		{
			corpseNum_ += number_placed_in_first_order_action;
		}

		//フラグを折る
		isPutting_ = false;
	}

	//プレイヤーモデルの手の座標を取得
	VECTOR pos = playerModel_->GetFrameLocalPosition("hand.R_end");
	height = 0.0f;

	//死体モデルの座標を変更
	for (int i = 0; i < corpseModel_.size(); i++)
	{
		if (corpseModel_[i].first) 
		{
			continue;
		}

		corpseModel_[i].second->SetPos({ pos.x,pos.y + height,pos.z });
		height += corpse_height_gap;
	}
}

//ホワイトボードの更新
void GameEnd::WhiteBoardUpdate()
{
	//短縮化
	auto& sound = SoundManager::GetInstance();

	if (isWentToRightSide_) 
	{
		return;
	}

	if (!isResultDisplaying_)
	{
		boardIsMoving_ = true;
		boardModel_->SetPos(VAdd(boardModel_->GetPos(), moveVec_));
		if (!sound.CheckSoundFile("pull")) 
		{
			sound.PlaySE("pull");
		}
	}

	//プレイヤーの座標をスクリーン座標にする
	VECTOR screenPos = ConvWorldPosToScreenPos(boardModel_->GetPos());

	//int型のスクリーン座標
	int castScreenPosX = static_cast<int>(screenPos.x);

	//画面の半分のサイズ(横)
	int screenWidthHalf = Game::screen_width / half;

	if (castScreenPosX >= screenWidthHalf - 1 && castScreenPosX <= screenWidthHalf + 1)
	{
		boardIsMoving_ = false;
		isResultDisplaying_ = true;
		playerModel_->ChangeAnimation(static_cast<int>(PlayerAnimType::Idle), true, false, anim_change_time);
	}

	//ホワイトボードが画面外に出たらフェードアウトする
	if (castScreenPosX < -Game::screen_width / half)
	{
		updateFunc_ = &GameEnd::FadeOutUpdate;
	}
}

//フェードインの更新
void GameEnd::FadeInUpdate()
{
	fadeValue_ = static_cast <int>(max_fade_value * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0)
	{
		updateFunc_ = &GameEnd::NormalUpdate;
		fadeValue_ = 0;
	}
}

//通常時の更新
void GameEnd::NormalUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();
	auto& file = ExternalFile::GetInstance();
	auto& sound = SoundManager::GetInstance();

	//プレイヤーモデルの座標
	VECTOR playerPos = playerModel_->GetPos();

	//プレイヤーモデルの更新
	playerModel_->Update();

	//プレイヤーの座標をスクリーン座標にする
	VECTOR screenPos = ConvWorldPosToScreenPos(playerModel_->GetPos());

	//プレイヤーが画面外に行ったら動いているフラグをfalseにする
	if (screenPos.x > Game::screen_width && isWentToRightSide_) 
	{
		isWentToRightSide_ = false;
		moveVec_.x = -1;
		playerModel_->SetPos(file.GetEndSpecifiedInfo("secondPos").pos);
		playerModel_->ChangeAnimation(static_cast<int>(PlayerAnimType::Pull), true, false, anim_change_time);
		playerPos = playerModel_->GetPos();
	}

	if (!isPutting_ && !isResultDisplaying_) 
	{
		//プレイヤーモデルの座標変更
		playerModel_->SetPos(VAdd(playerPos, moveVec_));
		if (playerModel_->GetSpecifiedAnimTime(run_anim_foot_step_frame_type_1) || playerModel_->GetSpecifiedAnimTime(run_anim_foot_step_frame_type_2))
		{
			sound.Set3DSoundInfo(playerPos, sound_fange, "asphaltStep");
			sound.PlaySE("asphaltStep");
		}
	}

	//ホワイトボードの更新
	WhiteBoardUpdate();

	if (input.IsTriggered(InputType::Space) && isResultDisplaying_) 
	{
		isResultDisplaying_ = false;
	}

	if (!isWentToRightSide_)
	{
		return;
	}

	//画面を分割した後の一つ当たりの横の長さ
	int divisionPosX = Game::screen_width / divisionNum_;

	//プレイヤーの座標を元に取得したスクリーン座標を
	//上記のサイズで余りを求める
	int errorRange = static_cast<int>(screenPos.x) % (divisionPosX * currentDivisionNum_);

	if (screenPos.x > corpse_put_pos && screenPos.x < Game::screen_width - corpse_put_pos)
	{
		//誤差範囲内だったら、プレイヤーのアニメーションを変更する
		if (errorRange >= -1 && errorRange <= 1 && !isPutting_) 
		{
			//アニメーションを変更する
			playerModel_->ChangeAnimation(static_cast<int>(PlayerAnimType::Put), false, true, anim_change_time);

			//フラグを立てる
			isPutting_ = true;

			currentDivisionNum_++;
		}
	}

	//プレイヤーが特定のアニメーションフレームになったらフラグを立てる
	if (isPutting_) 
	{
		if (playerModel_->GetSpecifiedAnimTime(front_anim_frame))
		{
			isTurn_ = true;
		}
		else if (playerModel_->GetSpecifiedAnimTime(direction_of_travel_anim_frame))
		{
			isTurn_ = false;
		}
	}

	//死体の更新
	CorpseUpdate(playerPos);

	if (!isPutting_)
	{
		//アニメーションを変更する
		playerModel_->ChangeAnimation(static_cast<int>(PlayerAnimType::Run), true, false, anim_change_time);
	}
}

//フェードアウトの更新
void GameEnd::FadeOutUpdate()
{
	fadeValue_ = static_cast <int>(max_fade_value * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) 
	{
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
		return;
	}
}
