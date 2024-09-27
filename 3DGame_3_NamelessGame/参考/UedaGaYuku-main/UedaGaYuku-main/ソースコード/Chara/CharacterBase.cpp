#include "DxLib.h"
#include "LoadData.h"
#include "UIBattle.h"
#include "EffectManager.h"
#include "CharacterBase.h"

// 定数
namespace
{
	// アニメーション情報
	constexpr float kAnimBlendMax = 1.0f;	 // アニメーションブレンドの最大値
	constexpr float kAnimBlendSpeed = 0.2f;	 // アニメーションブレンドの変化速度
	constexpr int kPosLogNum = 8;			 // 覚えておく過去の位置情報の数
	constexpr float kAlphaAdj = 0.3f;		 // 残像の透明度を調整
}

/// <summary>
/// コンストラクタ
/// </summary>
CharacterBase::CharacterBase():
	m_animSpeed{0.0f},
	m_status{0.0f},
	m_colInfo{0.0f},
	m_hp(0.0f),
	m_pos(VGet(0.0f, 0.0f, 0.0f)),
	m_moveSpeed(0.0f),
	m_angle(0.0f),
	m_punchCount(0),
	m_punchComboTime(0),
	m_punchCoolTime(0),
	m_kickCoolTime(0),
	m_attackTime(0),
	m_avoidCoolTime(0),
	m_avoidCount(0),
	m_isAttack(false),
	m_isGuard(false),
	m_isReceive(false),
	m_isSpecialAttack(false),
	m_isStartProduction(false),
	m_isClearProduction(false),
	m_isGameoverProduction(false),
	m_modelHandle(-1),
	m_currentPlayAnim(-1),
	m_currentAnimCount(0.0f),
	m_prevPlayAnim(-1),
	m_prevAnimCount(0.0f),
	m_animBlendRate(0.0f)
{
	m_posLog.resize(kPosLogNum);
}


/// <summary>
/// ダメージを受けた際の処理
/// </summary>
/// <param name="damage">ダメージ量</param>
void CharacterBase::OnDamage(float damage)
{
	m_hp -= damage;
	m_hp = std::max(0.0f, m_hp);

	// HPバーを更新
	if (damage > 0.0f)
	{
		m_pUIBattle->OnDamage(damage);
	}
}


/// <summary>
/// 当たり判定位置更新
/// </summary>
void CharacterBase::UpdateCol()
{
	// プレイヤーの向きをもとに当たり判定の位置を調整する
	MATRIX rotationMatrix = MGetRotY(m_angle);

	// プレイヤー全体の当たり判定位置を更新
	m_col.bodyTopPos = VAdd(m_pos, (VTransform(VGet(0.0f, m_colInfo.bodyHeight, 0.0f), rotationMatrix)));
	m_col.bodyBottomPos = m_pos;

	// 腕の当たり判定位置を更新
	m_col.armStartPos = VAdd(m_pos, (VTransform(m_colInfo.armStartPos, rotationMatrix)));
	m_col.armEndPos = VAdd(m_col.armStartPos, (VTransform(m_colInfo.armEndPos, rotationMatrix)));

	// 脚の当たり判定位置を更新
	m_col.legStartPos = VAdd(m_pos, (VTransform(m_colInfo.legStartPos, rotationMatrix)));
	m_col.legEndPos = VAdd(m_col.legStartPos, (VTransform(m_colInfo.legEndPos, rotationMatrix)));
}


