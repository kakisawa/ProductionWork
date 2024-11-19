#include "SceneTitle.h"
#include "SceneSelect.h"
#include "SceneDebug.h"

using namespace MyInputInfo;

namespace {
	constexpr int kTitlePosX = 380;
	constexpr int kTitlePosY = 140;

	constexpr int kButtonY = 880;
}

SceneTitle::SceneTitle():
	m_titleGraph(-1),
	m_buttonGraph(-1)
{
}

SceneTitle::~SceneTitle()
{
	DeleteGraph(m_titleGraph);
	DeleteGraph(m_buttonGraph);
}

void SceneTitle::Init()
{
	m_titleGraph = LoadGraph("Data/Image/SceneTitle/タイトルロゴ.png");
	m_buttonGraph= LoadGraph("Data/Image/SceneTitle/PressAnyButton.png");
}

std::shared_ptr<SceneBase> SceneTitle::Update(Input& input)
{
	if (input.IsTrigger(InputInfo::OK)) {			// STARTボタン

		return std::make_shared<SceneSelect>();	// ゲームセレクトシーンへ行く
	}


#ifdef _DEBUG
	if (input.IsTrigger(InputInfo::DebugStart)) {			// STARTボタン

		return std::make_shared<SceneDebug>();	// デバッグシーンへ行く
	}

#else

#endif // DEBUG

	return shared_from_this();
}

void SceneTitle::Draw()
{
	DrawString(0, 0, "SceneTitle", 0xffffff);
	DrawString(0, 20, "Press A", 0xffffff);

	DrawGraph(kTitlePosX, kTitlePosY, m_titleGraph, true);

	static int m_fadeAlpha;
	static bool isFade;

	if (isFade) {
		m_fadeAlpha += 3;

		if (m_fadeAlpha >= 255) {
			isFade = false;
		}
	}
	else {
		m_fadeAlpha -= 5;
		if (m_fadeAlpha <= 0) {
			isFade = true;
		}
	}

	// フェードイン・フェードアウト描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// 半透明で表示開始
	DrawGraph(0, kButtonY, m_buttonGraph, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく	

#ifdef _DEBUG
	
#endif // DEBUG
}

void SceneTitle::End()
{
}
