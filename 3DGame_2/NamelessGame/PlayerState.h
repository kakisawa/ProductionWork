#pragma once
#include <vector>
#include <memory>
#include <functional>

class PlayerState
{
public:
	// �v���C���[�̏��
	enum class State
	{
		kIdle = 1,	// �ҋ@
		kWalk = 2,	// �ړ�
		kJump = 3,	// �W�����v
		kAttack = 4,// �U��
	};

public:
	PlayerState();		// PlayerState�̃R���X�g���N�^
	~PlayerState() {};

	void Update();	//�X�V���� 

	/// <summary>
	/// �X�e�C�g�̒ǉ�
	/// </summary>
	/// <param name="stateUpdate">�X�V�֐�</param>
	/// <param name="stateInit">�������֐�</param>
	/// <param name="stateKind">�X�e�C�g�̎��</param>
	void AddState(std::function<void(void)> stateUpdate, std::function<void(void)> stateInit, State stateKind);

	// �X�e�C�g�̃Z�b�g
	void SetState(State state);

	// �X�e�C�g�I������
	void EndState();

	// ���݂̃X�e�C�g�擾
	State GetState() const { return m_pNowState.stateKind; }

private:
	// �X�e�C�g�f�[�^
	struct StateData {
		std::function<void(void)>stateUpdate{};	// �X�e�C�g�X�V����
		std::function<void(void)>stateInit{};	// �X�e�C�g����������
		State stateKind{};						// �X�e�C�g�̎��
	};

private:
	// �X�e�C�g�̕ύX
	void StateTransitionIdle();		// �ҋ@��Ԃɏ����������ΕύX
	void StateTransitionWalk();		// ������Ԃɏ����������ΕύX
	void StateTransitionJump();		// �W�����v��Ԃɏ����������ΕύX
	void StateTransitionAttack();	// �U����Ԃɏ����������ΕύX

	// �e��ԑJ�ڌĂяo��
	void StateTransition();

	// �X�e�C�g�̕ύX
	void ChangeState(State state);

	// �S�ẴX�e�C�g���i�[
	std::vector<StateData>	m_pState;

	// ���ݑI�΂�Ă���X�e�C�g
	StateData m_pNowState;

	// �A�N�V�����s�����Ƃ��Ă��邩
	bool m_isActionState;
};

