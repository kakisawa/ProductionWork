#pragma once

/// <summary>
// �V�F�[�_�[�Ǘ��N���X
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
	// �V�F�[�_�̃n���h��
	int m_psH;
	int m_vsH;
	int m_rt;
};