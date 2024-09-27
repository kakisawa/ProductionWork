#pragma once
#include <functional>
#include <unordered_map>

template <class TemplateState> class StatePattern
{
public:
	typedef std::function<void(void)> UpdateFunction;
private:
	class StateUpdateFuncSet {
	public :
		TemplateState state_;
		UpdateFunction update_, exit_;

		StateUpdateFuncSet() : state_(TemplateState()){}
	};

	TemplateState currentState_;
	std::unordered_map<TemplateState, StateUpdateFuncSet> stateFuncTable_;
	bool isInitialized = false;

public:
	/// <summary>
	/// �X�e�[�g�̒ǉ�
	/// </summary>
	/// <param name="state">�X�e�[�g</param>
	/// <param name="update">update�X�e�[�g</param>
	/// <param name="exit">exit�X�e�[�g</param>
	void AddState(TemplateState state, UpdateFunction update, UpdateFunction exit);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="state"></param>
	void SetExitState(TemplateState state);

	/// <summary>
	/// �X�e�[�g�̍X�V
	/// </summary>
	void Update();

	//�X�e�[�g�̎擾
	TemplateState GetCurrentState() const { return currentState_; }

};

template<class TemplateState>
inline void StatePattern<TemplateState>::AddState(TemplateState state, UpdateFunction update, UpdateFunction exit)
{
	StateUpdateFuncSet set;
	set.state_ = state;
	set.update_ = update;
	set.exit_ = exit;
	stateFuncTable_.emplace(state, set);
}

template<class TemplateState>
inline void StatePattern<TemplateState>::SetExitState(TemplateState state)
{
	stateFuncTable_[currentState].exit_();
	currentState_ = state;
}

template<class TemplateState>
inline void StatePattern<TemplateState>::Update()
{
	stateFuncTable_[currentState_].update_();
}
