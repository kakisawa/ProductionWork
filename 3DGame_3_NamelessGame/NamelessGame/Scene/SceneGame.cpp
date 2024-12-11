﻿#include "SceneGame.h"
#include "SceneDebug.h"
#include "SceneSelect.h"
#include "SceneGameClear.h"
#include "SceneGameOver.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
#include "../Object/Item.h"
#include "../Object/Camera.h"
#include "../Object/Map.h"
#include "../UI/UISceneGame.h"
#include "../Time.h"
#include "../Fade.h"
#include "DxLib.h"

using namespace MyInputInfo;

namespace
{
	constexpr int kGameClearPosX = 450;	// ゲームクリアロゴ画像座標
	constexpr int kGameClearPosY = 50;

	constexpr int kScorePosX = 475;		// ゲームクリア時スコア画像座標
	constexpr int kScorePosY = 755;

	constexpr int kDefeatedTimePosX = 427;	// ゲームクリア時間画像座標
	constexpr int kDefeatedTimePosY = 870;

	constexpr int kPressAPosX = 700;	// 「Aボタンで進む」画像座標
	constexpr int kPressAPosY = 1000;

	int count = 0;		// ゲームクリア演出時のシーン遷移用カウント
	// 3回Aボタンがクリックされたらセレクトシーンへ遷移する
	int second = 100;	// ゲームクリア演出時のロゴ画像を映すまでの待機時間

	float pos = 1.7f;	// ゲームクリアロゴ画像拡大表示演出用
}

SceneGame::SceneGame() :
	m_gameClearHandle(-1),
	m_pressAHandle(-1),
	m_defeatedTimeHandle(-1),
	m_scoreHandle(-1),
	m_isPause(false),
	m_isPlayBGM(true)
{
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	m_pItem->Init();
	m_pCamera->Init();
	m_pPlayer->Init();
	m_pEnemy->Init();
	m_pMap->Init();
	m_pUI->Init(*m_pPlayer, *m_pEnemy);
	m_pTime->Init();
	m_pSound->InitBGM();

	// BGMのロード
	m_pSound->LoadBGM(SoundManager::BGM_Type::kGameBGM);
	m_pSound->LoadBGM(SoundManager::BGM_Type::kGameClearBGM);
	// 通常バトル時のBGMを流す
	m_pSound->PlayBGM(SoundManager::BGM_Type::kGameBGM, DX_PLAYTYPE_LOOP);

	// 画像の読み込み
	m_gameClearHandle = LoadGraph("Data/Image/SceneGame/Clear/GameClear.png");
	m_pressAHandle = LoadGraph("Data/Image/SceneGame/Clear/PressA.png");
	m_defeatedTimeHandle = LoadGraph("Data/Image/SceneGame/Clear/DefeatedTime.png");
	m_scoreHandle = LoadGraph("Data/Image/SceneGame/Clear/Score.png");
	m_gameOverHandle = LoadGraph("Data/Image/SceneGame/Over/GameOver.png");
}

