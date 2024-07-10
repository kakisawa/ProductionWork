#pragma once

/// <summary>
// シェーダー管理クラス
/// </summary>
class Shader
{
public:
	Shader();
	~Shader();
	void Init();
	void Update();
	void Draw();
	void UnLoad();

private:
	// シェーダのハンドル
	int m_psH;
	int m_vsH;
	int m_rt;
};