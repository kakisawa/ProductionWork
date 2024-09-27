#pragma once
#include <vector>

class Player;
class EnemyBase;

/// <summary>
/// �G�t�F�N�g�Ǘ��N���X
/// </summary>
class EffectManager
{
public:
	EffectManager();
	~EffectManager();
	void Init();
	void Update();
	void Draw();
	void ClearEffect();							// ��ʏ�̃G�t�F�N�g���~����
	void PlayDamageEffect(const VECTOR& pos);	// �U���G�t�F�N�g���Đ�����
	void PlayGuardEffect(const VECTOR& pos);	// �K�[�h�G�t�F�N�g���Đ�����

private:
	int m_attackEffectTime;	// �U���G�t�F�N�g���Đ����鎞��
	int m_guardEffectTime;	// �K�[�h�G�t�F�N�g���Đ����鎞��

	struct Effect
	{
		int handle;
		VECTOR pos;
	};

	// �p�[�e�B�N���𐶂ݏo������
	struct EffectEmitter
	{
		std::vector<int> emitterHandle;
		std::vector<Effect> effects;
	};
	EffectEmitter emitter;

	// �G�t�F�N�g�̎��
	enum EffectKind
	{
		kAttack,	// �U��
		kGuard,		// �K�[�h
	};
};