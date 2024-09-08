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
	int m_fadeAlpha;        // �t�F�[�h�C���A�A�E�g
	bool m_isFadeIn;		// �t�F�[�h�C���p�̃t���O
	bool m_isFadeOut;		// �t�F�[�h�A�E�g�p�̃t���O

	bool m_isNextSceneFlag;	// ���̃V�[���֍s���t���O
};

