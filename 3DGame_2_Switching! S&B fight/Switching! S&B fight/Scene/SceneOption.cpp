#include "SceneOption.h"
#include "SceneTitle.h"
#include "../Util/Pad.h"
#include "../Util/Fade.h"
#include <cassert>

namespace
{
	const char* const kUI[6]{
		"data/Image/OptionScene/UI/説明背景.png",		// セレクト背景
		"data/Image/OptionScene/UI/説明文.png",	// 説明文
		"data/Image/OptionScene/Bg.jpg",
		"data/Image/OptionScene/UI/BGM.png",
		"data/Image/OptionScene/UI/SE.png",
		"data/Image/OptionScene/UI/タイトルに戻る.png",
	};

	// 箱の上下移動量
	constexpr int kSelectMoveY = 230;

	constexpr float kScreenWidth = 1920.0f;
	constexpr float kScreenHeight = 1080.0f;

	// 下の箱
	constexpr float UnderBoxX = (kScreenWidth * 0.1f) * 1.4f;
	constexpr float UnderBoxY = (kScreenHeight * 0.5f) * 0.5f;
	// 下の箱の長さ
	constexpr float UnderBoxWidth = (kScreenWidth * 0.8f) * 0.897f;
	constexpr float UnderBoxHeight = (kScreenHeight * 0.8f) * 0.1f;
	// 上の箱
	constexpr float UpBoxX = UnderBoxX;
	constexpr float UpBoxY = UnderBoxY;
	// 上の箱の長さ
	constexpr float UpBoxWidth = UnderBoxWidth;
	constexpr float UpBoxHeight = UnderBoxHeight;
	// タイトルに戻るBox位置
	constexpr float BackBoxX = UnderBoxX * 2.3f;
	constexpr float BackBoxY = UnderBoxY + (kSelectMoveY * 2);

	const VECTOR kSelectPos[6]{
		(VGet(UpBoxX,UnderBoxY,0.0f)),
		(VGet(UpBoxX + UpBoxWidth,UnderBoxHeight + UnderBoxY,0.0f)),
		(VGet(UpBoxX,UnderBoxY+ kSelectMoveY,0.0f)),
		(VGet(UpBoxX + UpBoxWidth,UpBoxY + UpBoxHeight+kSelectMoveY,0.0f)),
		(VGet(BackBoxX,BackBoxY,0.0f)),
		(VGet(BackBoxX + UnderBoxWidth * 0.5f,BackBoxY+ UnderBoxHeight,0.0f))
	};

	// BGM文字位置
	constexpr float BgmStringX = UnderBoxX;
	constexpr float BgmStringY = UnderBoxY - 80;
	// SE文字位置
	constexpr float SeStringX = UnderBoxX;
	constexpr float SeStringY = BgmStringY + kSelectMoveY;
	// タイトルに戻る文字位置
	constexpr float BackStringX = BackBoxX + 110;
	constexpr float BackStringY = BackBoxY + 10;

	constexpr float kCaptionScroll = 1.5f;		// テロップスクロール移動量
	constexpr float kCaptionPosX = -30.0f;		// テロップスクロール移動量

	constexpr int kCaptionBgPosY = 1003;
	constexpr int kCaptionPosY = 1015;
}

SceneOption::SceneOption() :
	m_scrollX(kCaptionPosX),
	m_select(Select::kBgmVolume)
{
}

SceneOption::~SceneOption()
{
}

void SceneOption::Init()
{
	// UI画像の読み込み
	for (int i = 0; i < m_uiGraph.size(); i++) {
		m_uiGraph[i] = LoadGraph(kUI[i]);
		assert(m_uiGraph[i] != -1);
	}

	m_pFade = std::make_shared<Fade>();

	m_isNextSceneFlag = false;

	m_pSound->Init();
	m_pSound->InitSound();	// サウンドの初期化
	m_pSound->LoadBGM(SoundManager::BGM_Type::kOptionBGM);	// サウンドの読み込み
	m_pSound->LoadSE(SoundManager::SE_Type::kSelectSE);
	m_pSound->LoadSE(SoundManager::SE_Type::kButtonSE);

	m_pSound->PlayBGM(SoundManager::BGM_Type::kOptionBGM, DX_PLAYTYPE_LOOP);

	m_select = kBgmVolume;
}

