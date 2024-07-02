#pragma once
#include "SceneBase.h"

using namespace std;
class SceneGameOver :
	public SceneBase
{
public:
	SceneGameOver();
	virtual ~SceneGameOver();

	virtual void Init() {}
	virtual shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End() {}

private:
	int graph;
	int m_fadeAlpha;        // �t�F�[�h�C���A�A�E�g

	bool m_isFadeIn;		// �t�F�[�h�C���p�̃t���O
	bool m_isFadeOut;		// �t�F�[�h�A�E�g�p�̃t���O
	bool m_isSceneEnd;		// �V�[���J�ڗp�̃t���O
};