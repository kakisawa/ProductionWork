#include "SceneRanking.h"
#include "SceneTitle.h"
#include "../Pad.h"

SceneRanking::SceneRanking()
{
}

SceneRanking::~SceneRanking()
{
}

void SceneRanking::Init()
{
}

shared_ptr<SceneBase> SceneRanking::Update()
{
	
	if (m_isNextSceneFlag)
	{
		if (Pad::IsTrigger(PAD_INPUT_Z))		// RB�{�^��
		{
			return make_shared<SceneTitle>();	// �^�C�g���V�[���֍s��
		}
	}

	DrawString(0, 20, "Please Press Button RB", 0x00ffff);

#ifdef _DEBUG
	// �f�o�b�O�p?(�t�F�[�h���ꂽ��g��Ȃ�����)
	if (Pad::IsNotPress(PAD_INPUT_Z)) {	// RB�{�^��
		m_isNextSceneFlag = true;
	}
#endif // DEBUG

	return shared_from_this();
}

void SceneRanking::Draw()
{
	DrawString(0, 0, "SceneRanking", 0xffffff);
}

void SceneRanking::End()
{
}
