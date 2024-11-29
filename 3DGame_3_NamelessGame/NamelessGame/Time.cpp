#include "Time.h"
#include "DxLib.h"

namespace {
	constexpr float kMaxtimeLimit = 10800.0f;	// 制限時間(3分=180秒)

	const char* const kNumberUI[11]{
		"Data/Image/SceneGame/時間/0.png",
		"Data/Image/SceneGame/時間/1.png",
		"Data/Image/SceneGame/時間/2.png",
		"Data/Image/SceneGame/時間/3.png",
		"Data/Image/SceneGame/時間/4.png",
		"Data/Image/SceneGame/時間/5.png",
		"Data/Image/SceneGame/時間/6.png",
		"Data/Image/SceneGame/時間/7.png",
		"Data/Image/SceneGame/時間/8.png",
		"Data/Image/SceneGame/時間/9.png",
		"Data/Image/SceneGame/時間/点.png",
	};

	constexpr int kMinutes = 3600;
	constexpr float kSecondTen = 600.0f;
	constexpr float kSecondOne = 60.0f;
}

Time::Time():
	m_second(kMaxtimeLimit),
	m_isTimeUp(false)
{
	// 数字UI画像読み込み
	for (int i = 0; i < m_numberUIHandle.size(); i++)
	{
		m_numberUIHandle[i] = LoadGraph(kNumberUI[i]);
	}
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

	DrawFormatString(0, 1020, 0xffffff,
		"残り時間:分=%d", m_second / kMinutes);

	DrawFormatString(0, 1040, 0xffffff,
		"残り時間:十秒=%d", ((m_second % kMinutes) / 60) / 10);

	DrawFormatString(0, 1060, 0xffffff,
		"残り時間:秒=%d", ((m_second % kMinutes) / 60) % 10);
}

void Time::End()
{
	// 数字UI画像読み込み
	for (int i = 0; i < m_numberUIHandle.size(); i++)
	{
		DeleteGraph(m_numberUIHandle[i]);
	}
}

void Time::Calculation()
{
	


}
