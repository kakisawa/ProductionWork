#pragma once
#include <memory>
#include "DxLib.h"
using namespace std;

// ゲームシーン基底クラス
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

	bool m_isNextSceneFlag;			// シーン切り替えフラグ
};