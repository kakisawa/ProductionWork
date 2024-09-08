#pragma once
#include <vector>

class Player;
class EnemyBase;

/// <summary>
/// �G�t�F�N�g�Ǘ��N���X
/// </summary>
class Effect
{
public:
	Effect();
	~Effect();
	void Init();
	void Update();
	void Draw();
	void ClearEffect();							// ��ʏ�̃G�t�F�N�g���~����
	void PlayDamageEffect(const VECTOR& pos);	// �U���G�t�F�N�g���Đ�����
	void PlayDeathEffect(const VECTOR& pos);	// �K�[�h�G�t�F�N�g���Đ�����

private:
	int m_attackEffectTime;	// �U���G�t�F�N�g���Đ����鎞��
	int m_deathEffectTime;	// ���S���G�t�F�N�g���Đ����鎞��

	int m_attackHandle;



	struct Effect_
	{
		int handle;
		VECTOR pos;
	};

	// �p�[�e�B�N���𐶂ݏo������
	struct EffectEmitter
	{
		std::vector<int> emitterHandle;
		std::vector<Effect_> effects;
	};

	EffectEmitter emitter;

	// �G�t�F�N�g�̎��
	enum EffectKind
	{
		kAttack,	// �U��
		kDeath,		// ���S��
	};
};