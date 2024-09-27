#include "DxLib.h"
#include "Vec2.h"
#include "Game.h"
#include "Input.h"
#include "Sound.h"
#include "UIBattle.h"
#include "Player.h"
#include "EnemyNinja.h"
#include "EnemyChef.h"
#include "EnemyAbe.h"
#include "Camera.h"
#include "Stage.h"
#include "ScenePause.h"
#include "SceneClear.h"
#include "SceneGameover.h"
#include "SceneStage2.h"

namespace
{
	constexpr int kMaxBattleNum = 3;		// 最大バトル数
	constexpr int kFightTextDispStart = 80;	// "Fight"のテキストを表示し始める時間
	constexpr int kFadeFrame = 4;			// フェード変化量
	constexpr int kGameoverFadeFrame = 2;	// ゲームオーバー時のフェード変化量
}


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="player">プレイヤー参照</param>
/// <param name="camera">カメラ参照</param>
/// <param name="pStage">ステージ参照</param>
SceneStage2::SceneStage2(std::shared_ptr<Player> pPlayer, std::shared_ptr<Camera> pCamera, std::shared_ptr<Stage> pStage)
{
	m_pPlayer = pPlayer;
	m_pCamera = pCamera;
	m_pStage = pStage;
	m_pEnemy = std::make_shared<EnemyNinja>();
	m_pUIBattle = std::make_shared<UIBattle>(0.0f, m_pEnemy->GetEnemyType());
	m_battleNum = 0;
}


/// <summary>
/// デストラクタ
/// </summary>
SceneStage2::~SceneStage2()
{
	StopSoundMem(Sound::m_bgmHandle[static_cast<int>(Sound::BgmKind::kStage2)]);
}


/// <summary>
/// 初期化
/// </summary>
void SceneStage2::Init()
{
	SceneStageBase::Init();
}


/// <summary>
/// 更新
/// </summary>
/// <param name="input">入力状態</param>
/// <returns>遷移先のシーン</returns>
std::shared_ptr<SceneBase> SceneStage2::Update(Input& input)
{
#ifdef _DEBUG	// デバッグモード
	// Pキーでポーズ、ポーズ中にPでコマ送り
	if (m_debugState == DebugState::Normal && input.IsTriggered("debug_pause"))
	{
		m_debugState = DebugState::Pause;
	}
	if (m_debugState == DebugState::Pause && input.IsTriggered("debug_enter"))
	{
		m_debugState = DebugState::Normal;
	}
	if (m_debugState != DebugState::Pause || input.IsTriggered("debug_pause"))
#endif
	{
		FadeOut(kFadeFrame); // フェードアウト
		UpdateSound();		 // サウンド更新

		// ポーズ画面を開く
		if (input.IsTriggered("pause"))
		{
			m_isPause = true;
			return std::make_shared<ScenePause>(shared_from_this());
		}

		m_pCamera->StartProduction();
		m_pCamera->Update(input, *m_pPlayer);
		m_pPlayer->Update(input, *m_pCamera, *m_pEnemy, *m_pStage);
		m_pEnemy->Update(*m_pPlayer, *m_pStage, *this);

		// スタート演出の更新
		m_nextBattleTime--;
		if (m_nextBattleTime > 0)
		{
			return shared_from_this();
		}
		else
		{
			m_pPlayer->SetIsStartProduction(false);
		}

		// 敵のHPが0になった場合
		if (m_pEnemy->GetHp() <= 0)
		{
			// クリア演出を行う
			ClearProduction();

			// クリア演出が終わったら次のバトルに移行する
			if (m_clearProductionTime <= 0)
			{
				m_clearTime.push_back(m_elapsedTime);
				m_elapsedTime = 0; // 経過時間をリセット

				// 次の敵を登場させる
				switch (m_battleNum)
				{
				case 0:	// 2戦目
					m_pEnemy = nullptr;
					m_pEnemy = std::make_shared<EnemyChef>();
					UpdateNextBattle();
					break;
				case 1:	// 3戦目
					m_pEnemy = nullptr;
					m_pEnemy = std::make_shared<EnemyAbe>();
					UpdateNextBattle();
					break;
				case 2: // 勝利時
					UpdateNextBattle();
					return std::make_shared<SceneClear>(StageKind::kStage2, m_clearTime);
					break;
				}

				m_battleNum++; // 現在のバトル数を増やす
			}

		}
		// プレイヤーのHPが0になった場合
		else if (m_pPlayer->GetHp() <= 0)
		{
			if (m_gameoverProductionTime > 0)
			{
				GameoverProduction();
			}
			else
			{
				FadeIn(kGameoverFadeFrame); // フェードイン
				StopSoundMem(Sound::m_bgmHandle[static_cast<int>(Sound::BgmKind::kStage2)]);
				return std::make_shared<SceneGameover>(shared_from_this());
			}
		}
		else
		{
			m_elapsedTime++; // 経過時間を進める
		}
	}

#ifdef _DEBUG
	// 敵のHPを0にする
	m_pEnemy->DebugDamage(input);
	// シーン遷移
	if (input.IsTriggered("debug_clear"))
	{
		return std::make_shared<SceneClear>(StageKind::kStage2, m_clearTime);
	}
	else if (m_pPlayer->GetHp() <= 0.0f || input.IsTriggered("debug_gameover"))
	{
		if (m_gameoverProductionTime > 0)
		{
			GameoverProduction();
		}
		else
		{
			FadeIn(kGameoverFadeFrame); // フェードイン
			StopSoundMem(Sound::m_bgmHandle[static_cast<int>(Sound::BgmKind::kStage2)]);
			return std::make_shared<SceneGameover>(shared_from_this());
		}
	}
#endif

	return shared_from_this();	// 自身のshared_ptrを返す
}


/// <summary>
/// 描画
/// </summary>
void SceneStage2::Draw()
{
	SceneStageBase::Draw();
	// スタート演出UIを表示
	m_pUIBattle->DrawStartProduction(m_nextBattleTime, m_battleNum, kMaxBattleNum);

#ifdef _DEBUG	// デバッグ表示
	// 現在のシーン
	DrawString(0, 0, "ステージ2", 0xffffff);
#endif
}


/// <summary>
/// サウンド更新
/// </summary>
void SceneStage2::UpdateSound()
{
	if (m_nextBattleTime < kFightTextDispStart && m_nextBattleTime > 0)
	{
		// 開始時に1度だけSEを流す
		if (!CheckSoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kBattleStart)]))
		{
			PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kBattleStart)], DX_PLAYTYPE_BACK);
		}
	}
	else
	{
		// BGMを鳴らす
		if (!CheckSoundMem(Sound::m_bgmHandle[static_cast<int>(Sound::BgmKind::kStage2)]))
		{
			PlaySoundMem(Sound::m_bgmHandle[static_cast<int>(Sound::BgmKind::kStage2)], DX_PLAYTYPE_LOOP);
		}
	}
}
