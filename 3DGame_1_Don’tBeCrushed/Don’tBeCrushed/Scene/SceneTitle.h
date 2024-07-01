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
	int m_model;		// ���f��

	float m_scele;	// ���f���T�C�Y

	VECTOR m_pos;			// �v���C���[�ʒu

	shared_ptr<Camera> m_pCamera = make_shared<Camera>();
};