#pragma once
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
	/// 制限時間終了フラグを渡す
	/// </summary>
	/// <returns>制限時間終了フラグ</returns>
	bool GetTimeUp() const { return m_isTimeUp; }

private:
	float m_second;		// 残りのゲーム秒数(制限時間)*60

	bool m_isTimeUp;	// 制限時間終了フラグ
};