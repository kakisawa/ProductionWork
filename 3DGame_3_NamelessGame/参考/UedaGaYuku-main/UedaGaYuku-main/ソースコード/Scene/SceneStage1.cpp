#include "DxLib.h"
#include "Vec2.h"
#include "Input.h"
#include "Sound.h"
#include "Font.h"
#include "UIBattle.h"
#include "Player.h"
#include "EnemyTuto.h"
#include "Camera.h"
#include "Stage.h"
#include "ScenePause.h"
#include "SceneClear.h"
#include "SceneGameover.h"
#include "SceneStage1.h"

namespace
{
	constexpr int kMaxBattleNum = 1;		// 最大バトル数
	constexpr int kStartSeTime = 60;		// スタートSEを再生表示し始める時間
	constexpr int kFadeFrame = 4;			// フェード変化量
	constexpr int kGameoverFadeFrame = 1;	// ゲームオーバー時のフェード変化量

	/*チュートリアル*/
	// 背景画像のパス
	const Vec2 kTutoTextBoxPos = { 425.0f , 650.0f }; // テキストボックス位置
	const Vec2 kTutoTextPos = { 480.0f , 690.0f };	  // テキスト位置
	constexpr int kTextColor = 0xffffff;			  // テキストの色
}

/// <summary>
/// コンストラクタ
/// </summary>
SceneStage1::SceneStage1(std::shared_ptr<Player> pPlayer, std::shared_ptr<Camera> pCamera, std::shared_ptr<Stage> pStage):
	m_tutoNum(0),
	m_isTuto(false)
{
	m_pPlayer = pPlayer;
	m_pCamera = pCamera;
	m_pStage = pStage;
	m_pEnemy = std::make_shared<EnemyTuto>();
	m_pUIBattle = std::make_shared<UIBattle>(0.0f, m_pEnemy->GetEnemyType());
	m_battleNum = 0;

	m_tutoHandle.resize(TutoHandle::kTutoHandleNum);
	for (int i = 0; i < m_tutoHandle.size(); i++)
	{
		if (i == TutoHandle::kTuto1 || i == TutoHandle::kTuto2)
		{
			m_tutoHandle[i] = LoadGraph("data/UI/Tutorial/pGauge.png");
		}
		else if (i == TutoHandle::kTuto3)
		{
			m_tutoHandle[i] = LoadGraph("data/UI/Tutorial/eGauge.png");
		}
		else if (i == TutoHandle::kTuto4)
		{
			m_tutoHandle[i] = LoadGraph("data/UI/Tutorial/ope.png");
		}
		else if (i == TutoHandle::kTextBox)
		{
			m_tutoHandle[i] = LoadGraph("data/UI/Tutorial/textBox.png");
		}
		else
		{
			m_tutoHandle[i] = LoadGraph("data/UI/Tutorial/def.png");
		}
	}
}


/// <summary>
/// デストラクタ
/// </summary>
SceneStage1::~SceneStage1()
{
	StopSoundMem(Sound::m_bgmHandle[static_cast<int>(Sound::BgmKind::kStage1)]);
	for (auto& handle : m_tutoHandle)
	{
		DeleteGraph(handle);
	}
}


/// <summary>
/// 初期化
/// </summary>
void SceneStage1::Init()
{
	SceneStageBase::Init();
}


/// <summary>
/// 更新
/// </summary>
/// <param name="input">入力状態</param>
/// <returns>遷移するシーン</returns>
std::shared_ptr<SceneBase> SceneStage1::Update(Input& input)
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
		FadeOut(kFadeFrame);	// フェードアウト
		UpdateSound();			// サウンド更新

		// ポーズ画面を開く
		if (input.IsTriggered("pause"))
		{
			m_isPause = true;
			return std::make_shared<ScenePause>(shared_from_this());
		}

		// チュートリアル表示中は動けないようにする
		if (!m_isTuto)
		{
			m_pCamera->StartProduction();
			m_pCamera->Update(input, *m_pPlayer);
			m_pPlayer->Update(input, *m_pCamera, *m_pEnemy, *m_pStage);
			m_pEnemy->Update(*m_pPlayer, *m_pStage, *this);
		}

		UpdateTuto(input);	// チュートリアルの表示状態を更新する
		if (m_isTuto) return shared_from_this();

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
				UpdateNextBattle();
				FadeIn(kFadeFrame); // フェードイン
				m_clearTime.push_back(m_elapsedTime);
				return std::make_shared<SceneClear>(StageKind::kStage1, m_clearTime);
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
				StopSoundMem(Sound::m_bgmHandle[static_cast<int>(Sound::BgmKind::kStage1)]);
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
		return std::make_shared<SceneClear>(StageKind::kStage1, m_clearTime);
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
			StopSoundMem(Sound::m_bgmHandle[static_cast<int>(Sound::BgmKind::kStage1)]);
			return std::make_shared<SceneGameover>(shared_from_this());
		}
	}
