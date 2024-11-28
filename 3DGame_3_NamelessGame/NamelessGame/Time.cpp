#include "Time.h"
#include "DxLib.h"

namespace {
	constexpr float kMaxtimeLimit = 10800.0f;	// 制限時間(3分=180秒)
}

Time::Time():
	m_second(3600),
	m_isTimeUp(false)
{
}

Time::~Time()
{
}

void Time::Init()
{
}

void Time::Update()
{
	m_second--;

	// 制限時間が0になったら
	if (m_second <= 0.0f)
	{
		m_isTimeUp = true;
	}
}

void Time::Draw()
{
	DrawFormatString(0, 1000, 0xffffff, "m_second=%.2f", m_second/60);
}
