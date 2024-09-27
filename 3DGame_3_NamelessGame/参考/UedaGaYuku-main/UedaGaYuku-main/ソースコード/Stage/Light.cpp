#include "DxLib.h"
#include "Player.h"
#include "Light.h"

// �萔
namespace
{
	/*�X�|�b�g���C�g*/
	constexpr float kSpotLightOutAngle = 45.0f;	 // �X�|�b�g���C�g�̉e���p�x
	constexpr float kSpotLightInAngle = 30.0f;	 // �X�|�b�g���C�g�̉e�����������n�߂�p�x
	constexpr float kSpotLightRange = 90.0f;	 // �L������
	constexpr float kSpotLighAtten0 = 0.5f;		 // ���������p�����[�^0
	constexpr float kSpotLighAtten1 = 0.01f;	 // ���������p�����[�^1
	constexpr float kSpotLighAtten2 = 0.1f;		 // ���������p�����[�^2
	const VECTOR kSpotLightPos = VGet(0.0f, 75.0f, -60.0f); // �X�|�b�g���C�g�ʒu
	const VECTOR kSpotLightDir = VGet(1.5f, -0.4f, 1.6f);	// �X�|�b�g���C�g����
}


/// <summary>
/// ���C�g�쐬
/// </summary>
void Light::Create(std::shared_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	m_spotLight = CreateSpotLightHandle(kSpotLightPos, kSpotLightDir, kSpotLightOutAngle, kSpotLightInAngle,
		kSpotLightRange, kSpotLighAtten0, kSpotLighAtten1, kSpotLighAtten2);
	m_dirLight = CreateDirLightHandle(VGet(0.0f, -1.0f, 0.0f));
}


/// <summary>
/// ���C�g�폜
/// </summary>
void Light::Delete()
{
	DeleteLightHandleAll();
}


/// <summary>
/// ���C�g�X�V
/// </summary>
void Light::Update()
{
	const VECTOR cameraPos = GetCameraPosition();  // �J�����ʒu�擾
	const VECTOR playerPos = m_pPlayer->GetPos();  // �v���C���[�ʒu�擾
	const VECTOR playerDir = VNorm(VSub(cameraPos, playerPos));  // �J��������v���C���[�̕����x�N�g�������߂�
	SetLightPositionHandle(m_spotLight, VAdd(playerPos, kSpotLightPos)); // �X�|�b�g���C�g�ʒu�X�V
}


#ifdef _DEBUG
/// <summary>
/// �`��
/// </summary>
void Light::Draw()
{
	DrawFormatString(0, 200, 0xffffff, "���C�g�ʒu(X:%.2f,Y:%.2f,Z;%.2f)", GetLightPositionHandle(m_spotLight).x, GetLightPositionHandle(m_spotLight).y, GetLightPositionHandle(m_spotLight).z);
	DrawFormatString(0, 220, 0xffffff, "���C�g����(X:%.2f,Y:%.2f,Z;%.2f)", GetLightDirectionHandle(m_spotLight).x, GetLightDirectionHandle(m_spotLight).y, GetLightDirectionHandle(m_spotLight).z);
}
#endif

