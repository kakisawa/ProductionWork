﻿#include "Map.h"
#include <cassert>

namespace {
	const VECTOR kPointPos1 = VGet(-100.0f, 0.0f, -100.0f);	// 左前
	const VECTOR kPointPos2 = VGet(-100.0f, 0.0f, 100.0f);	// 左後
	const VECTOR kPointPos3 = VGet(100.0f, 0.0f, 100.0f);	// 右後
	const VECTOR kPointPos4 = VGet(100.0f, 0.0f, -100.0f);	// 右前
}


Map::Map():
	m_model1(-1),
	m_model2(-1),
	m_model3(-1)
{
}

Map::~Map()
{
	MV1DeleteModel(m_model1);
}

void Map::Init()
{
	m_model1 = MV1LoadModel("Data/Model/Map/Map1-1.mv1");
	assert(m_model1 != -1);
	MV1SetPosition(m_model1,VGet(0.0f, -20.0f, 0.0f));
	MV1SetScale(m_model1, VGet(0.8f, 0.8f, 0.8f));

	m_pointPos.point1 = kPointPos1;
	m_pointPos.point2 = kPointPos2;
	m_pointPos.point3 = kPointPos3;
	m_pointPos.point4 = kPointPos4;
}

void Map::Update()
{
}

void Map::Draw()
{
	MV1DrawModel(m_model1);

	DrawSphere3D(m_pointPos.point1, 10.0f, 32, 0x0000ff, 0x0000ff, true);
	DrawSphere3D(m_pointPos.point2, 10.0f, 32, 0x0000ff, 0x0000ff, true);
	DrawSphere3D(m_pointPos.point3, 10.0f, 32, 0x0000ff, 0x0000ff, true);
	DrawSphere3D(m_pointPos.point4, 10.0f, 32, 0x0000ff, 0x0000ff, true);
}
