#include "Time.h"

Time::Time(int rimitTime) :
	m_time(0),
	m_rimitTime(rimitTime)
{
}

Time::~Time()
{
}

//�^�C�����Z�b�g
void Time::Reset()
{
	m_time = 0;
}

bool Time::Update()
{
	//���~�b�g���Ԃ𒴂�����true��Ԃ�
	if (m_time >= m_rimitTime) return true;
	m_time++;

	return false;
}
