﻿#pragma once
#include <array>

class Time
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Time();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Time();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了
	/// </summary>
	void End();

	/// <summary>
	/// 秒数の計算
	/// </summary>
	void Calculation();

	/// <summary>
	/// 秒数にあたる画像をセットする
	/// </summary>
	void SetTimeNavyHandle();

	/// <summary>
	/// 秒数にあたる画像をセットする
	/// </summary>
	void SetTimeGreenHandle();

	void DrawClearTime();

	/// <summary>
	/// 制限時間終了フラグを渡す
	/// </summary>
	/// <returns>制限時間終了フラグ</returns>
	bool GetTimeUp() const { return m_isTimeUp; }

private:

	int m_minutes;		// 分数一桁の値
	int m_secondsTen;	// 秒数十桁の値
	int m_secondsOne;	// 秒数一桁の値

	int m_elapsedMinutes;		// 経過分数一桁の値
	int m_elapsedSecondsTen;	// 経過秒数十桁の値
	int m_elapsedSecondsOne;	// 経過秒数一桁の値

	int m_timeHandleMinites;	// 分数一桁画像
	int m_timeHandleSecondsTen;	// 秒数十桁画像
	int m_timeHandleSecondsOne;	// 秒数一桁の画像

	int m_elapsedTimeHandleMinites;		// 経過分数一桁画像
	int m_elapsedTimeHandleSecondsTen;	// 経過秒数十桁画像
	int m_elapsedTimeHandleSecondsOne;	// 経過秒数一桁画像

	int m_timeBgHandle;
	std::array<int, 11>m_numberNavyUIHandle{};	// 時間の紺UI画像
	std::array<int, 11>m_numberGreenUIHandle{};	// 時間の緑UI画像

	int m_second;		// 残りのゲームカウント...(制限時間)*60
	int m_elapsedTime;	// 経過カウント

	bool m_isTimeUp;	// 制限時間終了フラグ
};