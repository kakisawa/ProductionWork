#pragma once
#include <memory>

class Stage;
class Camera;
class Input;
class UIBattle;
class Shader;

/// <summary>
/// �v���C���[�N���X
/// </summary>
class Player
{
public:
	// �v���C���[�̏��
	enum class State
	{
		kKick = 1,	// �L�b�N
		kPunch = 2,	// �p���`
		kRun = 3,	// �ړ�
		kStand = 4, // �ҋ@
	};

	// �A�j���[�V�������
	enum class AnimKind
	{
		kNone = -1,		// �Ȃ�
		kUnKnown = 0,	// �s��
		kKick = 1,		// �L�b�N
		kPunch = 2,		// �p���`
		kRun = 3,		// �ړ�
		kStand = 4,		// �ҋ@
	};

	Player();
	~Player();

	void Init(std::shared_ptr<Shader> Shader);
	void Update(const Input& input, const Camera& camera, Stage& stage);
	void Draw();

	void OnHitFloor();	// ���ɓ���������

	const VECTOR GetPos() const { return m_pos; }	// �v���C���[�̌��ݒn�擾
	bool GetIsMove() const { return m_isMove; }		// �ړ������ǂ����擾

private:
	// �ړ�����
	void Move(const VECTOR& MoveVector, Stage& stage);
	// �ړ��p�����[�^��ݒ肷��
	State UpdateMoveParameter(const Input& input, const Camera& camera, VECTOR& upMoveVec, VECTOR& leftMoveVec, VECTOR& moveVec);
	// �U������
	State Attack(const Input& input);
	// �v���C���[�̉�]�𐧌䂷��
	void UpdateAngle();
	// �A�j���[�V�����X�e�[�g�̍X�V
	void UpdateAnimState(State prevState);
	// �A�j���[�V��������
	void UpdateAnim();
	// �A�j���[�V�������Đ�����
	void PlayAnim(AnimKind PlayAnimIndex);

private:
	std::shared_ptr<UIBattle> m_pUIBattle;
	std::shared_ptr<Shader> m_pShader;

	// �v���C���[���
	float m_hp;					// HP
	float m_gauge;				// �Q�[�W��
	VECTOR m_pos;				// �ʒu
	bool m_isMove;				// �ړ��������ǂ���(true:�ړ�����)
	bool m_isAttack;			// �U�������ǂ���(ture:�U����)
	VECTOR m_targetMoveDir;		// �����ׂ������̃x�N�g��
	float m_angle;				// �����Ă�������̊p�x
	float m_jumpPower;			// Y�������̑��x
	float m_moveSpeed;			// �ړ����x
	State m_currentState;		// ���݂̏��
	int m_modelHandle;			// �v���C���[��3D���f��

	// �A�j���[�V�������
	int m_currentPlayAnim;		// �Đ����̃A�j���[�V�����̃A�^�b�`�ԍ�
	float m_currentAnimCount;	// �Đ����̃A�j���[�V�����̍Đ�����
	int m_prevPlayAnim;			// �O�̍Đ��A�j���[�V�����̃A�^�b�`�ԍ�
	float m_prevAnimCount;		// �O�̍Đ��A�j���[�V�����̍Đ�����
	float m_animBlendRate;		// ���݂Ɖߋ��̃A�j���[�V�����̃u�����h��
};

