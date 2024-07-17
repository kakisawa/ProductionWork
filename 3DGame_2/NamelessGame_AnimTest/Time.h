#pragma once
class Time
{
public:
	Time(int rimitTime);
	~Time();

	// タイムリセット
	void Reset();

	// タイム更新
	bool Update();

private:
	int m_time;			// 経過時間
	int m_rimitTime;	// リミット時間
};