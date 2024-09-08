#include "SceneGameClear.h"
#include "SceneTitle.h"
#include "../Util/Pad.h"
#include "../Util/Fade.h"

namespace {
	constexpr int kTitlePosX = 500;
	constexpr int kTitlePosY = 150;
}

SceneGameClear::SceneGameClear():
	m_graph(-1)
{
}

SceneGameClear::~SceneGameClear()
{
}

void SceneGameClear::Init()
{
	m_pSound->InitSound();	// サウンドの初期化
	m_graph = LoadGraph("data/GameClear!!.png");
}

shared_ptr<SceneBase> SceneGameClear::Update()
{
	if (Pad::IsTrigger(PAD_INPUT_R))		// RBボタン
	{
		return make_shared<SceneTitle>();	// タイトルシーンへ行く
	}


#ifdef _DEBUG
#endif // DEBUG

	return shared_from_this();
}

void SceneGameClear::Draw()
{
	DrawString(0, 0, "SceneGameClear", 0xffffff);
	DrawString(0, 20, "Please Press Button START", 0x00ffff);

	/*DrawExtendGraph(kTitlePosX, kTitlePosY,
		kTitlePosX + 1200, kTitlePosY + 430, m_graph, true);*/

	DrawGraph(kTitlePosX, kTitlePosY, m_graph, true);
}

void SceneGameClear::End()
{
	DeleteGraph(m_graph);
	SceneBase::End();
}
