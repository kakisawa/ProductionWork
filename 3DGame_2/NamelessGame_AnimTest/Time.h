#pragma once
class Time
{
public:
	Time(int rimitTime);
	~Time();

	// �^�C�����Z�b�g
	void Reset();

	// �^�C���X�V
	bool Update();

private:
	int m_time;			// �o�ߎ���
	int m_rimitTime;	// ���~�b�g����
};