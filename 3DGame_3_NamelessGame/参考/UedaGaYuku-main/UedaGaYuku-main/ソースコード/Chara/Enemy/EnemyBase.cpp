#include "DxLib.h"
#include "Input.h"
#include "EffectManager.h"
#include "Sound.h"
#include "Player.h"
#include "Stage.h"
#include "SceneStageBase.h"
#include "EnemyBase.h"
#include "DebugDraw.h"

// 定数
namespace
{
	constexpr float kInitAngle = 180.0f;	// 開始時の敵の向く方向
	constexpr float kEffectHeight = 30.0f;	// エフェクトを表示する高さ
}


/// <summary>
/// コンストラクタ
/// </summary>
EnemyBase::EnemyBase() :
	m_enemyType(0),
	m_stopTime(0),
	m_angleIntervalTime(0),
	m_intervalTime(0),
	m_guardTime(0),
	m_eToPDirVec(VGet(0.0f, 0.0f, 0.0f)),
	m_shadowMap(-1)
{
	m_currentState = CharacterBase::State::kFightIdle;
	m_angle = kInitAngle; // 真正面を向くようにする
}


/// <summary>
/// デストラクタ
/// </summary>
EnemyBase::~EnemyBase()
{
}


/// <summary>
/// 更新
/// </summary>
/// <param name="player"></param>
/// <param name="stage"></param>
/// <param name="sceneStage"></param>
void EnemyBase::Update(Player& player, Stage& stage, SceneStageBase& sceneStage)
{
	// 移動パラメータを設定する
	VECTOR	upMoveVec;		// 上ボタンを入力をしたときのプレイヤーの移動方向ベクトル
	VECTOR	leftMoveVec;	// 左ボタンを入力をしたときのプレイヤーの移動方向ベクトル
	VECTOR	moveVec;		// このフレームの移動ベクトル

	// エネミーの状態を更新
	CharacterBase::State prevState = m_currentState;

	// 攻撃処理の更新
	m_attackTime--;

	// 敵の位置からプレイヤー位置までのベクトルを求める
	m_eToPDirVec = VSub(player.GetPos(), m_pos);

	// 状態を更新する
	m_currentState = UpdateState(player, sceneStage, upMoveVec, leftMoveVec, moveVec);

	// プレイヤーとの当たり判定をチェックする
	player.CheckHitEnemyCol(*this, VGet(m_pos.x, m_pos.y + m_colInfo.bodyHeight, m_pos.z), m_pos, m_colInfo.bodyRadius);

	UpdateAnimState(prevState);		// アニメーション状態を更新
	UpdateAngle();					// 角度を更新
	UpdateGuard();					// ガード状態を更新
	Move(moveVec, player, stage);	// 移動ベクトルを元にエネミーを移動させる
	UpdateAnim();					// アニメーション処理の更新
	UpdateCol();					// 当たり判定位置更新
	UpdatePosLog();					// 位置ログを更新
	m_pEffect->Update();			// エフェクト更新
}


/// <summary>
/// 描画
/// </summary>
void EnemyBase::Draw()
{
	MV1DrawModel(m_modelHandle);
	m_pEffect->Draw();					// エフェクト描画

	// 回避中は残像を表示する
	if (m_currentState == State::kAvoid)
	{
		DrawAfterImage();
	}

#ifdef _DEBUG
	DebugDraw debug;
	debug.DrawEnemyInfo(m_pos, m_hp, static_cast<int>(m_currentState), m_attackTime);
	// 当たり判定描画
	debug.DrawBodyCol(m_col.bodyTopPos, m_col.bodyBottomPos, m_colInfo.bodyRadius); // 全身
	debug.DrawAimCol(m_col.armStartPos, m_col.armEndPos, m_colInfo.aimRadius);		// 腕
	debug.DrawLegCol(m_col.legStartPos, m_col.legEndPos, m_colInfo.legRadius);		// 脚
#endif
}