shared_ptr<SceneBase> SceneOption::Update()
{
	m_pFade->FadeIn(m_pFade->GatFadeInFlag());

	// 背景スクロール
	m_scrollX += kCaptionScroll;

	m_pSound->SelectOption();

	m_pFade->FadeOut(m_pFade->GatFadeOutFlag());
	m_isNextSceneFlag = m_pFade->GatNextSceneFlag();

	if (m_isNextSceneFlag)
	{
		return make_shared<SceneTitle>();	// タイトルシーンへ行く
	}

	if (m_pSound->GetSoundSelect() == m_pSound->Select::kBack && Pad::IsTrigger(PAD_INPUT_A))		// Aボタン
	{
		m_pFade->SetFadeOutFlag(true);
		m_pSound->PlaySE(SoundManager::SE_Type::kButtonSE, DX_PLAYTYPE_BACK);
	}
	

	m_pSound->ChangeSound();

	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		m_select += 1;
		if (m_select >= 3)
		{
			m_select = Select::kBgmVolume;
		}
	}
	if (Pad::IsTrigger(PAD_INPUT_UP))
	{
		m_select -= 1;
		if (m_select <= -1)
		{
			m_select = Select::kBack;
		}
	}

	if (m_select == Select::kBgmVolume)
	{
		c1 = { kSelectPos[0],kSelectPos[1] };
	}
	if (m_select == Select::kSeVolume)
	{
		c1 = { kSelectPos[2],kSelectPos[3] };
	}
	if (m_select == Select::kBack)
	{
		c1 = { kSelectPos[4],kSelectPos[5] };
	}

#ifdef _DEBUG
#endif // DEBUG

	return shared_from_this();
}

void SceneOption::Draw()
{
	DrawExtendGraph(0, 0, 1920, 1080, m_uiGraph[2], false);
	DrawBox(250, 150, 1670, 880, 0x000000, true);
	DrawSelect();
	DrawExplanation();

	m_pFade->Draw();
#ifdef _DEBUG
	DrawString(0, 0, "SceneOption", 0xffffff);
	DrawString(0, 20, "Please Press Button START", 0x00ffff);
	DrawFormatString(0, 40, 0xffffff, "select=%d", m_pSound->GetSoundSelect());
#endif // DEBUG
}

void SceneOption::End()
{
	// UI画像の削除
	for (int i = 0; i < m_uiGraph.size(); i++) {
		DeleteGraph(m_uiGraph[i]);
	}

	SceneBase::End();
}

void SceneOption::DrawSelect()
{
	DrawBoxAA(kSelectPos[0].x, kSelectPos[0].y,
		kSelectPos[1].x - UpBoxWidth + m_pSound->GetBgmVolume() * 5.4f, kSelectPos[1].y,
		0x0095d9, true, 2.0f);
	DrawBoxAA(kSelectPos[2].x, kSelectPos[2].y,
		kSelectPos[3].x - UpBoxWidth + m_pSound->GetSeVolume() * 5.4f, kSelectPos[3].y,
		0x0095d9, true, 2.0f);
	DrawBox(kSelectPos[4].x, kSelectPos[4].y,
		kSelectPos[5].x, kSelectPos[5].y,
		0xff00ff, false);

	for (int i = 0; i < 2; i++)
	{
		DrawBoxAA(UnderBoxX, UnderBoxY + (kSelectMoveY * i),
			UnderBoxX + UnderBoxWidth, UnderBoxY + UnderBoxHeight + (kSelectMoveY * i),
			0xFFFFFF, false, 2.0f);
	}
	DrawBoxAA(kSelectPos[4].x, kSelectPos[4].y,
		kSelectPos[5].x, kSelectPos[5].y,
		0xffffff, false, 3.0f);

	DrawBoxAA(c1.m_selectBox1.x - 3, c1.m_selectBox1.y - 3,
		c1.m_selectBox2.x + 3, c1.m_selectBox2.y + 3,
		0xff0000, false, 3.0f);

	DrawGraph(BgmStringX, BgmStringY, m_uiGraph[3],true);
	DrawGraph(SeStringX, SeStringY, m_uiGraph[4], true);
	DrawGraph(BackStringX, BackStringY, m_uiGraph[5], true);
}

void SceneOption::DrawExplanation()
{
	// 背景画像のサイズ取得
	Size expBgSize, expSize;
	GetGraphSize(m_uiGraph[0], &expBgSize.m_width, &expBgSize.m_height);
	GetGraphSize(m_uiGraph[1], &expSize.m_width, &expSize.m_height);

	// スクロール量を計算
	int scrollBg = static_cast<int>(m_scrollX) % static_cast<int>(expBgSize.m_width);

	// 説明文背景描画
	for (int index = 0; index < 4; index++)
	{
		DrawGraph(-scrollBg + index * expBgSize.m_width,
			static_cast<int>(kCaptionBgPosY),
			m_uiGraph[0], true);

		DrawGraph(-scrollBg + index * expSize.m_width,
			static_cast<int>(kCaptionPosY),
			m_uiGraph[1], true);
	}
}
