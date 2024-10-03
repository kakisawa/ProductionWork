#include "ModelBase.h"

namespace {
	const VECTOR InitVec = VGet(0.0f, 0.0f, 0.0f);	// Vector値初期価値
	constexpr float InitFloat = 0.0f;				// float値初期化
}

ModelBase::ModelBase() :
	model(-1),
	m_angle(InitFloat),
	m_pos(InitVec),
	m_move(InitVec),
	m_targetDir(InitVec)
{
}

ModelBase::~ModelBase()
{
	MV1DeleteModel(model);
}

void ModelBase::Init()
{
	MV1SetPosition(model, m_pos);
}

void ModelBase::Update()
{
	MV1SetPosition(model, m_pos);
}

void ModelBase::Draw()
{
	MV1DrawModel(model);
}