/// <summary>
/// アニメーションステートを更新
/// </summary>
/// <param name="prevState">1つ前のアニメーション</param>
void CharacterBase::UpdateAnimState(CharacterBase::State prevState)
{
	// 待機状態から
	if (prevState == CharacterBase::State::kFightIdle)
	{
		// 移動アニメーションを再生
		if (m_currentState == CharacterBase::State::kRun) PlayAnim(AnimKind::kRun);
		// パンチアニメーションを再生
		if (m_currentState == CharacterBase::State::kPunch1)PlayAnim(AnimKind::kPunch1);
		// キックアニメーションを再生
		if (m_currentState == CharacterBase::State::kKick)	PlayAnim(AnimKind::kKick);
		// 回避アニメーションを再生
		if (m_currentState == CharacterBase::State::kAvoid) PlayAnim(AnimKind::kAvoid);
		// 構えアニメーションを再生
		if (m_currentState == CharacterBase::State::kFightWalk) PlayAnim(AnimKind::kFightWalk);
		// ガードアニメーションを再生
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kGuard);
		// ダメージアニメーションを再生
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kReceive);
	}
	// 移動状態から
	else if (prevState == CharacterBase::State::kRun)
	{
		// 待機アニメーションを再生
		if (m_currentState == CharacterBase::State::kFightIdle) PlayAnim(AnimKind::kFightIdle);
		// パンチアニメーションを再生
		if (m_currentState == CharacterBase::State::kPunch1) PlayAnim(AnimKind::kPunch1);
		// キックアニメーションを再生
		if (m_currentState == CharacterBase::State::kKick) PlayAnim(AnimKind::kKick);
		// 回避アニメーションを再生
		if (m_currentState == CharacterBase::State::kAvoid) PlayAnim(AnimKind::kAvoid);
		// 構えアニメーションを再生
		if (m_currentState == CharacterBase::State::kFightWalk) PlayAnim(AnimKind::kFightWalk);
		// ガードアニメーションを再生
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kGuard);
		// ダメージアニメーションを再生
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kReceive);
	}
	// パンチ状態(1コンボ目)から
	else if (prevState == CharacterBase::State::kPunch1)
	{
		// 待機アニメーションを再生
		if (m_currentState == CharacterBase::State::kFightIdle) PlayAnim(AnimKind::kFightIdle);
		// 移動アニメーションを再生
		if (m_currentState == CharacterBase::State::kRun) PlayAnim(AnimKind::kRun);
		// 2コンボ目アニメーションを再生
		if (m_currentState == CharacterBase::State::kPunch2) PlayAnim(AnimKind::kPunch2);
		// キックアニメーションを再生
		if (m_currentState == CharacterBase::State::kKick) PlayAnim(AnimKind::kKick);
		// 回避アニメーションを再生
		if (m_currentState == CharacterBase::State::kAvoid) PlayAnim(AnimKind::kAvoid);
		// 構えアニメーションを再生
		if (m_currentState == CharacterBase::State::kFightWalk) PlayAnim(AnimKind::kFightWalk);
		// ガードアニメーションを再生
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kGuard);
		// ダメージアニメーションを再生
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kReceive);
	}
	// パンチ状態(2コンボ目)から
	else if (prevState == CharacterBase::State::kPunch2)
	{
		// 待機アニメーションを再生
		if (m_currentState == CharacterBase::State::kFightIdle) PlayAnim(AnimKind::kFightIdle);
		// 移動アニメーションを再生
		if (m_currentState == CharacterBase::State::kRun) PlayAnim(AnimKind::kRun);
		// 3コンボ目アニメーションを再生
		if (m_currentState == CharacterBase::State::kPunch3) PlayAnim(AnimKind::kPunch3);
		// キックアニメーションを再生
		if (m_currentState == CharacterBase::State::kKick) PlayAnim(AnimKind::kKick);
		// 回避アニメーションを再生
		if (m_currentState == CharacterBase::State::kAvoid) PlayAnim(AnimKind::kAvoid);
		// 構えアニメーションを再生
		if (m_currentState == CharacterBase::State::kFightWalk) PlayAnim(AnimKind::kFightWalk);
		// ガードアニメーションを再生
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kGuard);
		// ダメージアニメーションを再生
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kReceive);
	}
	// パンチ状態(3コンボ目)から
	else if (prevState == CharacterBase::State::kPunch3)
	{
		// 待機アニメーションを再生
		if (m_currentState == CharacterBase::State::kFightIdle) PlayAnim(AnimKind::kFightIdle);
		// 移動アニメーションを再生
		if (m_currentState == CharacterBase::State::kRun) PlayAnim(AnimKind::kRun);
		// 1コンボ目アニメーションを再生
		if (m_currentState == CharacterBase::State::kPunch1) PlayAnim(AnimKind::kPunch1);
		// キックアニメーションを再生
		if (m_currentState == CharacterBase::State::kKick) PlayAnim(AnimKind::kKick);
		// 回避アニメーションを再生
		if (m_currentState == CharacterBase::State::kAvoid) PlayAnim(AnimKind::kAvoid);
		// 構えアニメーションを再生
		if (m_currentState == CharacterBase::State::kFightWalk) PlayAnim(AnimKind::kFightWalk);
		// ガードアニメーションを再生
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kGuard);
		// ダメージアニメーションを再生
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kReceive);
	}
	// キック状態から
	else if (prevState == CharacterBase::State::kKick)
	{
		// 待機アニメーションを再生
		if (m_currentState == CharacterBase::State::kFightIdle) PlayAnim(AnimKind::kFightIdle);
		// 移動アニメーションを再生
		if (m_currentState == CharacterBase::State::kRun) PlayAnim(AnimKind::kRun);
		// パンチアニメーションを再生
		if (m_currentState == CharacterBase::State::kPunch1) PlayAnim(AnimKind::kPunch1);
		// 回避アニメーションを再生
		if (m_currentState == CharacterBase::State::kAvoid) PlayAnim(AnimKind::kAvoid);
		// 構えアニメーションを再生
		if (m_currentState == CharacterBase::State::kFightWalk) PlayAnim(AnimKind::kFightWalk);
		// ガードアニメーションを再生
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kGuard);
		// ダメージアニメーションを再生
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kReceive);
	}
	// 回避状態から
	else if (prevState == CharacterBase::State::kAvoid)
	{
		// 待機アニメーションを再生
		if (m_currentState == CharacterBase::State::kFightIdle) PlayAnim(AnimKind::kFightIdle);
		// 移動アニメーションを再生
		if (m_currentState == CharacterBase::State::kRun) PlayAnim(AnimKind::kRun);
		// パンチアニメーションを再生
		if (m_currentState == CharacterBase::State::kPunch1) PlayAnim(AnimKind::kPunch1);
		// キックアニメーションを再生
		if (m_currentState == CharacterBase::State::kKick) PlayAnim(AnimKind::kKick);
		// 構えアニメーションを再生
		if (m_currentState == CharacterBase::State::kFightWalk) PlayAnim(AnimKind::kFightWalk);
		// ガードアニメーションを再生
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kGuard);
		// ダメージアニメーションを再生
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kReceive);
	}
	// 構え状態から
	else if (prevState == CharacterBase::State::kFightWalk)
	{
		// 待機アニメーションを再生
		if (m_currentState == CharacterBase::State::kFightIdle) PlayAnim(AnimKind::kFightIdle);
		// 移動アニメーションを再生
		if (m_currentState == CharacterBase::State::kRun) PlayAnim(AnimKind::kRun);
		// パンチアニメーションを再生
		if (m_currentState == CharacterBase::State::kPunch1) PlayAnim(AnimKind::kPunch1);
		// キックアニメーションを再生
		if (m_currentState == CharacterBase::State::kKick) PlayAnim(AnimKind::kKick);
		// 回避アニメーションを再生
		if (m_currentState == CharacterBase::State::kAvoid) PlayAnim(AnimKind::kAvoid);
		// ガードアニメーションを再生
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kGuard);
		// ダメージアニメーションを再生
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kReceive);
	}
	// ガード状態から
	else if (prevState == CharacterBase::State::kGuard)
	{
		// 待機アニメーションを再生
		if (m_currentState == CharacterBase::State::kFightIdle) PlayAnim(AnimKind::kFightIdle);
		// 移動アニメーションを再生
		if (m_currentState == CharacterBase::State::kRun) PlayAnim(AnimKind::kRun);
		// パンチアニメーションを再生
		if (m_currentState == CharacterBase::State::kPunch1) PlayAnim(AnimKind::kPunch1);
		// キックアニメーションを再生
		if (m_currentState == CharacterBase::State::kKick) PlayAnim(AnimKind::kKick);
		// 回避アニメーションを再生
		if (m_currentState == CharacterBase::State::kAvoid) PlayAnim(AnimKind::kAvoid);
		// 構えアニメーションを再生
		if (m_currentState == CharacterBase::State::kFightWalk) PlayAnim(AnimKind::kFightWalk);
	}
	// 攻撃を受けた状態から
	else if (prevState == CharacterBase::State::kReceive)
	{
		// 待機アニメーションを再生
		if (m_currentState == CharacterBase::State::kFightIdle) PlayAnim(AnimKind::kFightIdle);
		// 移動アニメーションを再生
		if (m_currentState == CharacterBase::State::kRun) PlayAnim(AnimKind::kRun);
		// パンチアニメーションを再生
		if (m_currentState == CharacterBase::State::kPunch1) PlayAnim(AnimKind::kPunch1);
		// キックアニメーションを再生
		if (m_currentState == CharacterBase::State::kKick) PlayAnim(AnimKind::kKick);
		// 回避アニメーションを再生
		if (m_currentState == CharacterBase::State::kAvoid) PlayAnim(AnimKind::kAvoid);
		// 構えアニメーションを再生
		if (m_currentState == CharacterBase::State::kFightWalk) PlayAnim(AnimKind::kFightWalk);
		// ガードアニメーションを再生
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kGuard);
	}
}


