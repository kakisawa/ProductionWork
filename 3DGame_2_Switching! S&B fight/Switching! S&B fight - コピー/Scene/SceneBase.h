#pragma once
#include <memory>
#include "DxLib.h"
#include "../Manager/SoundManager.h"

using namespace std;

class Fade;
// �Q�[���V�[�����N���X
class SceneBase :
	public enable_shared_from_this<SceneBase>
{
public:
	SceneBase();
	virtual ~SceneBase(){}
	virtual void Init() {}
	virtual shared_ptr<SceneBase> Update() = 0;
	virtual void Draw() {}
	virtual void End();

protected:
	bool m_isNextSceneFlag;			// �V�[���؂�ւ��t���O

	std::shared_ptr<Fade> m_pFade;

	SoundManager* m_pSound;
};