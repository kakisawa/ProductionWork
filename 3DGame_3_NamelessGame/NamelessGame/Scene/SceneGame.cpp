#include "SceneGame.h"
#include "DxLib.h"

SceneGame::SceneGame():
	model(-1)
{
	model = MV1LoadModel("Data/Model/PlayerModel.mv1");
}

SceneGame::~SceneGame()
{
	MV1DeleteModel(model);
}

void SceneGame::Init()
{
	pos = VGet(300.0f, 250.0f, -100.0f);
	MV1SetPosition(model, pos);
	MV1SetScale(model, VGet(0.5f, 0.5f, 0.5f));
}

void SceneGame::Update()
{
	
}

void SceneGame::Draw()
{
	MV1DrawModel(model);
}

void SceneGame::End()
{
}
