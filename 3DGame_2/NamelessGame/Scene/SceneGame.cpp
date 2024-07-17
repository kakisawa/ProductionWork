#include "SceneGame.h"
#include "SceneGameClear.h"
#include "SceneGameOver.h"
#include "../Player.h"
#include "../Camera.h"
#include "../Pad.h"
#include <cassert>

namespace {
	const char* const kModel = "data/model/artifact.mv1";
	const char* const kArrowModel = "data/model/Arrow.mv1";

	constexpr float kArrowSize = 30.0f;
}

SceneGame::SceneGame()
{
	// 仮
	m_model = MV1LoadModel(kModel);
	MV1SetScale(m_model, VGet(5.0f, 5.0f, 5.0f));

	m_arrowModel[0] = MV1LoadModel(kArrowModel);
	m_arrowModel[1] = MV1DuplicateModel(m_arrowModel[0]);
	m_arrowModel[2] = MV1DuplicateModel(m_arrowModel[0]);
	m_arrowModel[3] = MV1DuplicateModel(m_arrowModel[0]);

	for (int i = 0; i < 4; i++)
	{
		MV1SetScale(m_arrowModel[i], VGet(kArrowSize, kArrowSize, kArrowSize));
	}
	
	MV1SetPosition(m_arrowModel[0], VGet(-30.0f, 0.0f, 0.0f));
	MV1SetPosition(m_arrowModel[1], VGet(30.0f, 0.0f, 0.0f));
	MV1SetPosition(m_arrowModel[2], VGet(0.0f, 0.0f, 14.0f));
	MV1SetPosition(m_arrowModel[3], VGet(0.0f, 0.0f, -14.0f));

	MV1SetRotationXYZ(m_arrowModel[0], VGet(0.0f, 180.0f * DX_PI_F/180.0f, 0.0f));
	MV1SetRotationXYZ(m_arrowModel[2], VGet(0.0f, -90.0f * DX_PI_F / 180.0f, 90.0f * DX_PI_F / 180.0f));
	MV1SetRotationXYZ(m_arrowModel[3], VGet(0.0f, 90.0f * DX_PI_F / 180.0f, 90.0f * DX_PI_F / 180.0f));


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

	m_pPlayer->Update(*m_pCamera);
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
	VECTOR pos = VGet(-10.0f, 0.0f, 0.0f);
	MV1SetPosition(m_model, pos);

	return shared_from_this();
}

void SceneGame::Draw()
{
	DrawString(0, 0, "SceneGame", 0xffffff);

	m_pPlayer->Draw();

	// 仮
	MV1DrawModel(m_model);

	for (int i = 0; i < 4; i++)
	{
		MV1DrawModel(m_arrowModel[i]);
	}
	
}

void SceneGame::End()
{
	m_pPlayer->End();

	MV1DeleteModel(m_model);
	for (int i = 0; i < 4; i++)
	{
		MV1DeleteModel(m_arrowModel[i]);
	}
}
