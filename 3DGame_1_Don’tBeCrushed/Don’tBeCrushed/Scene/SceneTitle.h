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
	int m_graph;		// タイトル画像
	int m_select;
	int m_select2;
	int m_select3;
	int m_model;		// モデル
	int m_fadeAlpha;	// フェードアウト

	float m_scele;		// モデルサイズ

	bool m_isFadeIn;	// フェードインフラグ
	bool m_isFadeOut;	// フェードアウトフラグ
	bool m_isSceneEnd;	// シーン切り替えフラグ

	VECTOR m_pos;		// モデル座標

	shared_ptr<Camera> m_pCamera = make_shared<Camera>();
};