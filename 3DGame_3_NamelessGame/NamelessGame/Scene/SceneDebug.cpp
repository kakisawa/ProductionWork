﻿#include "SceneDebug.h"
#include "SceneTitle.h"
#include "SceneSelect.h"
#include "SceneOption.h"
#include "SceneExplanation.h"
#include "SceneRanking.h"
#include "SceneGame.h"
#include "SceneGameClear.h"
#include "SceneGameOver.h"

namespace {
	constexpr int kSelectBasePosX = 300;				// シーン表示X座標
	constexpr int kSelectBasePosY = 300;				// シーン表示Y座標
	constexpr int kSelectMoveY = 20;					// シーン表示Y座標の移動量

	constexpr int kSelectBoxX = kSelectBasePosX -2;		// 選択中シーン表示BoxX座標
	constexpr int kSelectBoxY = kSelectBasePosY - 2;	// 選択中シーン表示BoxX座標
	constexpr int kSelectWidth = 150;					// 選択中シーン表示Boxの幅

	constexpr int kSceneNum = 8;						// 選択できるシーンの数

	constexpr float ks = kSelectBoxY + kSelectMoveY * kSceneNum;
}

SceneDebug::SceneDebug() :
	m_nextScene(nextScene::kGameScene)
{
}

void SceneDebug::Init()
{
	// 選択中のシーンを表示するBoxの初期位置
	m_selectBox.selectPos = VGet(kSelectBoxX, kSelectBasePosY + kSelectMoveY * 5, 0.0f);
}

std::shared_ptr<SceneBase> SceneDebug::Update(Input &input)
{
	if(input.IsTrigger(InputInfo::DebugStart)){				// STARTボタン
		if (m_nextScene == nextScene::kTitleScene)
		{
			return std::make_shared<SceneTitle>();	// タイトルシーンへ行く
		}
		if (m_nextScene == nextScene::kSelectScene)
		{
			return std::make_shared<SceneSelect>();	// セレクトシーンへ行く
		}
		if (m_nextScene == nextScene::kOptionScene)
		{
			return std::make_shared<SceneOption>();	// オプションシーンへ行く
		}
		if (m_nextScene == nextScene::kExplanationScene)
		{
			return std::make_shared<SceneExplanation>();	// 操作説明シーンへ行く
		}
		if (m_nextScene == nextScene::kRankingScene)
		{
			return std::make_shared<SceneRanking>();	// ランキングシーンへ行く
		}
		if (m_nextScene == nextScene::kGameScene)
		{
			return std::make_shared<SceneGame>();	// ゲームシーンへ行く
		}
		if (m_nextScene == nextScene::kGameClearScene)
		{
			return std::make_shared<SceneGameClear>();	// ゲームクリアシーンへ行く
		}
		if (m_nextScene == nextScene::kGameOverScene)
		{
			return std::make_shared<SceneGameOver>();	// ゲームオーバーシーンへ行く
		}
		if (m_nextScene == nextScene::kGameEnd)
		{
			DxLib_End();						// ゲームを終了する
		}
	}

	SwitchingScene(input);


	return shared_from_this();
}

void SceneDebug::Draw()
{
	// 現在のシーン
	DrawString(0, 0, "SceneDebug", 0xffffff);

	// 選択中のシーンを表示するBox
	DrawBoxAA(m_selectBox.selectPos.x, m_selectBox.selectPos.y,
		m_selectBox.selectPos.x + kSelectWidth, m_selectBox.selectPos.y + kSelectMoveY,
		0xff00ff,false);

	DrawFormatString(0, 500, 0xffffff, "m_selectBox.selectPos.y=%.2f", m_selectBox.selectPos.y);
	DrawFormatString(0, 520, 0xffffff, "ks=%.2f", ks);

	// 各シーン
	DrawString(kSelectBasePosX, kSelectBasePosY, "SceneTitle", 0xffffff);
	DrawString(kSelectBasePosX, kSelectBasePosY + kSelectMoveY, "SceneSelect", 0xffffff);
	DrawString(kSelectBasePosX, kSelectBasePosY + kSelectMoveY * 2, "SceneOption", 0xffffff);
	DrawString(kSelectBasePosX, kSelectBasePosY + kSelectMoveY * 3, "SceneExplanation", 0xffffff);
	DrawString(kSelectBasePosX, kSelectBasePosY + kSelectMoveY * 4, "SceneRanking", 0xffffff);
	DrawString(kSelectBasePosX, kSelectBasePosY + kSelectMoveY * 5, "SceneGame", 0xffffff);
	DrawString(kSelectBasePosX, kSelectBasePosY + kSelectMoveY * 6, "SceneGameClear", 0xffffff);
	DrawString(kSelectBasePosX, kSelectBasePosY + kSelectMoveY * 7, "SceneGameOver", 0xffffff);
	DrawString(kSelectBasePosX, kSelectBasePosY + kSelectMoveY * 8, "GameEnd", 0xffffff);
}

