#pragma once
class Fade
{
public:
	Fade();
	virtual ~Fade();

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
	/// 
	/// </summary>
	/// <param name="flag"></param>
	void FadeIn(bool flag);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="flag"></param>
	void FadeOut(bool flag);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="flag"></param>
	void SetFadeOutFlag(bool flag) { m_isFadeOut = flag; }

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool GatFadeInFlag() const { return m_isFadeIn; }

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool GatFadeOutFlag() const { return m_isFadeOut; }

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool GatNextSceneFlag() const { return m_isNextSceneFlag; }

private:
	int m_fadeAlpha;        // フェードイン、アウト
	bool m_isFadeIn;		// フェードイン用のフラグ
	bool m_isFadeOut;		// フェードアウト用のフラグ

	bool m_isNextSceneFlag;	// 次のシーンへ行くフラグ
};