/// <summary>
/// アニメーションを再生する
/// </summary>
/// <param name="PlayAnimIndex">再生するアニメーション状態</param>
void CharacterBase::PlayAnim(AnimKind playAnimIndex)
{
	// 1つ前のアニメーションがアタッチされている場合削除する
	if (m_prevPlayAnim != -1)
	{
		MV1DetachAnim(m_modelHandle, m_prevPlayAnim);
		m_prevPlayAnim = -1;
	}

	// 現在再生中のアニメーションを1つ前に移動する
	m_prevPlayAnim = m_currentPlayAnim;
	m_prevAnimCount = m_currentAnimCount;

	// 新たにアニメーションをアタッチする
	m_currentPlayAnim = MV1AttachAnim(m_modelHandle, static_cast<int>(playAnimIndex), -1, false);
	m_currentAnimCount = 0.0f;

	// ブレンド率はPrevが有効でない場合、1.0にする
	if (m_prevPlayAnim == -1)
	{
		m_animBlendRate = kAnimBlendMax;
	}
	else
	{
		m_animBlendRate = 0.0f;
	}
}


/// <summary>
/// アニメーションの処理を行う
/// </summary>
void CharacterBase::UpdateAnim()
{
	float animTotalTime; // 再生中のアニメーション時間

	// ブレンド率が1以下の場合
	if (m_animBlendRate < kAnimBlendMax)
	{
		m_animBlendRate += kAnimBlendSpeed;
		m_animBlendRate = std::min(m_animBlendRate, kAnimBlendMax);
	}

	// 現在再生中のアニメーションの処理
	if (m_currentPlayAnim != -1)
	{
		// アニメーションの総時間を取得する
		animTotalTime = MV1GetAttachAnimTotalTime(m_modelHandle, m_currentPlayAnim);

		// アニメーションによって再生スピードを変える
		if (m_currentState == CharacterBase::State::kPunch1)
		{
			m_currentAnimCount += m_animSpeed.punch1;
		}
		else if (m_currentState == CharacterBase::State::kPunch2)
		{
			m_currentAnimCount += m_animSpeed.punch2;
		}
		else if (m_currentState == CharacterBase::State::kPunch3)
		{
			m_currentAnimCount += m_animSpeed.punch3;
		}
		else if (m_currentState == CharacterBase::State::kKick)
		{
			m_currentAnimCount += m_animSpeed.kick;
		}
		else if (m_currentState == CharacterBase::State::kSpecialAttack)
		{
			m_currentAnimCount += m_animSpeed.kick;
		}
		else if (m_currentState == CharacterBase::State::kAvoid)
		{
			m_currentAnimCount += m_animSpeed.specialAttack;
		}
		else if (m_currentState == CharacterBase::State::kFightWalk)
		{
			// 移動時のみアニメーションを再生
			if (m_isMove)
			{
				m_currentAnimCount += m_animSpeed.fightWalk;
			}
			else
			{
				m_currentAnimCount = 0.0f;
			}
		}
		else if (m_currentState == CharacterBase::State::kGuard)
		{
			m_currentAnimCount += m_animSpeed.guard;
			if (m_isGuard)
			{
				m_currentAnimCount = std::min(m_currentAnimCount, m_status.guardAnimTime);
			}
		}
		else if (m_currentState == CharacterBase::State::kReceive)
		{
			m_currentAnimCount += m_animSpeed.receive;
		}
		else if (m_currentState == CharacterBase::State::kDown)
		{
			m_currentAnimCount += m_animSpeed.down;
			// アニメーションを一時停止する
			if (m_currentAnimCount > animTotalTime)
			{
				m_currentAnimCount = animTotalTime;
				MV1SetAttachAnimTime(m_modelHandle, m_currentPlayAnim, m_currentAnimCount);
				MV1SetAttachAnimBlendRate(m_modelHandle, m_currentPlayAnim, m_animBlendRate);
				return;
			}
		}
		else
		{
			m_currentAnimCount += m_animSpeed.fightIdle;
		}

		// アニメーションが終わった場合
		if (m_currentAnimCount > animTotalTime)
		{
			// 特定のアニメーションが終わったら待機状態に移行
			bool isAnimChange = m_currentState == CharacterBase::State::kAvoid || m_currentState == CharacterBase::State::kGuard || m_currentState == CharacterBase::State::kReceive;

			// 攻撃アニメーションが終了したら待機状態に移行
			if (m_isAttack || m_isSpecialAttack)
			{
				m_isAttack = false;
				m_isSpecialAttack = false;
				m_isReceive = false;
				m_currentState = CharacterBase::State::kFightIdle;
				PlayAnim(AnimKind::kFightIdle);
			}
			else if (isAnimChange)
			{
				m_isReceive = false;
				m_currentState = CharacterBase::State::kFightIdle;
				PlayAnim(AnimKind::kFightIdle);
			}
			else
			{
				// アニメーションの再生時間をループ
				m_currentAnimCount = static_cast<float>(fmod(m_currentAnimCount, animTotalTime));
			}
		}

		// 再生時間を更新
		MV1SetAttachAnimTime(m_modelHandle, m_currentPlayAnim, m_currentAnimCount);
		// アニメーションのブレンド率を設定する
		MV1SetAttachAnimBlendRate(m_modelHandle, m_currentPlayAnim, m_animBlendRate);
	}

	// 1つ前に再生していたアニメーションの処理
	if (m_prevPlayAnim != -1)
	{
		// アニメーションの総時間を取得する
		animTotalTime = MV1GetAttachAnimTotalTime(m_modelHandle, m_prevPlayAnim);

		// アニメーションによって再生速度を変える
		if (m_prevPlayAnim == static_cast<int>(CharacterBase::AnimKind::kPunch1))
		{
			m_prevAnimCount += m_animSpeed.punch1;
		}
		else if (m_prevPlayAnim == static_cast<int>(CharacterBase::AnimKind::kPunch2))
		{
			m_prevAnimCount += m_animSpeed.punch2;
		}
		else if (m_prevPlayAnim == static_cast<int>(CharacterBase::AnimKind::kPunch3))
		{
			m_prevAnimCount += m_animSpeed.punch3;
		}
		else if (m_prevPlayAnim == static_cast<int>(CharacterBase::AnimKind::kKick))
		{
			m_prevAnimCount += m_animSpeed.kick;
		}
		else if (m_currentState == CharacterBase::State::kSpecialAttack)
		{
			m_prevAnimCount += m_animSpeed.specialAttack;
		}
		else if (m_prevPlayAnim == static_cast<int>(CharacterBase::AnimKind::kAvoid))
		{
			m_prevAnimCount += m_animSpeed.avoid;
		}
		else if (m_currentState == CharacterBase::State::kFightWalk)
		{
			// 移動時のみアニメーションを再生
			if (m_isMove)
			{
				m_prevAnimCount += m_animSpeed.fightWalk;
			}
			else
			{
				m_prevAnimCount = 0.0f;
			}
		}
		else if (m_currentState == CharacterBase::State::kGuard)
		{
			m_prevAnimCount += m_animSpeed.guard;
			if (m_isGuard)
			{
				m_prevAnimCount = std::min(m_currentAnimCount, m_status.guardAnimTime);
			}
		}
		else if (m_currentState == CharacterBase::State::kReceive)
		{
			m_prevAnimCount += m_animSpeed.receive;
		}
		else if (m_currentState == CharacterBase::State::kDown)
		{
			m_prevAnimCount += m_animSpeed.down;
		}
		else
		{
			m_prevAnimCount += m_animSpeed.fightIdle;
		}

		// アニメーションの再生時間をループ
		if (m_prevPlayAnim > animTotalTime)
		{
			m_prevAnimCount = static_cast<float>(fmod(m_prevAnimCount, animTotalTime));
		}

		// 再生時間を更新
		MV1SetAttachAnimTime(m_modelHandle, m_prevPlayAnim, m_prevAnimCount);
		// アニメーションのブレンド率を設定する
		MV1SetAttachAnimBlendRate(m_modelHandle, m_prevPlayAnim, kAnimBlendMax - m_animBlendRate);
	}
}



/// <summary>
/// 位置ログを更新する
/// </summary>
void CharacterBase::UpdatePosLog()
{
	// 位置ログをずらす
	for (int i = static_cast<int>(m_posLog.size()) - 1; i >= 1; i--)
	{
		m_posLog[i] = m_posLog[i - 1];
	}
	// 1フレーム前の位置情報を入れる
	m_posLog[0] = m_pos;
}


/// <summary>
/// 回避の際に残像を表示する
/// </summary>
void CharacterBase::DrawAfterImage()
{
	for (int i = static_cast<int>(m_posLog.size()) - 1; i >= 0; i--)
	{
		// ログが古くなるほど透明になるようにする
		float alpha = (1.0f - (static_cast<float>(i) / m_posLog.size())) * kAlphaAdj;
		MV1SetOpacityRate(m_modelHandle, alpha);
		MV1SetPosition(m_modelHandle, m_posLog[i]);
		MV1DrawModel(m_modelHandle);
	}
	MV1SetOpacityRate(m_modelHandle, 1.0f); // 透明度を戻す
}