std::shared_ptr<SceneBase> SceneGame::Update(Input& input)
{
	// スタートボタンを押したらポーズ状態にする
	if (input.IsTrigger(InputInfo::DebugStart)) {
		if (m_isPause) {
			m_isPause = false;
		}
		else {
			m_isPause = true;
		}
	}

		// エネミーが死んでいなければ
	if (!m_pEnemy->GetDeathFlag())
	{
		if (!m_pPlayer->GetDeathFlag()||m_pTime->GetTimeUp()) {
			m_pFade->FadeIn(true);
			m_pMap->Update();
			m_pPlayer->Update(*m_pEnemy, *m_pItem, *m_pCamera, input);
			m_pEnemy->Update(*m_pMap, *m_pPlayer);
			m_pCamera->Update(*m_pPlayer);
			m_pItem->Update();
			m_pUI->Update(*m_pPlayer, *m_pEnemy);
			m_pTime->Update();
		}
	}

	// プレイヤーが死亡したら・制限時間を過ぎてしまったら
	if (m_pPlayer->GetDeathFlag() || m_pTime->GetTimeUp()) {
		// フェード演出を行う
		m_pFade->HarfFade(true);
		// フェード演出が終わったら、BGMを変更する
		if (!m_pFade->GetHarfFadeFlag()) {
			if (m_isPlayBGM) {
			m_pSound->StopBGM(SoundManager::BGM_Type::kGameBGM);
			m_isPlayBGM = false;
			}

			if (input.IsTrigger(InputInfo::OK))
			{
				return std::make_shared<SceneSelect>();	// ゲームオーバーシーンへ行く
			}
		}
	}

	// エネミーを倒したら
	if (m_pEnemy->GetDeathFlag())
	{
		// フェード演出を行う
		m_pFade->HarfFade(true);
		// フェード演出が終わったら、BGMを変更する
		if (!m_pFade->GetHarfFadeFlag()) {
			m_pSound->StopBGM(SoundManager::BGM_Type::kGameBGM);
			if (m_isPlayBGM) {
				m_pSound->PlayBGM(SoundManager::BGM_Type::kGameClearBGM, DX_PLAYTYPE_LOOP);
				m_isPlayBGM = false;
			}
			// ゲームクリア時の演出
			second--;
			if (second <= 0) {
				pos -= 0.03f;
				if (pos <= 1.0f)
				{
					// Aボタンが押されたら、次の処理を行う
						// 3回押されたらセレクトシーンへ遷移する
					if (input.IsTrigger(InputInfo::OK))
					{
						count++;
					}

					if (count >= 3)
					{
						return std::make_shared<SceneSelect>();	// ゲームセレクトシーンへ行く
					}
				}
			}

		}
	}


#ifdef _DEBUG
	//if (input.IsTrigger(InputInfo::DebugStart)) {			// STARTボタン

	//	return std::make_shared<SceneDebug>();	// ゲームシーンへ行く
	//}
#endif // DEBUG

	return shared_from_this();
}

void SceneGame::Draw()
{
	m_pMap->Draw();
	m_pItem->Draw();
	m_pPlayer->Draw();
	m_pEnemy->Draw();
	m_pUI->Draw();
	m_pTime->Draw();

	m_pFade->Draw();

	// プレイヤーが死亡orタイムアップしたら
	if (m_pPlayer->GetDeathFlag()) 
	{
		if (!m_pFade->GetHarfFadeFlag()) {
			DrawGraph(kGameClearPosX, kGameClearPosY, m_gameOverHandle, true);
			DrawGraph(kPressAPosX, kPressAPosY, m_pressAHandle, true);
		}
	}


	// 敵が死亡したら
	if (m_pEnemy->GetDeathFlag()) {

		if (!m_pFade->GetHarfFadeFlag()) {
			if (second <= 0) {
				// ゲームクリアロゴを描画する
				DrawRotaGraph3(kGameClearPosX, kGameClearPosY, 0, 0,
					std::max(1.0f, pos), std::max(1.0f, pos),
					0, m_gameClearHandle, true, false);
				if (pos <= 1.0f)
				{
					DrawGraph(kPressAPosX, kPressAPosY, m_pressAHandle, true);
				}
			}
			// Aボタンを押すごとに画像を追加する
			if (count >= 1) {
				DrawGraph(kScorePosX, kScorePosY, m_scoreHandle, true);
				if (count >= 2) {
					DrawGraph(kDefeatedTimePosX, kDefeatedTimePosY, m_defeatedTimeHandle, true);
					m_pTime->DrawClearTime();
				}
			}

		}
	}

#ifdef DEBUG
	//DrawString(0, 0, "SceneGame", 0xffffff);
#endif // DEBUG
}

void SceneGame::End()
{
	m_pUI->End();
	m_pSound->ReleaseSound();

	DeleteGraph(m_gameClearHandle);
	DeleteGraph(m_pressAHandle);
	DeleteGraph(m_defeatedTimeHandle);
	DeleteGraph(m_scoreHandle);
	DeleteGraph(m_gameOverHandle);
}