/// <summary>
/// 敵の状態を更新する
/// </summary>
/// <param name="player">プレイヤー参照</param>
/// <param name="sceneStage">シーン情報参照</param>
/// <param name="upMoveVec">上方向への移動ベクトル</param>
/// <param name="leftMoveVec">左方向への移動ベクトル</param>
/// <param name="moveVec">移動ベクトル</param>
/// <returns>現在の状態</returns>
EnemyBase::CharacterBase::State EnemyBase::UpdateState(Player& player, SceneStageBase& sceneStage, VECTOR& upMoveVec, VECTOR& leftMoveVec, VECTOR& moveVec)
{
	CharacterBase::State nextState = m_currentState;

	// このフレームでの移動ベクトルを初期化
	moveVec = VGet(0.0f, 0.0f, 0.0f);

	// スタート演出時は待機状態にする
	if (sceneStage.GetBattleStartTime() > 0)
	{
		nextState = CharacterBase::State::kFightIdle;
		return nextState;
	}

	// プレイヤーが必殺技を発動時ガードを解除する
	if (player.GetIsSpecialAttack())
	{
		OffGuard();
	}

	// パンチできない場合
	if (m_punchCoolTime > 0)
	{
		m_punchCoolTime--;
		return m_currentState;
	}

	m_punchComboTime--; // コンボ入力の受付時間の更新

	// キックできない場合
	if (m_kickCoolTime > 0)
	{
		m_kickCoolTime--;
		return m_currentState;
	}

	// 特定の場合は状態を更新しない
	bool isKeepState =
		   m_isGuard
		|| m_isAttack
		|| (m_currentState == CharacterBase::State::kReceive)
		|| (m_currentState == CharacterBase::State::kDown)
		|| player.GetIsSpecialAttack()
		|| player.GetHp() <= 0;
	if (isKeepState) return nextState;

	// 敵のHPが0になった場合
	if (m_hp <= 0)
	{
		Down(); // ダウン処理を行う
		return m_currentState;
	}

	// エネミーとプレイヤーの距離を計算
	m_eToPDirVec = VSub(player.GetPos(), m_pos);
	float distance = VSize(m_eToPDirVec);
	if (m_intervalTime > 0)
	{
		m_intervalTime--;	// クールダウンタイマーを減少させる
		return nextState;
	}

	// プレイヤーが一定距離離れた場合
	if (distance > m_enemyInfo.approachRange)
	{
		// 数秒たったらプレイヤーに近づく
		if (m_stopTime < 0)
		{
			m_eToPDirVec = VNorm(m_eToPDirVec);
			moveVec = VScale(m_eToPDirVec, m_moveSpeed);
			nextState = CharacterBase::State::kRun; // 移動状態にする
		}
		else
		{
			m_stopTime--;
		}
	}
	// プレイヤーが攻撃範囲に入った場合
	else if (distance <= m_enemyInfo.attackRange)
	{
		// 確率で攻撃を行う
		int randNum = GetRand(m_enemyInfo.maxProb);

		// パンチ攻撃
		if (randNum <= m_enemyInfo.punchProb)
		{
			nextState = Punch();
		}
		// キック攻撃
		else if (randNum <= m_enemyInfo.kickProb + m_enemyInfo.punchProb)
		{
			nextState = kick();
		}
		// 回避
		else if (randNum <= m_enemyInfo.avoidProb + m_enemyInfo.kickProb + m_enemyInfo.punchProb)
		{
			nextState = Avoid();
		}
		// ガード
		else if (randNum <= m_enemyInfo.guardProb + m_enemyInfo.avoidProb + m_enemyInfo.kickProb + m_enemyInfo.punchProb)
		{
			nextState = Guard();
		}
		// 待機状態
		else
		{
			nextState = Fighting();
		}

		m_intervalTime = m_enemyInfo.stateIntervalTime;
	}
	else
	{
		m_stopTime = m_enemyInfo.minStopTime + GetRand(m_enemyInfo.maxStopTime);	// プレイヤーに近づくまでの時間をランダムで計算する
		nextState = CharacterBase::State::kFightIdle;								// 待機状態にする
	}

	return nextState;
}


/// <summary>
/// 移動処理
/// </summary>
/// <param name="MoveVec">移動量</param>
/// <param name="stage">ステージ情報参照</param>
void EnemyBase::Move(const VECTOR& moveVec, Player& player, Stage& stage)
{
	if (fabs(moveVec.x) > 0.0f || fabs(moveVec.z) > 0.0f)
	{
		m_isMove = true;
	}
	else
	{
		m_isMove = false;
	}

	// ステージと当たり判定を行って座標を保存する
	m_pos = stage.CheckEnemyCol(*this, moveVec);

	// プレイヤーの座標を更新する
	MV1SetPosition(m_modelHandle, m_pos);
}


/// <summary>
/// パンチ攻撃
/// </summary>
/// <returns>現在の状態</returns>
CharacterBase::State EnemyBase::Punch()
{
	CharacterBase::State nextState = m_currentState;
	// コンボ入力受付時間内にボタンが押された場合
	if (m_punchComboTime > 0)
	{
		m_punchCount++;
	}
	else
	{
		m_punchCount = 0;
	}

	// コンボ数によってパンチを変える
	switch (m_punchCount)
	{
	case 0:
		m_punchComboTime = m_status.punchReceptionTime; // コンボ入力の受付時間をリセット
		m_isAttack = true;
		m_isFighting = false;
		PlayAnim(AnimKind::kPunch1);
		nextState = CharacterBase::State::kPunch1;
		break;
	case 1:
		m_punchComboTime = m_status.punchReceptionTime;
		m_isAttack = true;
		m_isFighting = false;
		PlayAnim(AnimKind::kPunch2);
		nextState = CharacterBase::State::kPunch2;
		break;
	case 2:
		m_punchComboTime = m_status.punchReceptionTime;
		m_isAttack = true;
		m_isFighting = false;
		PlayAnim(AnimKind::kPunch3);
		nextState = CharacterBase::State::kPunch3;
		break;
	case 3:
		m_isAttack = false;
		m_punchCount = 0;
		m_punchCoolTime = m_status.punchCoolTime;	// クールダウンタイムを設定
		nextState = CharacterBase::State::kFightIdle;
		break;
	}

	return nextState;
}


