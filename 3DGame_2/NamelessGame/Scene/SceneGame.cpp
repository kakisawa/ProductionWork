#include "SceneGame.h"
#include "SceneGameClear.h"
#include "SceneGameOver.h"
#include "../Player.h"
#include "../Camera.h"
#include "../Pad.h"
#include <cassert>

SceneGame::SceneGame()
{

}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	m_pCamera->Init();
}

shared_ptr<SceneBase> SceneGame::Update()
{
	Pad::Update();

	m_pPlayer->Update();

	m_pCamera->Update(*m_pPlayer);

#ifdef _DEBUG
	if (Pad::IsTrigger(PAD_INPUT_2))		// Xキー(PAD::B)を押したら
	{
		return make_shared<SceneGameClear>();	// ゲームクリアへ行く
	}
	if (Pad::IsTrigger(PAD_INPUT_3))		// Cキー(PAD::X)を押したら
	{
		return make_shared<SceneGameOver>();	// ゲームオーバーへ行く
	}
#endif // _DEBUG

	return shared_from_this();
}

void SceneGame::Draw()
{
	DrawString(0, 0, "SceneGame", 0xffffff);

	m_pPlayer->Draw();
}

void SceneGame::End()
{
	m_pPlayer->End();
}
