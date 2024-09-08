#pragma once
#include "SceneBase.h"
#include <array>

class SceneTitle :
	public SceneBase
{
public:
	SceneTitle();
	virtual ~SceneTitle();

	virtual void Init();
	virtual shared_ptr<SceneBase> Update();
	virtual void Draw();
	void End() override;

	/// <summary>
	/// ���̃V�[����I��
	/// </summary>
	void SwitchingScene();

	void DrawExplanation();

	enum nextScene
	{
		kNone,
		kGameScene,
		kOptionScene,
		kRankingScene,
		kGameEnd,
	};

private:
	int m_bgGraph;

	int m_titleGraph;
	int m_pressBgGraph;
	int m_pressEnyButtonGraph;

	bool m_isSelect;
	bool m_isBackScene;
	
	std::array<int, 13> m_uiGraph{};	// UI�p�摜

	VECTOR m_explanationBgPos;
	VECTOR m_explanationPos;

	struct Size             // �����p�摜�̃T�C�Y
	{
		int m_width;        // ��
		int m_height;       // �c
	};

	// �X�N���[���ړ���
	float m_scrollX_1;

	struct Cursor
	{
		VECTOR m_selectBox1;	// �l�p�n�_
		VECTOR m_selectBox2;	// �l�p�I�_
	}c1;

	nextScene m_nextScene;
};