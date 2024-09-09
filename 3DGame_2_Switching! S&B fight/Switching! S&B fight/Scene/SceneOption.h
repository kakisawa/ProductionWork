#pragma once
#include "SceneBase.h"
#include <array>

class SceneOption :
    public SceneBase
{
public:
	SceneOption();
	virtual ~SceneOption();

	virtual void Init();
	virtual shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End();

	void DrawSelect();
	void DrawExplanation();

private:
	enum Select
	{
		kBgmVolume,   // BGM
		kSeVolume,    // SE
		kBack,
	};

	struct Cursor
	{
		VECTOR m_selectBox1;	// �l�p�n�_
		VECTOR m_selectBox2;	// �l�p�I�_
	}c1;

	struct Size             // �����p�摜�̃T�C�Y
	{
		int m_width;        // ��
		int m_height;       // �c
	};

	// �X�N���[���ړ���
	float m_scrollX;

	std::array<int, 6> m_uiGraph{};	// UI�p�摜

	int m_select;


	// �w�i�N���W�b�g
	// ����ҁF<a href = "https://jp.freepik.com/free-photo/view-of-romantic-castle-bedroom_72464236.htm#query=%E5%9F%8E%E3%81%AE%E5%AF%9D%E5%AE%A4&position=0&from_view=keyword&track=ais_hybrid&uuid=99596e2f-76e1-482f-afa0-97f16ef53fab">Freepik< / a>
};

