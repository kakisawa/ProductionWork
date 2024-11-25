#include "Effect.h"
#include "EffekseerForDXLib.h"
#include "Input.h"

namespace {
	constexpr int kEffectNum = 5;
}

Effect::Effect()
{
	m_effect[EffectKind::kKnife].m_handle= LoadEffekseerEffect("data/Effect/attack.efk");
}

Effect::~Effect()
{
}

void Effect::Init()
{
}

void Effect::Update()
{
}

void Effect::Draw()
{
}

void Effect::ClearEffect()
{
}

void Effect::AddEffect(EffectKind kind, VECTOR pos)
{

}
