#pragma once
class Fade
{
public:
	Fade();
	virtual ~Fade();

	void Init();
	void Update();
	void Draw();

	void FadeIn(bool flag);
	void FadeOut(bool flag);

	void SetFadeOutFlag(bool flag) { m_isFadeOut = flag; }

	bool GatFadeInFlag() const { return m_isFadeIn; }
	bool GatFadeOutFlag() const { return m_isFadeOut; }
	bool GatNextSceneFlag() const { return m_isNextSceneFlag; }

private:
	int m_fadeAlpha;        // フェードイン、アウト
	bool m_isFadeIn;		// フェードイン用のフラグ
	bool m_isFadeOut;		// フェードアウト用のフラグ

	bool m_isNextSceneFlag;	// 次のシーンへ行くフラグ
};

