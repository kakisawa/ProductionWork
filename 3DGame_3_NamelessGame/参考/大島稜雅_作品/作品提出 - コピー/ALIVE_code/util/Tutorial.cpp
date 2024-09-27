#include "Tutorial.h"
#include "InputState.h"
#include "ExternalFile.h"
#include "FontsManager.h"
#include "DrawFunctions.h"
#include "Util.h"
#include "game.h"

#include <algorithm>

namespace {
	//キーボード画像のチップサイズ
	constexpr int input_graph_chip_size = 59;

	//パッド画像のチップサイズ
	constexpr int controller_graph_chip_size = 59;

	//最大時間
	constexpr int max_time = 300;

	//画面の横幅を半分にする
	constexpr int screen_half_width = 2;

	//文字列の横幅を半分にする
	constexpr int str_half_width = 2;

	//サイズの半分
	constexpr int scale_half = 2;

	//画像サイズの拡縮率
	constexpr float graph_scale_size = 1.2f;

	//UI画像を表示する高さの補正
	constexpr float graph_correction_height = 1.3f;

	//UI文字列を表示する高さの補正
	constexpr float str_correction_height = 1.6f;
}

//コンストラクタ
Tutorial::Tutorial()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//キーボードの画像を描画する位置
	UIPos_[UIGraph::KeyBord].x = Game::screen_width / screen_half_width - input_graph_chip_size;
	UIPos_[UIGraph::KeyBord].y = Game::screen_height - input_graph_chip_size * graph_correction_height;

	//padのボタンを描画する位置
	UIPos_[UIGraph::XboxBotton].x = Game::screen_width / screen_half_width - controller_graph_chip_size;
	UIPos_[UIGraph::XboxBotton].y = Game::screen_height - controller_graph_chip_size * graph_correction_height;

	//クランクのチュートリアル
	tutorialData_["CrankTutorial"]	.push_back({ static_cast<int>(InputType::Activate)	,false , "クランクを回す" });
	tutorialData_["CrankTutorial"]	.push_back({ static_cast<int>(InputType::Max)		,false , "回す" });
	tutorialData_["CrankTutorial"]	.push_back({ static_cast<int>(InputType::Activate)	,false , "放す" });

	//死体を持てることについてのチュートリアル
	tutorialData_["SwitchTutorial"]	.push_back({ static_cast<int>(InputType::Death)		,false , "死亡" });
	tutorialData_["SwitchTutorial"] .push_back({ static_cast<int>(InputType::Activate)	,false , "死体を持つ" });
	tutorialData_["SwitchTutorial"] .push_back({ static_cast<int>(InputType::Activate)	,false , "死体を置く" });

	//走るチュートリアル
	tutorialData_["RunTutorial"]	.push_back({ static_cast<int>(InputType::Dush)		,false , "走る" });

	//ジャンプのチュートリアル
	tutorialData_["JumpTutorial"]	.push_back({ static_cast<int>(InputType::Space)		,false , "ジャンプ" });

	//エレベーターのチュートリアル
	tutorialData_["ElevatorTutorial"].push_back({ static_cast<int>(InputType::Activate)	,false , "レバーを引く" });

	//死体を足場として使うチュートリアル
	tutorialData_["CorpseScaffoldTutorial"].push_back({ static_cast<int>(InputType::Death)	,false , "死亡" });
	tutorialData_["CorpseScaffoldTutorial"].push_back({ static_cast<int>(InputType::Max)	,false , "死体は足場として使える" });

	//何も表示したくないとき
	tutorialData_["None"].push_back({ static_cast<int>(InputType::Max)	,false , "" });

	//フォントの取得
	fontPigumo42_ = FontsManager::GetInstance().GetFontHandle("ピグモ 0042");
}

//デストラクタ
Tutorial::~Tutorial()
{
}

//更新
void Tutorial::Update(const VECTOR& playerPos)
{
	//プレイヤーから一番近いチュートリアルの配置データを取得
	ObjectInfo tutorialInfo = ExternalFile::GetInstance().GetTutorialObjInfo(playerPos);

	//取得したデータのポジションからプレイヤーのポジションの距離のサイズを取得する
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(tutorialInfo.pos, playerPos);

	//取得したデータの距離のサイズを半分にする
	float range = VSize(tutorialInfo.scale) / scale_half;

	//チュートリアルの範囲内に入っていたら
	//一番近いチュートリアルの名前を取得する
	if (range > distanceSize)
	{
		//一番近いチュートリアルの名前を取得
		currentTutorialName_ = tutorialInfo.name;
	}
	else
	{
		currentTutorialName_ = "None";
	}

	//現在のチュートリアルの名前と1フレーム前のチュートリアルの名前が
	//違っていたらcurrentDisplayNum_に0をいれる
	if (currentTutorialName_ != oldTutorialName_)
	{
		currentDisplayNum_ = 0;
		elapsedTime_ = 0;
	}

	//1フレーム前のチュートリアルに使われていた名前を保存する
	oldTutorialName_ = currentTutorialName_;
}

//描画
void Tutorial::Draw()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//表示するチュートリアルの数を超えたら以降何も表示しない
	//currentDisplayNum_は0から始まるため数調整で1を追加する
	if (currentDisplayNum_ + 1 > static_cast<int>(tutorialData_[currentTutorialName_].size()))
	{
		return;
	}

	//現在のチュートリアルデータの取得
	TutorialData& data = tutorialData_[currentTutorialName_][currentDisplayNum_];

	//キータイプを表示するか
	if (DoNotDiplayKeyType(data))
	{
		//文字列の横幅を取得する
		int width = GetDrawStringWidthToHandle(data.str.c_str(), static_cast<int>(data.str.size()), fontPigumo42_);

		//キーの役割の描画
		DrawStringToHandle(Game::screen_width / screen_half_width - width / str_half_width, Game::screen_height - static_cast<int>(input_graph_chip_size * str_correction_height), data.str.c_str(), 0xffffff, fontPigumo42_);

		return;
	}

	//キーの役割の描画
	DrawStringToHandle(Game::screen_width / screen_half_width, static_cast<int>(Game::screen_height - input_graph_chip_size * str_correction_height), data.str.c_str(), 0xffffff, fontPigumo42_);

	if (input.LastInputDevice())
	{
		//キー画像の描画
		input.DrawKeyGraph(data.keyType, UIPos_[UIGraph::KeyBord].x, UIPos_[UIGraph::KeyBord].y, graph_scale_size);
	}
	else
	{
		input.DrawPadGraph(data.keyType, UIPos_[UIGraph::XboxBotton].x, UIPos_[UIGraph::XboxBotton].y, graph_scale_size);
	}
	

	//現在表示されているキーが押されたか
	if (input.IsTriggered(static_cast<InputType>(data.keyType)))
	{
		data.isPushKey = true;
		currentDisplayNum_++;
	}
}

//キータイプを表示しないか
bool Tutorial::DoNotDiplayKeyType(const TutorialData& data)
{
	//経過時間が最大時間と同じだったら現在の表示番号を増やす
	if (elapsedTime_ == max_time)
	{
		currentDisplayNum_++;
		elapsedTime_ = 0;
	}

	//keyタイプがmaxだったら経過時間を増やし、リターンする
	if (static_cast<int>(InputType::Max) == data.keyType)
	{
		elapsedTime_ = (std::min)(elapsedTime_ + 1, max_time);
		return true;
	}

	return false;
}