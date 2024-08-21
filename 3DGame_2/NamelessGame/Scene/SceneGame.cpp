#include "SceneGame.h"
#include "SceneGameClear.h"
#include "SceneGameOver.h"
#include "../GameMap.h"
#include "../Enemy/EnemyRight.h"
#include "../Enemy/EnemyLeft.h"
#include "../Player.h"
#include "../Camera.h"
#include "../Pad.h"
#include <cassert>

namespace {
	const char* const kCupModel = "data/model/artifact.mv1";
	const char* const kArrowModel = "data/model/Arrow.mv1";

	constexpr float kArrowSize = 30.0f;
}

SceneGame::SceneGame():
	m_pos(VGet(0.0f,0.0f,0.0f)),
	m_modelCup(-1)
{
	// 仮
	m_modelCup = MV1LoadModel(kCupModel);
	MV1SetScale(m_modelCup, VGet(5.0f, 5.0f, 5.0f));

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
	m_pMap->Init();
	m_pPlayer->Init(m_pMap);
	m_pEnemyRight->Init();
	m_pEnemyLeft->Init();
	m_pCamera->Init();
}

shared_ptr<SceneBase> SceneGame::Update()
{
	m_pPlayer->Update(*m_pCamera);
	m_pEnemyRight->Update();
	m_pEnemyLeft->Update();
	m_pCamera->Update(*m_pPlayer);


#ifdef _DEBUG
	if (Pad::IsTrigger(PAD_INPUT_Y))		// LBボタンを押したら
	{
		return make_shared<SceneGameClear>();	// ゲームクリアへ行く
	}
	if (Pad::IsTrigger(PAD_INPUT_Z))		// RBボタンを押したら
	{
		return make_shared<SceneGameOver>();	// ゲームオーバーへ行く
	}

	
#endif // _DEBUG

	// 仮
	VECTOR pos = VGet(-10.0f, 0.0f, 0.0f);
	MV1SetPosition(m_modelCup, pos);

	return shared_from_this();
}

void SceneGame::Draw()
{
	DrawString(0, 0, "SceneGame", 0xffffff);
	DrawString(0, 80, "Please Press Button RB or LB", 0x00ffff);

	m_pMap->Draw();
	m_pEnemyRight->Draw();
	m_pEnemyLeft->Draw();
	m_pPlayer->Draw();

	// 仮
	MV1DrawModel(m_modelCup);

	for (int i = 0; i < 4; i++)
	{
		MV1DrawModel(m_arrowModel[i]);
	}
	

	// 授業用
	/*
	int y = 50;
	for (const auto& loc : m_localData) {
		DrawFormatString(50, y, 0xffffff, "name=%s : tag=%s : pos=%2.2f,%2.2f,%2.2f : rot=%2.2f,%2.2f,%2.2f : scale=%2.2f,%2.2f,%2.2f",
			loc.name.c_str(),
			loc.tag.c_str(),
			loc.pos.x, loc.pos.y, loc.pos.z,
			loc.rot.x, loc.rot.y, loc.rot.z,
			loc.scale.x, loc.scale.y, loc.scale.z);

		if (loc.tag == "Sphere")
		{
			DrawSphere3D(VGet(loc.pos.x, loc.pos.y, loc.pos.z), 1.0f, 35,
				GetColor(255, 255, 255), GetColor(255, 255, 255), true);
		}

		y += 20;
	}*/
	
}

void SceneGame::End()
{
	m_pMap->End();
	m_pPlayer->End();
	m_pEnemyRight->End();
	m_pEnemyLeft->End();

	MV1DeleteModel(m_modelCup);
	for (int i = 0; i < 4; i++)
	{
		MV1DeleteModel(m_arrowModel[i]);
	}
}

// 授業用
/*
namespace {
	float DegreeToRadian(float deg) {
		return deg + (DX_PI_F);
	}
}
void SceneGame::LoadLocations()
{
	int handle= FileRead_open("data/data.loc");
	// データ数
	int dataCnt = 0;
	FileRead_read(&dataCnt, sizeof(dataCnt), handle);
	m_locationData.resize(dataCnt);
	for (auto& loc : m_locationData)
	{
		// オブジェクト名ロード
		byte nameCnt = 0;
		FileRead_read(&nameCnt, sizeof(nameCnt), handle);
		loc.name.resize(nameCnt);
		FileRead_read(loc.name.data(), sizeof(char) * loc.name.size(), handle);

		// タグ名
		byte tagNameCnt= 0;
		FileRead_read(&tagNameCnt, sizeof(tagNameCnt), handle);
		loc.name.resize(tagNameCnt);
		FileRead_read(loc.name.data(), sizeof(char) * loc.name.size(), handle);



		// 座標情報
		FileRead_read(&loc.pos, sizeof(loc.pos), handle);

		// 回転情報
		FileRead_read(&loc.rot, sizeof(loc.rot), handle);

		// スケーリング情報
		FileRead_read(&loc.scale, sizeof(loc.scale), handle);
	}

	FileRead_close(dataCnt);

	for (auto& loc : m_locationData)
	{
		loc.rot.x = DegreeToRadian(loc.rot.x);
		loc.rot.y = DegreeToRadian(loc.rot.y);
		loc.rot.z = DegreeToRadian(loc.rot.z);
	}

}
*/