#pragma once
#include <vector>
#include <functional>

class PlayerState
{
public:
	// �v���C���[�̏��
	enum class StateKind
	{
		kIdle = 1,	// �ҋ@
		kWalk = 2,	// �ړ�
		kJump = 3,	// �W�����v
		kAttack = 4,	// �U��
	};

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	PlayerState();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~PlayerState();

	void Update();

	/// <summary>
	/// �X�e�C�g�ǉ�
	/// </summary>
	/// <param name="stateUpdate"></param>
	/// <param name="stateInit"></param>
	/// <param name="stateKind"></param>
	void AddState(std::function<void(void)> stateUpdate, std::function<void(void)> stateInit, StateKind stateKind);

	//�X�e�C�g�Z�b�g
	void SetState(StateKind state);

	//�X�e�C�g�I������
	void EndState();

	//���݂̃X�e�C�g�擾
	StateKind GetState() const { return m_pNowState.stateKind; }

private:
	struct StateData
	{
		std::function<void(void)> stateUpdate{};	//�X�e�C�g�X�V����
		std::function<void(void)> stateInit{};	//�X�e�C�g����������
		StateKind stateKind{};					//�X�e�C�g�̎��
	};

private:
	void StateTransIdle();			//�ҋ@��Ԃɏ����������ΕύX
	void StateTransWalk();			//������Ԃɏ����������ΕύX
	void StateTransJump();			//�W�����v��Ԃɏ����������ΕύX
	void StateTransAttack();		//�U����Ԃɏ����������ΕύX

	void StateTransition();				//�e��ԑJ�ڌĂяo��

	void ChangeState(StateKind state);	//�X�e�C�g�ύX

	std::vector<StateData> m_pState;	//�S���̃X�e�C�g���i�[

	StateData m_pNowState;				//���ݑI�΂�Ă���X�e�C�g

	bool m_isActionState = false;		//�A�N�V�����s�����s���Ă��邩
};