#include "SceneGame.h"
#include "SceneGameClear.h"
#include "SceneGameOver.h"
#include "../Player.h"
#include "../Camera.h"
#include "../Pad.h"
#include <cassert>

namespace {
	const char* const kModel = "data/model/artifact.mv1";
	int model;
}

SceneGame::SceneGame()
{
	// 仮
	model = MV1LoadModel(kModel);
	MV1SetScale(model, VGet(5.0f, 5.0f, 5.0f));
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	m_pPlayer->Init();
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

	// 仮
	VECTOR pos = VGet(0.0f, 0.0f, 0.0f);
	MV1SetPosition(model, pos);

	return shared_from_this();
}

void SceneGame::Draw()
{
	DrawString(0, 0, "SceneGame", 0xffffff);

	m_pPlayer->Draw();

	// 仮
	MV1DrawModel(model);
}

void SceneGame::End()
{
	m_pPlayer->End();

	MV1DeleteModel(model);
}