/// <summary>
/// キック攻撃
/// </summary>
/// <returns>現在の状態</returns>
CharacterBase::State EnemyBase::kick()
{
	// キック攻撃
	m_isAttack = true;
	m_isFighting = false;
	m_kickCoolTime = m_status.kickCoolTime;	// クールダウンタイムを設定
	PlayAnim(AnimKind::kKick);
	return CharacterBase::State::kKick;
}


/// <summary>
/// 回避処理
/// </summary>
/// <param name="moveVec">移動ベクトル</param>
CharacterBase::State EnemyBase::Avoid()
{
	// 回避できない場合
	if (m_avoidCoolTime > 0)
	{
		m_avoidCoolTime--;
		return m_currentState;
	}

	PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kAvoid)], DX_PLAYTYPE_BACK); // SE再生
	m_isFighting = false;
	m_avoidCount++;

	// 回避数が最大になった場合
	if (m_avoidCount > m_status.maxAvoidCount)
	{
		m_avoidCount = 0;
		m_avoidCoolTime = m_status.avoidCoolTime;	// クールダウンタイムを設定
	}
	else
	{
		// 向いている方向と逆方向に移動する
		VECTOR backMoveVec = VScale(VNorm(m_eToPDirVec), -1.0f * m_status.avoidDist);
		m_pos = VAdd(m_pos,backMoveVec);
		return CharacterBase::State::kAvoid;
	}

	return m_currentState;
}


/// <summary>
/// 構え処理
/// </summary>
CharacterBase::State  EnemyBase::Fighting()
{
	m_isFighting = true;
	PlayAnim(AnimKind::kFightWalk);
	return CharacterBase::State::kFightWalk;
}


/// <summary>
/// ガード処理
/// </summary>
CharacterBase::State EnemyBase::Guard()
{
	if (m_isGuard)
	{
		return CharacterBase::State::kGuard;
	}

	m_isGuard = true;
	// ガードの時間を設定
	m_guardTime = m_enemyInfo.guardTime;
	return CharacterBase::State::kGuard;
}


/// <summary>
/// ガード状態を更新
/// </summary>
void EnemyBase::UpdateGuard()
{
	if (!m_isGuard) return;

	if (m_guardTime > 0)
	{
		m_guardTime--;
	}
	else
	{
		OffGuard(); // ガード状態を解除する
	}
}


/// <summary>
/// ガード状態を解除
/// </summary>
CharacterBase::State EnemyBase::OffGuard()
{
	m_isGuard = false;
	m_guardTime = 0;
	return CharacterBase::State::kFightIdle;
}


/// <summary>
/// 攻撃を受けている最中の処理
/// </summary>
/// <returns></returns>
void EnemyBase::Receive()
{ 
	m_isAttack = false; // 攻撃状態を解除
	m_currentState = CharacterBase::State::kReceive;

	if (m_attackTime > 0) return;
	PlayAnim(CharacterBase::AnimKind::kReceive);
	m_pEffect->PlayDamageEffect(VGet(m_pos.x, m_pos.y + kEffectHeight, m_pos.z));					// 攻撃エフェクト再生
	PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kAttack)], DX_PLAYTYPE_BACK); 	// 攻撃SE再生
}


/// <summary>
/// ダウン処理
/// </summary>
void EnemyBase::Down()
{
	m_isAttack = false;
	m_isFighting = false;
	m_isGuard = false;
	m_currentState = CharacterBase::State::kDown;
	PlayAnim(CharacterBase::AnimKind::kDown);
}


/// <summary>
/// 敵の角度を更新
/// </summary>
void EnemyBase::UpdateAngle()
{
	m_angleIntervalTime++;

	// 移動中は常にプレイヤーの方向を向くようにする
	if(m_currentState == CharacterBase::State::kRun || m_currentState == CharacterBase::State::kAvoid)
	{
		m_angle = atan2f(m_eToPDirVec.x, m_eToPDirVec.z);
	}
	// 一定時間経過したら
	else if (m_angleIntervalTime >= m_enemyInfo.changeAngleFrame)
	{
		// ランダムでプレイヤーの方向を向く
		int randNum = GetRand(m_enemyInfo.maxProb);
		if (randNum <= m_enemyInfo.changeAngleProb)
		{
			m_angle = atan2f(m_eToPDirVec.x, m_eToPDirVec.z);
		}
		m_angleIntervalTime = 0;
	}

	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, m_angle + DX_PI_F, 0.0f));
}


