#include "Map.h"

namespace {
	const VECTOR kPointPos1 = VGet(-100.0f, 0.0f, -100.0f);
	const VECTOR kPointPos2 = VGet(-100.0f, 0.0f, 150.0f);
	const VECTOR kPointPos3 = VGet(100.0f, 0.0f, 150.0f);
	const VECTOR kPointPos4 = VGet(100.0f, 0.0f, -100.0f);
}


Map::Map()
{
}

Map::~Map()
{
}

void Map::Init()
{
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
	DrawSphere3D(m_pointPos.point1, 10.0f, 32, 0x0000ff, 0x0000ff, true);
	DrawSphere3D(m_pointPos.point2, 10.0f, 32, 0x0000ff, 0x0000ff, true);
	DrawSphere3D(m_pointPos.point3, 10.0f, 32, 0x0000ff, 0x0000ff, true);
	DrawSphere3D(m_pointPos.point4, 10.0f, 32, 0x0000ff, 0x0000ff, true);
}