#endif

	return shared_from_this();	// 自身のshared_ptrを返す
}


/// <summary>
/// 描画
/// </summary>
void SceneStage1::Draw()
{
	SceneStageBase::Draw();

	if (m_tutoNum < TutoHandle::kTutoNum)
	{
		DrawTutorial(); // チュートリアル表示
	}
	else
	{
		m_pUIBattle->DrawStartProduction(m_nextBattleTime, m_battleNum, kMaxBattleNum); // 演出UIを表示
	}

#ifdef _DEBUG	// デバッグ表示
	// 現在のシーン
	DrawString(0, 0, "ステージ1", 0xffffff);
#endif
}


/// <summary>
/// サウンド更新
/// </summary>
void SceneStage1::UpdateSound()
{
	if (m_nextBattleTime < kStartSeTime && m_nextBattleTime > 0)
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
		if (!CheckSoundMem(Sound::m_bgmHandle[static_cast<int>(Sound::BgmKind::kStage1)]))
		{
			PlaySoundMem(Sound::m_bgmHandle[static_cast<int>(Sound::BgmKind::kStage1)], DX_PLAYTYPE_LOOP);
		}
	}
}


/// <summary>
/// チュートリアルの表示状態を更新する
/// </summary>
/// <param name="input">入力状態</param>
void SceneStage1::UpdateTuto(Input& input)
{
	if (m_tutoNum < TutoHandle::kTutoNum)
	{
		m_isTuto = true;
	}

	if (m_isTuto)
	{
		if (m_tutoNum >= TutoHandle::kTutoNum)
		{
			m_isTuto = false;
		}

		if (input.IsTriggered("A"))
		{
			m_tutoNum++;	// チュートリアルの表示を進める
		}
	}
}


/// <summary>
/// チュートリアル表示
/// </summary>
void SceneStage1::DrawTutorial()
{
	// 背景表示
	DrawGraph(0, 0, m_tutoHandle[m_tutoNum], true);

	// テキストボックス表示
	DrawGraphF(kTutoTextBoxPos.x, kTutoTextBoxPos.y, m_tutoHandle[TutoHandle::kTextBox], true);

	// ボタン表示
	m_pUIBattle->DrawTutoButtonText();

	// テキスト表示
	if (m_tutoNum == TutoHandle::kTuto0)		m_tutoText = "ここでは基本的なルールなどについて説明するぞ！";
	else if (m_tutoNum == TutoHandle::kTuto1)	m_tutoText = "左上の赤いゲージがプレイヤーのHPだ。\nその下の青いバーが、プレイヤーのゲージだ。";
	else if (m_tutoNum == TutoHandle::kTuto2)	m_tutoText = "ゲージは攻撃を行うことで溜まっていくぞ。\nゲージが溜まると必殺技が発動できるぞ！";
	else if (m_tutoNum == TutoHandle::kTuto3)	m_tutoText = "右下の赤いゲージが敵のHPだ。\n敵のHPを0にしたら勝利だ！";
	else if (m_tutoNum == TutoHandle::kTuto4)	m_tutoText = "操作方法については右側に書いてあるから確認してくれ！";
	else if (m_tutoNum == TutoHandle::kTuto5)	m_tutoText = "説明については以上だ！\n人形を用意したから実際に闘ってみよう！";

	DrawStringFToHandle(kTutoTextPos.x, kTutoTextPos.y, m_tutoText.c_str(), kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kTutorial)]);
}