/// <summary>
/// 攻撃を受けた際の処理
/// </summary>
/// <param name="damage">ダメージ量</param>
void EnemyBase::OnDamage(float damage)
{
	CharacterBase::OnDamage(damage);

	// ガード状態の場合
	if (m_currentState == CharacterBase::State::kGuard)
	{
		// 少し後ろに移動する
		VECTOR backMoveVec = VScale(m_eToPDirVec, -1.0f);
		m_pos = VAdd(m_pos, VScale(backMoveVec, m_status.backMove));
		m_pEffect->PlayGuardEffect(VGet(m_pos.x, m_pos.y + kEffectHeight, m_pos.z)); // エフェクト表示
		if (!CheckSoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kGuardAttack)]))
		{
			PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kGuardAttack)], DX_PLAYTYPE_BACK);	// ガード時攻撃SE再生
		}
	}
	else
	{
		Receive(); // 攻撃を受けた状態にする
	}
}


/// <summary>
/// プレイヤーとの当たり判定をチェックする
/// </summary>
/// <param name="player">プレイヤー参照</param>
/// <param name="eCapPosTop">敵の当たり判定カプセルの頂点位置</param>
/// <param name="eCapPosBottom">敵の当たり判定カプセルの頂点位置</param>
/// <param name="eCapRadius">敵の当たり判定カプセルの半径</param>
void EnemyBase::CheckHitPlayerCol(Player& player, VECTOR eCapPosTop, VECTOR eCapPosBottom, float eCapRadius)
{
	// プレイヤーと敵の当たり判定を行う
	bool isHit = HitCheck_Capsule_Capsule(m_col.bodyTopPos, m_col.bodyBottomPos, m_colInfo.bodyRadius, eCapPosTop, eCapPosBottom, eCapRadius);
	// パンチ
	bool isHitPunch = HitCheck_Capsule_Capsule(m_col.armStartPos, m_col.armEndPos, m_colInfo.aimRadius, eCapPosTop, eCapPosBottom, eCapRadius);
	// キック
	bool isHitKick = HitCheck_Capsule_Capsule(m_col.legStartPos, m_col.legEndPos, m_colInfo.legRadius, eCapPosTop, eCapPosBottom, eCapRadius);

	// 背後から攻撃したかどうか
	m_eToPDirVec = VNorm(m_eToPDirVec);
	bool isBackAttack = VDot(player.GetDir(), m_eToPDirVec) < 0.0f;

	// パンチ状態かどうか
	bool isStatePunch = m_currentState == CharacterBase::State::kPunch1 || m_currentState == CharacterBase::State::kPunch2 || m_currentState == CharacterBase::State::kPunch3;
	// パンチが当たった場合
	if (isHitPunch && isStatePunch)
	{
		if (m_attackTime > 0) return;

		// プレイヤーがガードしていないか、背後から攻撃した場合
		if (!player.GetIsGuard())
		{
			// 1コンボ目
			if (m_currentState == CharacterBase::State::kPunch1)
			{
				player.OnDamage(m_status.punchPower);
				m_attackTime = m_status.punchTime;
			}
			// 2コンボ目
			if (m_currentState == CharacterBase::State::kPunch2)
			{
				player.OnDamage(m_status.secondPunchPower);
				m_attackTime = m_status.punchTime;
			}
			// 3コンボ目
			if (m_currentState == CharacterBase::State::kPunch3)
			{
				player.OnDamage(m_status.thirdPunchPower);
				m_attackTime = m_status.punchTime;
			}
		}
		else
		{
			player.OnDamage(0.0f);
		}
	}
	// キックが当たった場合
	else if (isHitKick && m_currentState == CharacterBase::State::kKick)
	{
		if (m_attackTime > 0) return;

		// キックが当たった場合
		if (!player.GetIsGuard())
		{
			player.OnDamage(m_status.kickPower);
			m_attackTime = m_status.kickTime;
		}
		else
		{
			player.OnDamage(0.0f);
		}
	}

	// 掴みが決まらなかった場合
	if (!isHit && m_currentState == CharacterBase::State::kGrab)
	{
		// 掴み失敗のアニメーションを再生する
		PlayAnim(CharacterBase::AnimKind::kStumble);
	}
}


#ifdef _DEBUG
/// <summary>
/// デバッグ用
/// </summary>
void EnemyBase::DebugDamage(Input& input)
{
	if (input.IsTriggered("debug_damage"))
	{
		m_hp = 0;
	}
}
#endif
