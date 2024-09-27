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
	/// ステートの追加
	/// </summary>
	/// <param name="state">ステート</param>
	/// <param name="update">updateステート</param>
	/// <param name="exit">exitステート</param>
	void AddState(TemplateState state, UpdateFunction update, UpdateFunction exit);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="state"></param>
	void SetExitState(TemplateState state);

	/// <summary>
	/// ステートの更新
	/// </summary>
	void Update();

	//ステートの取得
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
