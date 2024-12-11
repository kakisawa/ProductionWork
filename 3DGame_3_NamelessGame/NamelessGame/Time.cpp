#include "Time.h"
#include "DxLib.h"

namespace {
	constexpr int kMaxtimeLimit = 10800;	// 制限時間(3分=180秒)

	const char* const kNumberNavyUI[11]{
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
	const char* const kNumberGreenUI[11]{
		"Data/Image/SceneGame/Clear/0.png",
		"Data/Image/SceneGame/Clear/1.png",
		"Data/Image/SceneGame/Clear/2.png",
		"Data/Image/SceneGame/Clear/3.png",
		"Data/Image/SceneGame/Clear/4.png",
		"Data/Image/SceneGame/Clear/5.png",
		"Data/Image/SceneGame/Clear/6.png",
		"Data/Image/SceneGame/Clear/7.png",
		"Data/Image/SceneGame/Clear/8.png",
		"Data/Image/SceneGame/Clear/9.png",
		"Data/Image/SceneGame/Clear/点.png",
	};

	constexpr int kMinutes = 3600;	// 1分の値
	constexpr int kSecondOne = 60;	// 1秒の値
}

Time::Time():
	m_minutes(0),
	m_elapsedTime(0),
	m_secondsTen(0),
	m_secondsOne(0),
	m_elapsedMinutes(0),
	m_elapsedSecondsTen(0),
	m_elapsedSecondsOne(0),
	m_timeHandleMinites(-1),
	m_timeHandleSecondsTen(-1),
	m_timeHandleSecondsOne(-1),
	m_second(kMaxtimeLimit),
	m_isTimeUp(false)
{
	// 数字UI画像読み込み
	for (int i = 0; i < m_numberNavyUIHandle.size(); i++)
	{
		m_numberNavyUIHandle[i] = LoadGraph(kNumberNavyUI[i]);
		m_numberGreenUIHandle[i] = LoadGraph(kNumberGreenUI[i]);
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
	m_elapsedTime++;

	// 表示用時間計算
	m_minutes = m_second / kMinutes;
	m_secondsTen = ((m_second % kMinutes) / kSecondOne) / 10;
	m_secondsOne= ((m_second % kMinutes) / kSecondOne) % 10;

	// 経過時間計算
	m_elapsedMinutes = m_elapsedTime / kMinutes;
	m_elapsedSecondsTen = ((m_elapsedTime % kMinutes) / kSecondOne) / 10;
	m_elapsedSecondsOne = ((m_elapsedTime % kMinutes) / kSecondOne) % 10;

	SetTimeNavyHandle();
	SetTimeGreenHandle();

	// 制限時間が0になったら
	if (m_second <= 0.0f)
	{
		m_isTimeUp = true;
	}
}

void Time::Draw()
{
	DrawGraph(10, 20, m_timeBgHandle, true);

	DrawGraph(35, 45, m_timeHandleMinites, true);
	DrawGraph(90, 50, m_numberNavyUIHandle[10], true);
	DrawGraph(110, 45, m_timeHandleSecondsTen, true);
	DrawGraph(155, 45, m_timeHandleSecondsOne, true);

#ifdef _DEBUG
	//DrawFormatString(0, 1000, 0xffffff, "m_second=%d", m_elapsedTime);
	//DrawFormatString(0, 1020, 0xffffff, "残り時間:分=%d", m_elapsedMinutes);
	//DrawFormatString(0, 1040, 0xffffff, "残り時間:十秒=%d", m_elapsedSecondsTen);
	//DrawFormatString(0, 1060, 0xffffff, "残り時間:秒=%d", m_elapsedSecondsOne);

	/*DrawFormatString(0, 1000, 0xffffff, "m_second=%.2f", m_second / 60);
	DrawFormatString(0, 1020, 0xffffff,"残り時間:分=%d", m_minutes);
	DrawFormatString(0, 1040, 0xffffff,"残り時間:十秒=%d", m_secondsTen);
	DrawFormatString(0, 1060, 0xffffff,"残り時間:秒=%d", m_secondsOne);*/

#endif // DEBUG
}

void Time::End()
{
	// 数字UI画像読み込み
	for (int i = 0; i < m_numberNavyUIHandle.size(); i++)
	{
		DeleteGraph(m_numberNavyUIHandle[i]);
		DeleteGraph(m_numberGreenUIHandle[i]);
	}

	DeleteGraph(m_timeBgHandle);
}

void Time::SetTimeNavyHandle()
{
	if (m_minutes == 0)
	{
		m_timeHandleMinites = m_numberNavyUIHandle[0];
	}
	else if (m_minutes == 1)
	{
		m_timeHandleMinites = m_numberNavyUIHandle[1];
	}
	else if (m_minutes == 2)
	{
		m_timeHandleMinites = m_numberNavyUIHandle[2];
	}
	else if (m_minutes == 3)
	{
		m_timeHandleMinites = m_numberNavyUIHandle[3];
	}


	if (m_secondsTen == 0)
	{
		m_timeHandleSecondsTen = m_numberNavyUIHandle[0];
	}
	else if (m_secondsTen == 1)
	{
		m_timeHandleSecondsTen = m_numberNavyUIHandle[1];
	}
	else if (m_secondsTen == 2)
	{
		m_timeHandleSecondsTen = m_numberNavyUIHandle[2];
	}
	else if (m_secondsTen == 3)
	{
		m_timeHandleSecondsTen = m_numberNavyUIHandle[3];
	}
	else if (m_secondsTen == 4)
	{
		m_timeHandleSecondsTen = m_numberNavyUIHandle[4];
	}
	else if (m_secondsTen == 5)
	{
		m_timeHandleSecondsTen = m_numberNavyUIHandle[5];
	}
	else if (m_secondsTen == 6)
	{
		m_timeHandleSecondsTen = m_numberNavyUIHandle[6];
	}
	else if (m_secondsTen == 7)
	{
		m_timeHandleSecondsTen = m_numberNavyUIHandle[7];
	}
	else if (m_secondsTen == 8)
	{
		m_timeHandleSecondsTen = m_numberNavyUIHandle[8];
	}
	else if (m_secondsTen == 9)
	{
		m_timeHandleSecondsTen = m_numberNavyUIHandle[9];
	}


	if (m_secondsOne == 0)
	{
		m_timeHandleSecondsOne = m_numberNavyUIHandle[0];
	}
	else if (m_secondsOne == 1)
	{
		m_timeHandleSecondsOne = m_numberNavyUIHandle[1];
	}
	else if (m_secondsOne == 2)
	{
		m_timeHandleSecondsOne = m_numberNavyUIHandle[2];
	}
	else if (m_secondsOne == 3)
	{
		m_timeHandleSecondsOne = m_numberNavyUIHandle[3];
	}
	else if (m_secondsOne == 4)
	{
		m_timeHandleSecondsOne = m_numberNavyUIHandle[4];
	}
	else if (m_secondsOne == 5)
	{
		m_timeHandleSecondsOne = m_numberNavyUIHandle[5];
	}
	else if (m_secondsOne == 6)
	{
		m_timeHandleSecondsOne = m_numberNavyUIHandle[6];
	}
	else if (m_secondsOne == 7)
	{
		m_timeHandleSecondsOne = m_numberNavyUIHandle[7];
	}
	else if (m_secondsOne == 8)
	{
		m_timeHandleSecondsOne = m_numberNavyUIHandle[8];
	}
	else if (m_secondsOne == 9)
	{
		m_timeHandleSecondsOne = m_numberNavyUIHandle[9];
	}
}

void Time::SetTimeGreenHandle()
{

	if (m_elapsedMinutes == 0)
	{
		m_elapsedTimeHandleMinites = m_numberGreenUIHandle[0];
	}
	else if (m_elapsedMinutes == 1)
	{
		m_elapsedTimeHandleMinites = m_numberGreenUIHandle[1];
	}
	else if (m_elapsedMinutes == 2)
	{
		m_elapsedTimeHandleMinites = m_numberGreenUIHandle[2];
	}
	else if (m_elapsedMinutes == 3)
	{
		m_elapsedTimeHandleMinites = m_numberGreenUIHandle[3];
	}


	if (m_elapsedSecondsTen == 0)
	{
		m_elapsedTimeHandleSecondsTen = m_numberGreenUIHandle[0];
	}
	else if (m_elapsedSecondsTen == 1)
	{
		m_elapsedTimeHandleSecondsTen = m_numberGreenUIHandle[1];
	}
	else if (m_elapsedSecondsTen == 2)
	{
		m_elapsedTimeHandleSecondsTen = m_numberGreenUIHandle[2];
	}
	else if (m_elapsedSecondsTen == 3)
	{
		m_elapsedTimeHandleSecondsTen = m_numberGreenUIHandle[3];
	}
	else if (m_elapsedSecondsTen == 4)
	{
		m_elapsedTimeHandleSecondsTen = m_numberGreenUIHandle[4];
	}
	else if (m_elapsedSecondsTen == 5)
	{
		m_elapsedTimeHandleSecondsTen = m_numberGreenUIHandle[5];
	}
	else if (m_elapsedSecondsTen == 6)
	{
		m_elapsedTimeHandleSecondsTen = m_numberGreenUIHandle[6];
	}
	else if (m_elapsedSecondsTen == 7)
	{
		m_elapsedTimeHandleSecondsTen = m_numberGreenUIHandle[7];
	}
	else if (m_elapsedSecondsTen == 8)
	{
		m_elapsedTimeHandleSecondsTen = m_numberGreenUIHandle[8];
	}
	else if (m_elapsedSecondsTen == 9)
	{
		m_elapsedTimeHandleSecondsTen = m_numberGreenUIHandle[9];
	}


	if (m_elapsedSecondsOne == 0)
	{
		m_elapsedTimeHandleSecondsOne = m_numberGreenUIHandle[0];
	}
	else if (m_elapsedSecondsOne == 1)
	{
		m_elapsedTimeHandleSecondsOne = m_numberGreenUIHandle[1];
	}
	else if (m_elapsedSecondsOne == 2)
	{
		m_elapsedTimeHandleSecondsOne = m_numberGreenUIHandle[2];
	}
	else if (m_elapsedSecondsOne == 3)
	{
		m_elapsedTimeHandleSecondsOne = m_numberGreenUIHandle[3];
	}
	else if (m_elapsedSecondsOne == 4)
	{
		m_elapsedTimeHandleSecondsOne = m_numberGreenUIHandle[4];
	}
	else if (m_elapsedSecondsOne == 5)
	{
		m_elapsedTimeHandleSecondsOne = m_numberGreenUIHandle[5];
	}
	else if (m_elapsedSecondsOne == 6)
	{
		m_elapsedTimeHandleSecondsOne = m_numberGreenUIHandle[6];
	}
	else if (m_elapsedSecondsOne == 7)
	{
		m_elapsedTimeHandleSecondsOne = m_numberGreenUIHandle[7];
	}
	else if (m_elapsedSecondsOne == 8)
	{
		m_elapsedTimeHandleSecondsOne = m_numberGreenUIHandle[8];
	}
	else if (m_elapsedSecondsOne == 9)
	{
		m_elapsedTimeHandleSecondsOne = m_numberGreenUIHandle[9];
	}
}

void Time::DrawClearTime()
{
	DrawGraph(1024, 870, m_elapsedTimeHandleMinites, true);
	DrawGraph(1120, 870, m_numberGreenUIHandle[10], true);
	DrawGraph(1164, 870, m_elapsedTimeHandleSecondsTen, true);
	DrawGraph(1236, 870, m_elapsedTimeHandleSecondsOne, true);
}
