#include "DxLib.h"
#include "Game.h"
#include "Shader.h"
#include "SceneBase.h"


/// <summary>
/// コンストラクタ
/// </summary>
SceneBase::SceneBase()
{
	m_pShader = std::make_shared<Shader>();
}


/// <summary>
/// デストラクタ
/// </summary>
SceneBase::~SceneBase()
{
	m_pShader->UnLoad();
}
