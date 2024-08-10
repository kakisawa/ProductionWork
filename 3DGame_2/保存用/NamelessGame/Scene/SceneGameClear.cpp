#include "SceneGameClear.h"
#include "SceneTitle.h"
#include "../Pad.h"

SceneGameClear::SceneGameClear()
{
}

SceneGameClear::~SceneGameClear()
{
}

void SceneGameClear::Init()
{
}

shared_ptr<SceneBase> SceneGameClear::Update()
{
	

	if (m_isNextSceneFlag)
	{

		if (Pad::IsTrigger(PAD_INPUT_Z))		// RB�{�^��
		{
			return make_shared<SceneTitle>();	// �^�C�g���V�[���֍s��
		}
	}

#ifdef _DEBUG
	// �f�o�b�O�p?(�t�F�[�h���ꂽ��g��Ȃ�����)
	if (Pad::IsNotPress(PAD_INPUT_Z)) {	// RB�{�^��
		m_isNextSceneFlag = true;
	}
#endif // DEBUG

	return shared_from_this();
}

void SceneGameClear::Draw()
{
	DrawString(0, 0, "SceneGameClear", 0xffffff);
	DrawString(0, 20, "Please Press Button RB", 0x00ffff);
}

void SceneGameClear::End()
{
}
