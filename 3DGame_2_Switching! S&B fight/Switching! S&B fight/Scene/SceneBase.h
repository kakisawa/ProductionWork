#pragma once
#include <memory>
#include "DxLib.h"
using namespace std;

// �Q�[���V�[�����N���X
class SceneBase :
	public enable_shared_from_this<SceneBase>
{
public:
	SceneBase();
	virtual ~SceneBase() {}
	virtual void Init() {}
	virtual shared_ptr<SceneBase> Update() = 0;
	virtual void Draw() {}
	virtual void End() {}

protected:

	bool m_isNextSceneFlag;			// �V�[���؂�ւ��t���O
};