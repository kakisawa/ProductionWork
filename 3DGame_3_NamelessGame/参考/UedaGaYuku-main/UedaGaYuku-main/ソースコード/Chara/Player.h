#pragma once
#include "CharacterBase.h"
#include <memory>

class Stage;
class EnemyBase;
class Camera;
class Input;
class UIGauge;
class EffectManager;

/// <summary>
/// �v���C���[�N���X
/// </summary>
class Player : public CharacterBase 
{
public:
	Player();
	~Player();
	virtual void Init(std::shared_ptr<EffectManager> pEffect, VECTOR pos) override;
	void Update(const Input& input, const Camera& camera, EnemyBase& enemy, Stage& stage);
	virtual void Draw() override;

	// �_���[�W���󂯂��ۂ̏���
	virtual void OnDamage(float damage) override;
	// �񕜏���
	void Recovery();
	// �G�l�~�[�Ƃ̓����蔻����`�F�b�N����
	void CheckHitEnemyCol(EnemyBase& enemy, VECTOR eCapPosTop, VECTOR eCapPosBottom, float eCapRadius);

	bool GetIsMove() const { return m_isMove; }						// �ړ������ǂ����擾
	bool GetIsSpecialAttack() const{ return m_isSpecialAttack; }	// �K�E�Z�𔭓������ǂ����擾
	VECTOR GetDir() const { return m_targetMoveDir; }				// �v���C���[�̌����x�N�g�����擾

private:
	// �ړ�����
	void Move(const VECTOR& moveVec, Stage& stage);
	// �p���`�U������
	void Punch(const Input& input);
	// �L�b�N�U������
	void Kick(const Input& input);
	// �������
	void Avoid(const Input& input, Stage& stage, VECTOR& moveVec);
	// �\������
	void Fighting(const Input& input);
	// �K�[�h����
	void Guard(const Input& input);
	// �K�[�h��Ԃ���������
	void OffGuard();
	// �U�����󂯂Ă���Œ��̏���
	void Receive();
	// �K�E�Z
	void SpecialAttack(const Input& input, EnemyBase& enemy);
	//�U�����ʒu����
	void AdjAttackPos(EnemyBase& enemy);
	// �G��|�����Ƃ�
	void DestroyEnemy();
	// �Q�[���I�[�o�[��
	void Gameover();
	// �ړ��p�����[�^��ݒ肷��
	CharacterBase::State UpdateMoveParameter(const Input& input, const Camera& camera, VECTOR& upMoveVec, VECTOR& leftMoveVec, VECTOR& moveVec);
	// �v���C���[�̊p�x���X�V
	void UpdateAngle(EnemyBase& enemy);
	// �U�����̊p�x���X�V
	void UpdateAttackAngle();
	// �p�b�h�̐U����������
	void VibrationPad();

private:
	// �v���C���[���
	VECTOR m_pToEVec;			// �v���C���[����G�Ɍ������x�N�g��
	VECTOR m_targetMoveDir;		// �����ׂ������̃x�N�g��
	float m_gauge;				// �Q�[�W��
	bool m_isAccumulateGaugeSe;	// �Q�[�W�����܂���SE���Đ����邩�ǂ���(true:�Đ�����)
};
