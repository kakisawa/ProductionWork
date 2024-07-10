#include "DxLib.h"
#include "Game.h"
#include "Shader.h"
#include "SceneBase.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneBase::SceneBase()
{
	m_pShader = std::make_shared<Shader>();
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneBase::~SceneBase()
{
	m_pShader->UnLoad();
}
