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
	constexpr int kSecondOne = 60;
}

Time::Time():
	m_timeHandleMinites(-1),
	m_timeHandleSecondsTen(-1),
	m_timeHandleSecondsOne(-1),
	m_second(kMaxtimeLimit),
	m_isTimeUp(false)
{
	// 数字UI画像読み込み
	for (int i = 0; i < m_numberUIHandle.size(); i++)
	{
		m_numberUIHandle[i] = LoadGraph(kNumberUI[i]);
	}
	m_timeBgHandle = LoadGraph("Data/Image/SceneGame/時間/Time.png");
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

	// 表示用時間計算
	m_minutes = m_second / kMinutes;
	m_secondsTen = ((m_second % kMinutes) / kSecondOne) / 10;
	m_secondsOne= ((m_second % kMinutes) / kSecondOne) % 10;

	SetTimeHandle();


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
		"残り時間:分=%d", m_minutes);

	DrawFormatString(0, 1040, 0xffffff,
		"残り時間:十秒=%d", m_secondsTen);

	DrawFormatString(0, 1060, 0xffffff,
		"残り時間:秒=%d", m_secondsOne);

	DrawGraph(10, 20, m_timeBgHandle, true);

	DrawGraph(35, 45, m_timeHandleMinites, true);
	DrawGraph(90, 50, m_numberUIHandle[10], true);
	DrawGraph(110, 45, m_timeHandleSecondsTen, true);
	DrawGraph(155, 45, m_timeHandleSecondsOne, true);
}

void Time::End()
{
	// 数字UI画像読み込み
	for (int i = 0; i < m_numberUIHandle.size(); i++)
	{
		DeleteGraph(m_numberUIHandle[i]);
	}

	DeleteGraph(m_timeBgHandle);
}

void Time::Calculation()
{
	


}

void Time::SetTimeHandle()
{
	if (m_minutes == 1)
	{
		m_timeHandleMinites = m_numberUIHandle[1];
	}
	else if (m_minutes == 2)
	{
		m_timeHandleMinites = m_numberUIHandle[2];
	}
	else if (m_minutes == 3)
	{
		m_timeHandleMinites = m_numberUIHandle[3];
	}


	if (m_secondsTen == 0)
	{
		m_timeHandleSecondsTen = m_numberUIHandle[0];
	}
	else if (m_secondsTen == 1)
	{
		m_timeHandleSecondsTen = m_numberUIHandle[1];
	}
	else if (m_secondsTen == 2)
	{
		m_timeHandleSecondsTen = m_numberUIHandle[2];
	}
	else if (m_secondsTen == 3)
	{
		m_timeHandleSecondsTen = m_numberUIHandle[3];
	}
	else if (m_secondsTen == 4)
	{
		m_timeHandleSecondsTen = m_numberUIHandle[4];
	}
	else if (m_secondsTen == 5)
	{
		m_timeHandleSecondsTen = m_numberUIHandle[5];
	}
	else if (m_secondsTen == 6)
	{
		m_timeHandleSecondsTen = m_numberUIHandle[6];
	}
	else if (m_secondsTen == 7)
	{
		m_timeHandleSecondsTen = m_numberUIHandle[7];
	}
	else if (m_secondsTen == 8)
	{
		m_timeHandleSecondsTen = m_numberUIHandle[8];
	}
	else if (m_secondsTen == 9)
	{
		m_timeHandleSecondsTen = m_numberUIHandle[9];
	}


	if (m_secondsOne == 0)
	{
		m_timeHandleSecondsOne = m_numberUIHandle[0];
	}
	else if (m_secondsOne == 1)
	{
		m_timeHandleSecondsOne = m_numberUIHandle[1];
	}
	else if (m_secondsOne == 2)
	{
		m_timeHandleSecondsOne = m_numberUIHandle[2];
	}
	else if (m_secondsOne == 3)
	{
		m_timeHandleSecondsOne = m_numberUIHandle[3];
	}
	else if (m_secondsOne == 4)
	{
		m_timeHandleSecondsOne = m_numberUIHandle[4];
	}
	else if (m_secondsOne == 5)
	{
		m_timeHandleSecondsOne = m_numberUIHandle[5];
	}
	else if (m_secondsOne == 6)
	{
		m_timeHandleSecondsOne = m_numberUIHandle[6];
	}
	else if (m_secondsOne == 7)
	{
		m_timeHandleSecondsOne = m_numberUIHandle[7];
	}
	else if (m_secondsOne == 8)
	{
		m_timeHandleSecondsOne = m_numberUIHandle[8];
	}
	else if (m_secondsOne == 9)
	{
		m_timeHandleSecondsOne = m_numberUIHandle[9];
	}
}
