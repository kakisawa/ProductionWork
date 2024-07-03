#pragma once
#include "SceneBase.h"
#include "DxLib.h"

using namespace std;
class Camera;
class SceneTitle :
	public SceneBase
{
public:
	SceneTitle();
	virtual ~SceneTitle();

	virtual void Init() {}
	virtual shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End();

private:
	int m_graph;		// �^�C�g���摜
	int m_select;
	int m_select2;
	int m_select3;
	int m_model;		// ���f��
	int m_fadeAlpha;	// �t�F�[�h�A�E�g

	float m_scele;		// ���f���T�C�Y

	bool m_isFadeIn;	// �t�F�[�h�C���t���O
	bool m_isFadeOut;	// �t�F�[�h�A�E�g�t���O
	bool m_isSceneEnd;	// �V�[���؂�ւ��t���O

	VECTOR m_pos;		// ���f�����W

	shared_ptr<Camera> m_pCamera = make_shared<Camera>();
};