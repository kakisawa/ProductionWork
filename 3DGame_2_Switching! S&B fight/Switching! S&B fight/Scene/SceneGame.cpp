#include "SceneGame.h"
#include "SceneGameClear.h"
#include "SceneGameOver.h"
#include "SceneTitle.h"
#include "../Object/GameMap.h"
#include "../Object/Camera.h"
#include "../Object/Player/Player.h"
#include "../Object/Enemy/EnemyRight.h"
#include "../Object/Enemy/EnemyLeft.h"
#include "../Util/Pad.h"
#include "../Util/Fade.h"
#include <cassert>

namespace {
	constexpr int kExplanationInitPosX = 620;	// 操作説明画像の初期位置X
	constexpr int kExplanationInitPosY = 130;	// 操作説明画像の初期位置Y
	constexpr int kExplanationPosX = 1472;		// 操作説明画像の最終位置X
	constexpr int kExplanationPosY = 0;			// 操作説明画像の最終位置Y

	constexpr int kExplanationSizeX = 448;		// 操作説明画像のサイズX
	constexpr int kExplanationSizeY = 530;		// 操作説明画像のサイズY
	constexpr int kExplanationAddWidth = 16;	// 操作説明画像のX軸移動量
	constexpr int kExplanationSubHeight = 9;	// 操作説明画像のY軸移動量

	constexpr int kGameFinishCount = 60;		// クリア条件後、シーン遷移への待機時間
}

SceneGame::SceneGame() :
	m_explanationGraph(-1),
	m_waitCount(kGameFinishCount),
	m_isGameClearFlag(false),
	m_isExplanationFinishFlag(false),
	m_isExplanationSet(false),
	m_isExplanationView(true),
	m_isPause(false)
{
	// フェード
	m_pFade = std::make_shared<Fade>();
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	// 初期化処理
	m_pMap->Init();
	m_pPlayer->Init(m_pMap);
	m_pEnemyRight->Init(m_pMap);
	m_pEnemyLeft->Init(m_pMap);
	m_pCamera->Init();
	m_pSound->Init();	// サウンドの初期化

	// 使用サウンドのロード
	m_pSound->LoadBGM(SoundManager::BGM_Type::kGameBGM);	// サウンドの読み込み
	m_pSound->PlayBGM(SoundManager::BGM_Type::kGameBGM, DX_PLAYTYPE_LOOP);

	// 画像のロード
	m_explanationGraph = LoadGraph("data/Image/GameScene/UI/操作説明.png");
	m_pauseGraph= LoadGraph("data/Image/GameScene/UI/ポーズ表示画像.png");


	// 操作説明画像の位置設定
	size.m_width = kExplanationInitPosX;
	size.m_height = kExplanationInitPosY;
	size.m_widthMax = kExplanationInitPosX + 800;
	size.m_heightMax = kExplanationInitPosY + 800;
}

