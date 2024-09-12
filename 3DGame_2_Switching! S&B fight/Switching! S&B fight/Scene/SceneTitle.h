#pragma once
#include "SceneBase.h"
#include <array>

class SceneTitle :
	public SceneBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SceneTitle();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SceneTitle();

	/// <summary>
	/// ������
	/// </summary>
	virtual void Init();

	/// <summary>
	/// �X�V
	/// </summary>
	virtual shared_ptr<SceneBase> Update();

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// �I��
	/// </summary>
	void End() override;

	/// <summary>
	/// ���̃V�[����I��
	/// </summary>
	void SwitchingScene();

	/// <summary>
	/// �e���b�v�`��
	/// </summary>
	void DrawCaption();

private:
	int m_bgGraph;				// �w�i
	int m_titleGraph;			// �^�C�g�����S
	std::array<int, 13> m_uiGraph{};	// UI�p�摜

	float m_scrollX;			// �X�N���[���ړ���
	bool m_isMenu;				// ���j���[�\���t���O

	struct Size             // �e���b�v�摜�̃T�C�Y
	{
		int m_width;        // ��
		int m_height;       // �c
	};

	enum nextScene			// ���̃V�[��
	{
		kNone,				// ����
		kGameScene,			// �Q�[���V�[��
		kOptionScene,		// �I�v�V�����V�[��
		kRankingScene,		// �����L���O�V�[��
		kGameEnd,			// �Q�[���I��
	};

	struct Cursor			// �I�𒆂̃Z���N�g
	{
		VECTOR m_selectBox1;	// �l�p�n�_
		VECTOR m_selectBox2;	// �l�p�I�_
	}c1;

	nextScene m_nextScene;	// ���̃V�[��
};