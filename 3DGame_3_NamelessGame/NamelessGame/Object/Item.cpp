#include "Item.h"
#include <DxLib.h>
#include <cassert>

namespace {
	const VECTOR kInitPos = VGet(100.0f, 15.0f, 100.0f);
	const VECTOR kModelSize = VGet(50.0f, 50.0f, 50.0f);

	VECTOR move = VGet(0.0f, 0.0f, 0.0f);
	float m_angle=0.0f;
}

Item::Item():
	m_model(-1),
	m_pos(kInitPos)
{
	
}

Item::~Item()
{
	
}

void Item::Init()
{
	m_model = MV1LoadModel("Data/Model/ItemBox.mv1");
	assert(m_model != -1);

	MV1SetPosition(m_model, kInitPos);
	MV1SetScale(m_model, kModelSize);
}

void Item::Update()
{
	Floating();
}

void Item::Draw()
{
	MV1DrawModel(m_model);
	DrawFormatString(0, 500, 0xffffff, "m_pos=%.2f", m_pos.y);
	DrawFormatString(0, 520, 0xffffff, "m_angle=%.2f", m_angle);
}

void Item::Floating()
{
	m_angle += 2.0f;
	float angle = m_angle * (DX_PI_F / 180.0f);
	move.y = static_cast<float>(sin(angle)) * 0.1f;

	m_pos = VAdd(m_pos, move);

	MV1SetPosition(m_model, m_pos);
}
