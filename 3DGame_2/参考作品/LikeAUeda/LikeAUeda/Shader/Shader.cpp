#include "DxLib.h"
#include "Game.h"
#include "Shader.h"

// 定数
namespace
{
	const char* kPsHName = "Shader/PixelShader.pso";  // ピクセルシェーダ
	const char* kVsHName = "Shader/VertexShader.vso"; // 頂点シェーダ
}

/// <summary>
/// コンストラクタ
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
/// デストラクタ
/// </summary>
Shader::~Shader()
{
	DeleteShader(m_vsH);
	DeleteShader(m_psH);
	DeleteShader(m_rt);
}


/// <summary>
/// 初期化
/// </summary>
void Shader::Init()
{
	// シェーダを定義する
	MV1SetUseOrigShader(true);
	SetUseVertexShader(m_vsH);
	SetUsePixelShader(m_psH);
}


/// <summary>
/// 更新
/// </summary>
void Shader::Update()
{
	SetDrawScreen(m_rt);
	ClearDrawScreen();

	// 使用するシェーダをセットする
	SetUseVertexShader(m_vsH);
	SetUsePixelShader(m_psH);

	SetRenderTargetToShader(0, m_rt);
}


/// <summary>
/// 描画
/// </summary>
void Shader::Draw()
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	UnLoad();
}


/// <summary>
/// シェーダー解除
/// </summary>
void Shader::UnLoad()
{
	MV1SetUseOrigShader(false);
	SetUsePixelShader(-1);
	SetUseVertexShader(-1);
	SetRenderTargetToShader(0, -1);
}
