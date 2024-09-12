#pragma once
#include "SceneBase.h"
#include <array>

class SceneRanking :
    public SceneBase
{
public:
	SceneRanking();
	virtual ~SceneRanking();

	virtual void Init();
	virtual shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End();

	void DrawExplanation();


private:
	std::array<int, 4> m_uiGraph{};	// UI�p�摜

	float m_scrollX_1;		// �X�N���[���ړ���

	struct Size             // �����p�摜�̃T�C�Y
	{
		int m_width;        // ��
		int m_height;       // �c
	};
};