void SceneDebug::SwitchingScene(Input& input)
{
	// 下キーを押すと次のシーンの変更をする
	if (input.IsTrigger(InputInfo::Down))
	{
		// 選択中のシーンを表示するBoxの座標移動
		m_selectBox.selectPos = VAdd(m_selectBox.selectPos, VGet(0.0f, kSelectMoveY, 0.0f));
		if (m_selectBox.selectPos.y > kSelectBasePosY + kSelectMoveY * kSceneNum) {
			m_selectBox.selectPos.y = kSelectBoxY;
		}

		if (m_nextScene == nextScene::kTitleScene)
		{
			m_nextScene = nextScene::kSelectScene;
		}
		else if (m_nextScene == nextScene::kSelectScene)
		{
			m_nextScene = nextScene::kOptionScene;
		}
		else if (m_nextScene == nextScene::kOptionScene)
		{
			m_nextScene = nextScene::kExplanationScene;
		}
		else if (m_nextScene == nextScene::kExplanationScene)
		{
			m_nextScene = nextScene::kRankingScene;
		}
		else if (m_nextScene == nextScene::kRankingScene)
		{
			m_nextScene = nextScene::kGameScene;
		}
		else if (m_nextScene == nextScene::kGameScene)
		{
			m_nextScene = nextScene::kGameClearScene;
		}
		else if (m_nextScene == nextScene::kGameClearScene)
		{
			m_nextScene = nextScene::kGameOverScene;
		}
		else if (m_nextScene == nextScene::kGameOverScene)
		{
			m_nextScene = nextScene::kGameEnd;
		}
		else if (m_nextScene == nextScene::kGameEnd)
		{
			m_nextScene = nextScene::kTitleScene;
		}
	}

	// 上キーを押すと次のシーンの変更をする
	if (input.IsTrigger(InputInfo::Up))
	{
		// 選択中のシーンを表示するBoxの座標移動
		m_selectBox.selectPos = VAdd(m_selectBox.selectPos, VGet(0.0f, -kSelectMoveY, 0.0f));
		if (m_selectBox.selectPos.y < kSelectBoxY) {
			m_selectBox.selectPos.y = kSelectBoxY + kSelectMoveY * kSceneNum;
		}

		if (m_nextScene == nextScene::kTitleScene)
		{
			m_nextScene = nextScene::kGameEnd;
		}
		else if (m_nextScene == nextScene::kGameEnd)
		{
			m_nextScene = nextScene::kGameOverScene;
		}
		else if (m_nextScene == nextScene::kGameOverScene)
		{
			m_nextScene = nextScene::kGameClearScene;
		}
		else if (m_nextScene == nextScene::kGameClearScene)
		{
			m_nextScene = nextScene::kGameScene;
		}
		else if (m_nextScene == nextScene::kGameScene)
		{
			m_nextScene = nextScene::kRankingScene;
		}
		else if (m_nextScene == nextScene::kRankingScene)
		{
			m_nextScene = nextScene::kExplanationScene;
		}
		else if (m_nextScene == nextScene::kExplanationScene)
		{
			m_nextScene = nextScene::kOptionScene;
		}
		else if (m_nextScene == nextScene::kOptionScene)
		{
			m_nextScene = nextScene::kSelectScene;
		}
		else if (m_nextScene == nextScene::kSelectScene)
		{
			m_nextScene = nextScene::kTitleScene;
		}
	}
}
