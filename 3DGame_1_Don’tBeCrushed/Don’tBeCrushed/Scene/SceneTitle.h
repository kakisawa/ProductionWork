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
	int m_model;		// モデル

	float m_scele;	// モデルサイズ

	VECTOR m_pos;			// プレイヤー位置

	shared_ptr<Camera> m_pCamera = make_shared<Camera>();
};