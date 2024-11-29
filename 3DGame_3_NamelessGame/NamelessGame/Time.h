#pragma once
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

	void Calculation();

	/// <summary>
	/// 制限時間終了フラグを渡す
	/// </summary>
	/// <returns>制限時間終了フラグ</returns>
	bool GetTimeUp() const { return m_isTimeUp; }

private:

	int m_minutes;		// 分数一桁の値
	int m_seconds;		// 秒数十桁の値
	int m_minutes;		// 秒数一桁の値


	std::array<int, 11>m_numberUIHandle{};	// 時間のUI画像

	int m_second;		// 残りのゲーム秒数(制限時間)*60

	bool m_isTimeUp;	// 制限時間終了フラグ
};