shared_ptr<SceneBase> SceneGame::Update()
{
	// スタートボタンを押したらポーズ状態にする
	if (Pad::IsTrigger(PAD_INPUT_R)) {	
		if (m_isPause) {
			m_isPause = false;
		}
		else {
			m_isPause = true;
		}
	}

	// ポーズ中にバックボタンを押すとタイトル画面に戻る
	if (Pad::IsTrigger(PAD_INPUT_L) && m_isPause) {
		return make_shared<SceneTitle>();	// ゲームクリアへ行く
	}

	// シーンに入った直後にフェードインを行う
	m_pFade->FadeIn(m_pFade->GatFadeInFlag());

	// ポーズ状態になっていない場合のみゲーム画面を更新する
	if (!m_isPause) {
		m_pCamera->Update(*m_pPlayer);

		// 操作説明画像の紹介が終わったら更新を行う
		if (m_isExplanationFinishFlag) {
			// シーン切り替え条件に当てはまったらフェードアウトを行う
			m_pFade->FadeOut(m_pFade->GatFadeOutFlag());	
			m_isNextSceneFlag = m_pFade->GatNextSceneFlag();

			m_pPlayer->Update(*m_pCamera, *m_pEnemyRight, *m_pEnemyLeft);
			m_pEnemyRight->Update(*m_pPlayer);
			m_pEnemyLeft->Update(*m_pPlayer);

			// ゲームクリア関係
			m_isGameClearFlag = m_pEnemyLeft->GetHp() <= 0 && m_pEnemyRight->GetHp() <= 0;	// ゲームクリア条件フラグ

			if (m_isGameClearFlag){						// ゲームクリア条件を達成したら
				m_waitCount--;							// すぐにシーン遷移をせず、m_waitCount分待つ
				if (m_waitCount <= 0) {					// カウントがゼロになったらフェードアウトを行うフラグを立てる
					m_pFade->SetFadeOutFlag(true);
					if (m_isNextSceneFlag){				// フェードアウトが終わったら、ゲームクリア画面に遷移する
						return make_shared<SceneGameClear>();
					}
				}
			}

			// ゲームオーバー関係
			if (m_pPlayer->GetDeathFlag())
			{
				m_pFade->SetFadeOutFlag(true);			// フェードアウトを行うフラグを立てる
				if (m_isNextSceneFlag){					// フェードアウトが終わったら、ゲームオーバー画面に遷移する
					return make_shared<SceneGameOver>();
				}
			}

			// Xボタンを押したら、操作説明画像の表示がON/OFF切り替わる
			if (Pad::IsTrigger(PAD_INPUT_C))
			{
				if (m_isExplanationView) {
					m_isExplanationView = false;
				}
				else {
					m_isExplanationView = true;
				}
			}
		}
		else {	// ゲーム開始前処理
			// Aボタンを押したら、操作説明画像が既定の位置へ移動するフラグを立てる
			if (Pad::IsTrigger(PAD_INPUT_A)) {	
				m_isExplanationSet = true;
			}
			
			// 操作説明画像が既定の位置に移動したら、ゲーム開始前処理を終了する
			if (size.m_height <= 0 && size.m_width >= kExplanationPosX){
				m_isExplanationFinishFlag = true;
			}

			// 操作説明画像を既定の位置へ移動させる
			if (m_isExplanationSet){
				size.m_width = min(size.m_width, kExplanationPosX);
				size.m_widthMax = min(size.m_widthMax, 1920);
				size.m_height = max(size.m_height, 0);
				size.m_heightMax = max(size.m_heightMax, kExplanationSizeY);

				size.m_height -=kExplanationSubHeight;
				size.m_heightMax -= kExplanationSubHeight;
				size.m_width += kExplanationAddWidth;
				size.m_widthMax += kExplanationAddWidth;
			}
		}
	}

#ifdef _DEBUG
	

	if (Pad::IsTrigger(PAD_INPUT_L))		// LBボタンを押したら
	{
		return make_shared<SceneGameClear>();	// ゲームクリアへ行く
	}
	//if (Pad::IsTrigger(PAD_INPUT_R))		// RBボタンを押したら
	//{
	//	return make_shared<SceneGameOver>();	// ゲームオーバーへ行く
	//}

#endif // _DEBUG

	return shared_from_this();
}

void SceneGame::Draw()
{
	m_pMap->Draw();
	m_pEnemyRight->Draw();
	m_pEnemyLeft->Draw();
	m_pPlayer->Draw();

	m_pEnemyLeft->UIDraw();
	m_pEnemyRight->UIDraw();

	if (!m_isExplanationFinishFlag)
	{
		DrawBox(0, 0, 1920, 1080, 0x000000, true);
	}

	// 操作説明画像の描画
	if (m_isExplanationView) {
		DrawExtendGraph(size.m_width, size.m_height,
			size.m_widthMax, size.m_heightMax,
			m_explanationGraph, true);
	}

	if (m_isPause) {
		// ポーズ用の画像を描画する
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);	// 半透明で表示開始
		DrawBoxAA(0, 0, 1920, 1080, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく
		DrawGraph(500, 250, m_pauseGraph, true);

	}
	
	m_pFade->Draw();

#ifdef _DEBUG
	DrawString(0, 120, "SceneGame", 0xffffff);
	DrawString(0, 140, "Please Press Button RB or LB", 0x00ffff);	
#endif
}

void SceneGame::End()
{
	m_pMap->End();
	m_pPlayer->End();
	m_pEnemyRight->End();
	m_pEnemyLeft->End();

	DeleteGraph(m_explanationGraph);
	DeleteGraph(m_pauseGraph);
	SceneBase::End();
}