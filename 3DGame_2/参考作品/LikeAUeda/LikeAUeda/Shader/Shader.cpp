#include "DxLib.h"
#include "Game.h"
#include "Shader.h"

// �萔
namespace
{
	const char* kPsHName = "Shader/PixelShader.pso";  // �s�N�Z���V�F�[�_
	const char* kVsHName = "Shader/VertexShader.vso"; // ���_�V�F�[�_
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Shader::Shader():
	m_psH(-1),
	m_vsH(-1),
	m_rt(-1)
{
	m_rt = MakeScreen(Game::kScreenWidth, Game::kScreenHeight, true);
	m_vsH = LoadVertexShader(kVsHName);
	m_psH = LoadPixelShader(kPsHName);
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
Shader::~Shader()
{
	DeleteShader(m_vsH);
	DeleteShader(m_psH);
	DeleteShader(m_rt);
}


/// <summary>
/// ������
/// </summary>
void Shader::Init()
{
	// �V�F�[�_���`����
	MV1SetUseOrigShader(true);
	SetUseVertexShader(m_vsH);
	SetUsePixelShader(m_psH);
}


/// <summary>
/// �X�V
/// </summary>
void Shader::Update()
{
	SetDrawScreen(m_rt);
	ClearDrawScreen();

	// �g�p����V�F�[�_���Z�b�g����
	SetUseVertexShader(m_vsH);
	SetUsePixelShader(m_psH);

	SetRenderTargetToShader(0, m_rt);
}


/// <summary>
/// �`��
/// </summary>
void Shader::Draw()
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	UnLoad();
}


/// <summary>
/// �V�F�[�_�[����
/// </summary>
void Shader::UnLoad()
{
	MV1SetUseOrigShader(false);
	SetUsePixelShader(-1);
	SetUseVertexShader(-1);
	SetRenderTargetToShader(0, -1);
}
