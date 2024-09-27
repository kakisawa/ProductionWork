#include "EffekseerForDXLib.h"
#include "Input.h"
#include "Player.h"
#include "EnemyBase.h"
#include "EffectManager.h"

// �萔
namespace
{
	constexpr int kEffectNum = 2;				// �G�t�F�N�g�̎��
	constexpr float kAttackEffectScale = 4.0f;	// �U���G�t�F�N�g�g�嗦
	constexpr int kAttackEffectTime = 80;		// �U���G�t�F�N�g�̍Đ�����
	constexpr float kGaurdEffectScale = 8.0f;	// �K�[�h�G�t�F�N�g�̊g�嗦
	constexpr int kGaurdEffectTime = 30;		// �K�[�h�G�t�F�N�g�̍Đ�����
	constexpr float kGaurdEffectSpeed= 3.0f;	// �K�[�h�G�t�F�N�g�̍Đ����x
}


/// <summary>
/// �R���X�g���N�^
/// </summary>
EffectManager::EffectManager():
	m_attackEffectTime(0),
	m_guardEffectTime(0)
{
	emitter.emitterHandle.resize(kEffectNum);
	emitter.emitterHandle[EffectKind::kAttack] = LoadEffekseerEffect("data/Effect/attack.efk");
	emitter.emitterHandle[EffectKind::kGuard] = LoadEffekseerEffect("data/Effect/guard.efk");
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
EffectManager::~EffectManager()
{
	for (auto& handle : emitter.emitterHandle)
	{
		DeleteEffekseerEffect(handle);
	}
}


/// <summary>
/// ������
/// </summary>
void EffectManager::Init()
{
	Effekseer_InitDistortion();	// �G�t�F�N�g�̘c�݂�K�p����
	ClearEffect();
	m_attackEffectTime = 0;
	m_guardEffectTime = 0;
}


///// <summary>
///// �X�V
///// </summary>
void EffectManager::Update()
{
	Effekseer_Sync3DSetting();	// 3D�̏���DxLib��Effekseer�ō��킹��
	UpdateEffekseer3D();

	m_attackEffectTime--;
	m_guardEffectTime--;
}


/// <summary>
/// �`��
/// </summary>
void EffectManager::Draw()
{
	DrawEffekseer3D();
}


/// <summary>
/// ��ʏ�̃G�t�F�N�g���폜����
/// </summary>
void EffectManager::ClearEffect()
{
	for (auto& e : emitter.effects)
	{
		StopEffekseer3DEffect(e.handle);
	}
	emitter.effects.clear();
}


/// <summary>
/// �U���G�t�F�N�g���Đ�����
/// </summary>
/// <param name="pos">�G�t�F�N�g�ʒu</param>
void EffectManager::PlayDamageEffect(const VECTOR& pos)
{
	// �G�t�F�N�g��1��̂ݕ\�������悤�ɂ���
	if (m_attackEffectTime > 0) return;

	m_attackEffectTime = kAttackEffectTime;
	emitter.effects.push_back({ PlayEffekseer3DEffect(emitter.emitterHandle[EffectKind::kAttack]), {}});
	auto& effect = emitter.effects.back();

	// �G�t�F�N�g�̕\���ʒu��ݒ�
	SetPosPlayingEffekseer3DEffect(effect.handle, pos.x, pos.y, pos.z);
	SetScalePlayingEffekseer3DEffect(effect.handle, kAttackEffectScale, kAttackEffectScale, kAttackEffectScale);
}


/// <summary>
/// �K�[�h�G�t�F�N�g���Đ�����
/// </summary>
/// <param name="pos">�G�t�F�N�g�ʒu</param>
void EffectManager::PlayGuardEffect(const VECTOR& pos)
{
	// �K�[�h���̓G�t�F�N�g��1��̂ݕ\�������悤�ɂ���
	if (m_guardEffectTime > 0) return;

	m_guardEffectTime = kGaurdEffectTime;
	emitter.effects.push_back({ PlayEffekseer3DEffect(emitter.emitterHandle[EffectKind::kGuard]), {} });
	auto& effect = emitter.effects.back();

	// �G�t�F�N�g�̕\���ʒu��ݒ�
	SetPosPlayingEffekseer3DEffect(effect.handle, pos.x, pos.y, pos.z);
	SetScalePlayingEffekseer3DEffect(effect.handle, kGaurdEffectScale, kGaurdEffectScale, kGaurdEffectScale);
	SetSpeedPlayingEffekseer3DEffect(effect.handle, kGaurdEffectSpeed);
}
