#include "EffekseerForDXLib.h"
#include "Pad.h"
#include "../Object/Player/Player.h"
#include "../Object/Enemy/EnemyBase.h"
#include "Effect.h"
#include <cassert>

// �萔
namespace
{
	constexpr int kEffectNum = 2;				// �G�t�F�N�g�̎��
	constexpr float kAttackEffectScale = 4.0f;	// �U���G�t�F�N�g�g�嗦
	constexpr int kAttackEffectTime = 10;		// �U���G�t�F�N�g�̍Đ�����
	constexpr float kDeathEffectScale = 8.0f;	// ���S���G�t�F�N�g�̊g�嗦
	constexpr int kDeathEffectTime = 10;		// ���S���G�t�F�N�g�̍Đ�����
	constexpr float kDeathEffectSpeed = 3.0f;	// ���S���G�t�F�N�g�̍Đ����x
}


/// <summary>
/// �R���X�g���N�^
/// </summary>
Effect::Effect() :
	m_attackEffectTime(0),
	m_deathEffectTime(0),
	m_attackHandle(-1)
{
	emitter.emitterHandle.resize(kEffectNum);
	emitter.emitterHandle[EffectKind::kAttack] = LoadEffekseerEffect("data/Effect/Use/attack.efk");
	emitter.emitterHandle[EffectKind::kDeath] = LoadEffekseerEffect("data/Effect/Use/Death.efk");
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
Effect::~Effect()
{
	for (auto& handle : emitter.emitterHandle)
	{
		DeleteEffekseerEffect(handle);
	}
}


/// <summary>
/// ������
/// </summary>
void Effect::Init()
{
	Effekseer_InitDistortion();	// �G�t�F�N�g�̘c�݂�K�p����
	ClearEffect();
	m_attackEffectTime = 0;
	m_deathEffectTime = 0;
}


///// <summary>
///// �X�V
///// </summary>
void Effect::Update()
{
	Effekseer_Sync3DSetting();	// 3D�̏���DxLib��Effekseer�ō��킹��
	UpdateEffekseer3D();

	m_attackEffectTime--;
	m_deathEffectTime--;
}


/// <summary>
/// �`��
/// </summary>
void Effect::Draw()
{
	DrawEffekseer3D();
}


/// <summary>
/// ��ʏ�̃G�t�F�N�g���폜����
/// </summary>
void Effect::ClearEffect()
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
void Effect::PlayDamageEffect(const VECTOR& pos)
{
	// �G�t�F�N�g��1��̂ݕ\�������悤�ɂ���
	if (m_attackEffectTime > 0) return;

	m_attackEffectTime = kAttackEffectTime;
	emitter.effects.push_back({ PlayEffekseer3DEffect(emitter.emitterHandle[EffectKind::kAttack]), {} });
	auto& effect = emitter.effects.back();

	// �G�t�F�N�g�̕\���ʒu��ݒ�
	SetPosPlayingEffekseer3DEffect(effect.handle, pos.x, pos.y, pos.z);
	SetScalePlayingEffekseer3DEffect(effect.handle, kAttackEffectScale, kAttackEffectScale, kAttackEffectScale);
}


/// <summary>
/// �K�[�h�G�t�F�N�g���Đ�����
/// </summary>
/// <param name="pos">�G�t�F�N�g�ʒu</param>
void Effect::PlayDeathEffect(const VECTOR& pos)
{
	// �K�[�h���̓G�t�F�N�g��1��̂ݕ\�������悤�ɂ���
	if (m_deathEffectTime > 0) return;

	m_deathEffectTime = kDeathEffectTime;
	emitter.effects.push_back({ PlayEffekseer3DEffect(emitter.emitterHandle[EffectKind::kDeath]), {} });
	auto& effect = emitter.effects.back();

	// �G�t�F�N�g�̕\���ʒu��ݒ�
	SetPosPlayingEffekseer3DEffect(effect.handle, pos.x, pos.y, pos.z);
	SetScalePlayingEffekseer3DEffect(effect.handle, kDeathEffectScale, kDeathEffectScale, kDeathEffectScale);
}
