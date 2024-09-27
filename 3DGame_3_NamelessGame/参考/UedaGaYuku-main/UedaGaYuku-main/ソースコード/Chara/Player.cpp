#include "DxLib.h"
#include "Input.h"
#include "Sound.h"
#include "UIBattle.h"
#include "EffectManager.h"
#include "LoadData.h"
#include "Camera.h"
#include "Stage.h"
#include "EnemyBase.h"
#include "DebugDraw.h"
#include "Player.h"

// 定数
namespace
{
	/*プレイヤー情報*/
	constexpr float kMaxGauge = 100.0f;				// 最大ゲージ量
	constexpr float kPunchGaugeCharge = 2.8f;		// パンチ時に増えるゲージ量
	constexpr float kKickGaugeCharge = 5.0f;		// キック時に増えるゲージ量
	constexpr float kDecreaseGauge = 0.0f;			// 攻撃を受けた際に減るゲージ量
	constexpr float kAttackDist = 50.0f;			// 攻撃範囲
	constexpr float kAttackMove = 0.3f;				// 攻撃時の移動量
	constexpr float kHPRecoveryRate = 1.0f;			// プレイヤーのHPが回復する割合
	constexpr float kAngleSpeed = 0.2f;				// プレイヤー角度の変化速度
	constexpr float kScale = 0.3f;					// プレイヤーモデルの拡大率
	constexpr float kAdj = 3.0f;					// 敵に当たった時の位置調整量
	const VECTOR kInitDir = VGet(0.0f, 0.0f, 0.0f);	// 初期方向
	constexpr float kEffectHeight = 30.0f;			// エフェクトを表示する高さ

	/*アニメーション情報*/
	constexpr float kAnimBlendMax = 1.0f;			// アニメーションブレンドの最大値

	/*コントローラー*/
	constexpr int kVibrationPower = 1000;				// 振動の強さ
	constexpr int kVibrationTime = 150;					// 振動させる時間
	constexpr int kSpecialAttackVibrationPower = 150;	// 必殺技時の振動の強さ
	constexpr int kSpecialAttackVibrationTime = 10;		// 必殺技時の振動させる時間
}


/// <summary>
/// コンストラクタ
/// </summary>
Player::Player():
	m_gauge(0.0f),
	m_pToEVec(VGet(0.0f, 0.0f, 0.0f)),
	m_targetMoveDir(kInitDir),
	m_isAccumulateGaugeSe(false)
{
	// キャラクター情報を読み込む
	m_pLoadData = std::make_shared<LoadData>(*this, static_cast<int>(CharaType::kPlayer));
	m_pUIBattle = std::make_shared<UIBattle>(m_status.maxHp, static_cast<int>(CharaType::kPlayer));

	m_hp = m_status.maxHp;
	m_moveSpeed = 0.0f;
	m_angle = 0.0f;
	m_punchCount = 0;
	m_punchComboTime = 0;
	m_punchCoolTime = 0;
	m_kickCoolTime = 0;
	m_attackTime = 0;
	m_avoidCoolTime = 0;
	m_avoidCount = 0;
	m_isMove = false;
	m_isFighting = false;
	m_modelHandle = MV1LoadModel("data/Model/Chara/Player.mv1");

	MV1SetScale(m_modelHandle, VGet(kScale, kScale, kScale));
	m_animBlendRate = kAnimBlendMax;
	PlayAnim(AnimKind::kFightIdle);
}


/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	MV1DeleteModel(m_modelHandle);
}


/// <summary>
/// 初期化
/// </summary>
void Player::Init(std::shared_ptr<EffectManager> pEffect, VECTOR pos)
{
	m_pos = pos;
	m_pEffect = pEffect;
	m_isStartProduction = true;
	m_isClearProduction = false;
	m_isGameoverProduction = false;
	m_isAttack = false;
	m_isSpecialAttack = false;
	m_targetMoveDir = kInitDir;
	MV1SetPosition(m_modelHandle, m_pos);
	m_currentState = CharacterBase::State::kFightIdle;
	PlayAnim(AnimKind::kFightIdle);

	m_pEffect->Init();	// エフェクトの初期化

	// モデル全体のコリジョン情報のセットアップ
	MV1SetupCollInfo(m_modelHandle, -1);
}


/// <summary>
/// 更新
/// </summary>
/// <param name="input">入力コマンド</param>
/// <param name="stage">ステージ情報参照</param>
void Player::Update(const Input& input, const Camera& camera, EnemyBase& enemy, Stage& stage)
{
	// パッド入力によって移動パラメータを設定する
	VECTOR	upMoveVec;		// 上ボタンを入力をしたときのプレイヤーの移動方向ベクトル
	VECTOR	leftMoveVec;	// 左ボタンを入力をしたときのプレイヤーの移動方向ベクトル
	VECTOR	moveVec;		// このフレームの移動ベクトル

	// プレイヤーの状態を更新
	CharacterBase::State prevState = m_currentState;

	// 攻撃処理の更新
	m_attackTime--;

	// 特定の状態中はほかの処理をできないようにする
	const bool isUpdate = (m_currentState == CharacterBase::State::kDown) || !m_isSpecialAttack && !m_isStartProduction && !m_isClearProduction && !m_isGameoverProduction;
	if(isUpdate)
	{	
		Punch(input);						// パンチ処理
		Kick(input);						// キック処理
		Avoid(input, stage, moveVec);		// 回避処理
		Guard(input);						// ガード処理
		Fighting(input);					// 構え処理
	}
	else if (m_isClearProduction)
	{
		DestroyEnemy();						// 敵撃破時処理

	}
	else if (m_isGameoverProduction)
	{
		Gameover();							// ゲームオーバー処理
	}

	if (!m_isClearProduction && !m_isGameoverProduction)
	{
		SpecialAttack(input, enemy);		// 必殺技処理
	}

	m_currentState = UpdateMoveParameter(input, camera, upMoveVec, leftMoveVec, moveVec); // 移動処理

	// エネミーとの当たり判定をチェックする
	enemy.CheckHitPlayerCol(*this, VGet(m_pos.x, m_pos.y + m_colInfo.bodyHeight, m_pos.z), m_pos, m_colInfo.bodyRadius);
	// プレイヤーから敵のベクトルを計算する
	m_pToEVec = VSub(enemy.GetPos(), m_pos);

	UpdateAnimState(prevState);	// アニメーション状態を更新
	UpdateAngle(enemy);			// プレイヤーの移動方向を設定
	Move(moveVec, stage);		// 移動ベクトルを元にプレイヤーを移動させる
	UpdateAnim();				// アニメーション処理の更新
	UpdateCol();				// 当たり判定の位置更新
	UpdatePosLog();				// 位置ログを更新
	m_pUIBattle->UpdateHpBar();	// HPバーの更新
	m_pEffect->Update();		// エフェクト更新
}


/// <summary>
/// 描画
/// </summary>
void Player::Draw()
{
	MV1DrawModel(m_modelHandle);					  // プレイヤー描画
	m_pEffect->Draw();								  // エフェクト描画
	m_pUIBattle->DrawPlayerSilhouette();			  // シルエット描画
	m_pUIBattle->DrawPlayerName();					  // プレイヤー名表示
	m_pUIBattle->DrawPlayerHP(m_hp);				  // HPゲージを表示
	m_pUIBattle->DrawPlayerGauge(m_gauge, kMaxGauge); // ゲージ表示

	// 回避中は残像を表示する
	if (m_currentState == State::kAvoid)
	{
		DrawAfterImage();
	}

	// 必殺技の文字を表示する
	m_pUIBattle->DrawSpecialAttack(m_gauge, kMaxGauge);


#ifdef _DEBUG	// デバッグ表示
	DebugDraw debug;
	debug.DrawPlayerInfo(m_pos, m_hp, static_cast<int>(m_currentState), m_attackTime);
	// 当たり判定描画
	debug.DrawBodyCol(m_col.bodyTopPos, m_col.bodyBottomPos, m_colInfo.bodyRadius); // 全身
	debug.DrawAimCol(m_col.armStartPos, m_col.armEndPos, m_colInfo.aimRadius);		// 腕
	debug.DrawLegCol(m_col.legStartPos, m_col.legEndPos, m_colInfo.legRadius);		// 脚
#endif
}


/// <summary>
/// ダメージを受けた際の処理
/// </summary>
/// <param name="damage">ダメージ量</param>
void Player::OnDamage(float damage)
{
	CharacterBase::OnDamage(damage);

	// ガード状態の場合
	if (m_currentState == CharacterBase::State::kGuard)
	{
		m_pEffect->PlayGuardEffect(VGet(m_pos.x, m_pos.y + kEffectHeight, m_pos.z)); // エフェクト表示
		if (!CheckSoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kGuardAttack)]))
		{
			PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kGuardAttack)], DX_PLAYTYPE_BACK);	// ガード時攻撃SE再生
		}

		// 少し後ろに移動する
		VECTOR backMoveVec = VScale(m_targetMoveDir, -1.0f);
		m_pos = VAdd(m_pos, VScale(backMoveVec, m_status.backMove));
	}
	else
	{
		m_isAttack = false;
		Receive();
		OffGuard();
	}
}


/// <summary>
/// 回復処理
/// </summary>
void Player::Recovery()
{
	// 次試合の前にHPを全回復する
	m_hp = std::min(m_hp + m_status.maxHp * kHPRecoveryRate, m_status.maxHp);
}


/// <summary>
/// 敵との当たり判定をチェックする
/// </summary>
/// <param name="enemy">敵の参照</param>
/// <param name="eCapPosTop">敵のカプセルの上位置</param>
/// <param name="eCapPosBottom">敵のカプセルの下位置</param>
/// <param name="CapRadius">敵の当たり判定の半径</param>
void Player::CheckHitEnemyCol(EnemyBase& enemy, VECTOR eCapPosTop, VECTOR eCapPosBottom, float eCapRadius)
{
	// プレイヤーと敵の当たり判定を行う
	bool isHit = HitCheck_Capsule_Capsule(m_col.bodyTopPos, m_col.bodyBottomPos, m_colInfo.bodyRadius, eCapPosTop, eCapPosBottom, eCapRadius);
	// パンチ
	bool isHitPunch = HitCheck_Capsule_Capsule(m_col.armStartPos, m_col.armEndPos, m_colInfo.aimRadius, eCapPosTop, eCapPosBottom, eCapRadius);
	// キック
	bool isHitKick = HitCheck_Capsule_Capsule(m_col.legStartPos, m_col.legEndPos, m_colInfo.legRadius, eCapPosTop, eCapPosBottom, eCapRadius);

	// 背後から攻撃したかどうか
	bool isBackAttack = m_pToEVec.x < 0.0f;

	// パンチ状態かどうか
	bool isStatePunch = m_currentState == CharacterBase::State::kPunch1 || m_currentState == CharacterBase::State::kPunch2 || m_currentState == CharacterBase::State::kPunch3;

	// パンチが当たった場合
	if (isHitPunch && isStatePunch)
	{
		if (m_attackTime > 0) return;
		AdjAttackPos(enemy); // 敵に近づく

		// 敵がガードしていないか、背後から攻撃した場合
		if (!enemy.GetIsGuard() || isBackAttack)
		{
			VibrationPad(); // パッドを振動させる

			// 1コンボ目
			if (m_currentState == CharacterBase::State::kPunch1)
			{
				enemy.OnDamage(m_status.punchPower);
				m_attackTime = m_status.punchTime;
				m_gauge += kPunchGaugeCharge;	// 必殺技ゲージを増やす
			}
			// 2コンボ目
			if (m_currentState == CharacterBase::State::kPunch2)
			{
				enemy.OnDamage(m_status.secondPunchPower);
				m_attackTime = m_status.punchTime;
				m_gauge += kPunchGaugeCharge;
			}
			// 3コンボ目
			if (m_currentState == CharacterBase::State::kPunch3)
			{
				enemy.OnDamage(m_status.thirdPunchPower);
				m_attackTime = m_status.punchTime;
				m_gauge += kPunchGaugeCharge;
			}
		}
		else
		{
			enemy.OnDamage(0.0f);
		}
	}
	// キックが当たった場合
	else if (isHitKick && m_currentState == CharacterBase::State::kKick)
	{
		if (m_attackTime > 0) return;
		AdjAttackPos(enemy); // 敵に近づく

		if (!enemy.GetIsGuard() || isBackAttack)
		{
			enemy.OnDamage(m_status.kickPower);
			VibrationPad();	 // パッドを振動させる
			m_attackTime = m_status.kickTime;
			m_gauge += kKickGaugeCharge;
		}
		else
		{
			enemy.OnDamage(0.0f);
		}
	}
	// 攻撃中でなく、敵に当たった場合
	else if(isHit)
	{
		// プレイヤーの位置を補正する
		VECTOR collisionNormal = VNorm(VSub(m_pos, enemy.GetPos()));
		m_pos = VAdd(m_pos, VScale(collisionNormal, kAdj));
	}

	// 掴みが決まらなかった場合
	if(!isHit && m_currentState == CharacterBase::State::kGrab)
	{
		// 掴み失敗のアニメーションを再生する
		PlayAnim(CharacterBase::AnimKind::kStumble);
	}

	// ゲージが溜まったらSEを再生する
	if (m_gauge >= kMaxGauge && !m_isAccumulateGaugeSe)
	{
		if (!CheckSoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kAccumulateGauge)]))
		{
			PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kAccumulateGauge)], DX_PLAYTYPE_BACK);
			m_isAccumulateGaugeSe = true;
		}
	}

	if (m_gauge < kMaxGauge)
	{
		m_isAccumulateGaugeSe = false;
	}

	// ゲージ量の調整
	m_gauge = std::min(m_gauge, kMaxGauge);
	m_gauge = std::max(0.0f, m_gauge);
}


/// <summary>
/// プレイヤーの移動処理
/// </summary>
/// <param name="MoveVec">移動ベクトル</param>
/// <param name="stage">ステージ参照</param>
void Player::Move(const VECTOR& moveVec, Stage& stage)
{
	if (fabs(moveVec.x) > 0.0f || fabs(moveVec.z) > 0.0f)
	{
		m_isMove = true;
	}
	else
	{
		m_isMove = false;
	}

	// 当たり判定を行って座標を保存する
	m_pos = stage.CheckPlayerCol(*this, moveVec);

	// プレイヤーの座標を更新する
	MV1SetPosition(m_modelHandle, m_pos);
}


/// <summary>
/// パンチ攻撃処理
/// </summary>
/// <param name="input">入力処理</param>
/// <returns>現在の状態</returns>
void Player::Punch(const Input& input)
{
	// 特定の状態中はスキップ
	const bool isSkip = m_isAttack || m_isReceive || m_isGuard || m_isClearProduction || m_hp <= 0;
	if (isSkip) return;

	// パンチできない場合
	if (m_punchCoolTime > 0)
	{
		m_punchCoolTime--;
		return;
	}

	// コンボ入力の受付時間の更新
	m_punchComboTime--;

	if (input.IsTriggered("punch"))
	{
		m_isGuard = false;

		UpdateAttackAngle(); // 攻撃角度を更新

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
			m_currentState = CharacterBase::State::kPunch1;
			PlayAnim(AnimKind::kPunch1);
			break;
		case 1:
			m_punchComboTime = m_status.punchReceptionTime;
			m_isAttack = true;
			m_isFighting = false;
			m_currentState = CharacterBase::State::kPunch2;
			PlayAnim(AnimKind::kPunch2);
			break;
		case 2:
			m_punchComboTime = m_status.punchReceptionTime;
			m_isAttack = true;
			m_isFighting = false;
			m_currentState = CharacterBase::State::kPunch3;
			PlayAnim(AnimKind::kPunch3);
			break;
		case 3:
			m_punchCount = -1;
			m_punchCoolTime = m_status.punchCoolTime;	// クールダウンタイムを設定
			break;
		default:
			break;
		}
	}
}


/// <summary>
/// キック攻撃処理
/// </summary>
/// <param name="input">入力処理</param>
/// <returns>現在の状態</returns>
void Player::Kick(const Input& input)
{
	// 特定の状態中はスキップ
	const bool isSkip = m_isAttack || m_isReceive || m_isClearProduction || m_isGuard || m_hp <= 0;;
	if (isSkip) return;

	UpdateAttackAngle(); // 攻撃角度を更新

	// キックできない場合
	if (m_kickCoolTime > 0)
	{
		m_kickCoolTime--;
		return;
	}
	// キック攻撃
	else if (input.IsTriggered("kick"))
	{
		m_isAttack = true;
		m_isFighting = false;
		m_isGuard = false;
		m_kickCoolTime = m_status.kickCoolTime;	// クールダウンタイムを設定
		m_currentState = CharacterBase::State::kKick;
		PlayAnim(AnimKind::kKick);
	}
}


/// <summary>
/// 回避処理
/// </summary>
/// <param name="input">入力処理</param>
/// <returns>現在の状態</returns>
void Player::Avoid(const Input& input, Stage& stage, VECTOR& moveVec)
{
	// 特定の状態中はスキップ
	const bool isSkip = m_isGuard || m_isClearProduction || m_hp <= 0;
	if (isSkip) return;

	// 回避できない場合
	if (m_avoidCoolTime > 0)
	{
		m_avoidCoolTime--;
		return;
	}

	// ボタンを押した場合
	if (input.IsTriggered("avoidance"))
	{
		m_isFighting = false;
		m_isGuard = false;
		m_avoidCount++;
		// 回避数が最大になった場合
		if (m_avoidCount > m_status.maxAvoidCount)
		{
			m_avoidCount = 0;
			m_avoidCoolTime = m_status.avoidCoolTime;	// クールダウンタイムを設定
		}
		else
		{
			PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kAvoid)], DX_PLAYTYPE_BACK); // SE再生
			m_currentState = CharacterBase::State::kAvoid;

			// 移動ベクトルを設定する
			// 方向キーを入力中は入力方向へ移動する
			if (input.IsPressing("up") || input.IsPressing("down") || input.IsPressing("left") || input.IsPressing("right"))
			{
				m_pos = VAdd(m_pos, VScale(m_targetMoveDir, m_status.avoidDist));
			}
			// 方向キーが入力されていない場合は後ろへ移動する
			else
			{
				VECTOR backMoveVec = VScale(m_targetMoveDir, -1.0f);
				m_pos = VAdd(m_pos, VScale(backMoveVec, m_status.avoidDist));
			}
		}
	}
}


/// <summary>
/// 構え中処理
/// </summary>
/// <param name="input">入力処理</param>
/// <param name="enemy">敵情報参照</param>
/// <returns>現在の状態</returns>
void Player::Fighting(const Input& input)
{
	// 特定の状態中はスキップ
	const bool isSkip = m_isAttack || m_isReceive || m_isGuard || m_isClearProduction || m_hp <= 0;
	if (isSkip) return;

	if (input.IsTriggered("fighting"))
	{
		m_isFighting = true;
		m_isAttack = false;
		m_isGuard = false;
		m_currentState = CharacterBase::State::kFightWalk;
		PlayAnim(AnimKind::kFightWalk);
	}
	else if(input.IsReleased("fighting"))
	{
		m_isFighting = false;
		m_currentState = CharacterBase::State::kFightIdle;
		PlayAnim(AnimKind::kFightIdle);
	}
}


/// <summary>
/// ガード処理
/// </summary>
/// <param name="input">入力処理</param>
/// <returns>現在の状態</returns>
void Player::Guard(const Input& input)
{
	if (m_isClearProduction || m_hp <= 0) return; // 演出時は操作できないようにする

	UpdateAttackAngle(); // 攻撃角度を更新
	
	if (input.IsTriggered("guard"))
	{
		m_isAttack = false;
		m_isFighting = false;
		m_isGuard = true;
		m_attackTime = 0;
		m_currentState = CharacterBase::State::kGuard;
		PlayAnim(AnimKind::kGuard);
	}
	else if (input.IsReleased("guard"))
	{
		OffGuard();
	}
}


/// <summary>
/// ガード状態を解除する
/// </summary>
void Player::OffGuard()
{
	m_isGuard = false;
	m_currentState = CharacterBase::State::kFightIdle;
	PlayAnim(AnimKind::kFightIdle);
}


/// <summary>
/// 攻撃を受けている最中の処理
/// </summary>
void Player::Receive()
{
	m_currentState = CharacterBase::State::kReceive;
	PlayAnim(CharacterBase::AnimKind::kReceive);
	m_pEffect->PlayDamageEffect(VGet(m_pos.x, m_pos.y + kEffectHeight, m_pos.z));				 // 攻撃エフェクト表示
	PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kAttack)], DX_PLAYTYPE_BACK); // 攻撃SE再生
	// ゲージを減らす
	m_gauge -= kDecreaseGauge;
	m_pUIBattle->ResetSpecialAttack();
}


/// <summary>
/// 必殺技の処理
/// </summary>
/// <param name="input">入力処理</param>
void Player::SpecialAttack(const Input& input, EnemyBase& enemy)
{
	if (m_isSpecialAttack)
	{
		VibrationPad(); // パッドを振動させる
		enemy.OnDamage(m_status.specialAttackPower);
	}

	// プレイヤーと敵の距離を求める
	float pToEDist = VSize(m_pToEVec);

	// ゲージが溜まっていない、または敵が近くにいないときは必殺技を出せないようにする
	if (m_gauge < kMaxGauge || pToEDist >= kAttackDist) return;

	// ボタンを押したら必殺技を発動する
	if (input.IsTriggered("special"))
	{
		m_isSpecialAttack = true;
		m_gauge = 0.0f;	// ゲージを減らす

		m_currentState = State::kSpecialAttack;
		PlayAnim(AnimKind::kSpecialAttack);

		if (!CheckSoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kSpecialAttack)]))
		{
			PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kSpecialAttack)], DX_PLAYTYPE_BACK); // SEを鳴らす
		}
	}
}


/// <summary>
/// 攻撃時に位置を位置を調整する
/// </summary>
void Player::AdjAttackPos(EnemyBase& enemy)
{
	if (VSize(m_pToEVec) > kAttackDist)
	{
		m_pos = VAdd(m_pos, VScale(m_pToEVec, kAttackMove));
	}
}


/// <summary>
/// 敵を倒した時の処理
/// </summary>
void Player::DestroyEnemy()
{
	// 待機状態にする
	m_currentState = State::kFightIdle;
	PlayAnim(AnimKind::kFightIdle);
}


/// <summary>
/// ゲームオーバー時の処理
/// </summary>
void Player::Gameover()
{
	m_isAttack = false;
	m_isFighting = false;
	m_isGuard = false;
	// ダウン状態にする
	m_currentState = CharacterBase::State::kDown;
	PlayAnim(CharacterBase::AnimKind::kDown);
}


/// <summary>
/// 移動パラメータを設定する
/// </summary>
/// <param name="input">入力処理</param>
/// <param name="camera">カメラ</param>
/// <param name="upMoveVec">上方向への移動ベクトル</param>
/// <param name="leftMoveVec">左方向への移動ベクトル</param>
/// <param name="moveVec">移動ベクトル</param>
/// <returns>現在の状態</returns>
Player::CharacterBase::State Player::UpdateMoveParameter(const Input& input, const Camera& camera, VECTOR& upMoveVec, VECTOR& leftMoveVec, VECTOR& moveVec)
{
	CharacterBase::State nextState = m_currentState;

	/*プレイヤーの移動方向ベクトルを求める*/
	// 上ボタンを押したとき
	upMoveVec = VSub(camera.GetAngle(), camera.GetPos());
	upMoveVec.y = 0.0f;

	// 左ボタンを押したとき
	leftMoveVec = VCross(upMoveVec, VGet(0.0f, 1.0f, 0.0f));

	// ベクトルの正規化
	upMoveVec = VNorm(upMoveVec);
	leftMoveVec = VNorm(leftMoveVec);

	// このフレームでの移動ベクトルを初期化
	moveVec = VGet(0.0f, 0.0f, 0.0f);

	// 移動したか(true:移動した)
	bool isPressMove = false;

	// 特定の状態中は動けないようにする
	const bool isMove = !m_isAttack && !m_isSpecialAttack && !m_isGuard && !m_isStartProduction && !m_isClearProduction && !m_isGameoverProduction;
	if (isMove)
	{
		// ボタンを押したら移動
		if (input.IsPressing("up"))
		{
			moveVec = VAdd(moveVec, upMoveVec);
			isPressMove = true;
		}
		if (input.IsPressing("down"))
		{
			moveVec = VAdd(moveVec, VScale(upMoveVec, -1.0f));
			isPressMove = true;
		}
		if (input.IsPressing("left"))
		{
			moveVec = VAdd(moveVec, leftMoveVec);
			isPressMove = true;
		}
		if (input.IsPressing("right"))
		{
			moveVec = VAdd(moveVec, VScale(leftMoveVec, -1.0f));
			isPressMove = true;
		}

		// 移動ボタンが押されている場合
		if (isPressMove)
		{
			// 待機状態の場合
			if (m_currentState == CharacterBase::State::kFightIdle)
			{
				nextState = CharacterBase::State::kRun; // 移動状態にする
			}

			// プレイヤーが向く方向を設定する
			m_targetMoveDir = VNorm(moveVec);

			// プレイヤーの加速度を設定する
			if (m_moveSpeed < m_status.maxMoveSpeed)
			{
				m_moveSpeed += m_status.acceleration;
				m_moveSpeed = (std::min)(m_moveSpeed, m_status.maxMoveSpeed);
			}
			// プレイヤーの移動ベクトルを設定する
			// 構え中は移動速度を遅くする
			if (m_isFighting)
			{
				moveVec = VScale(m_targetMoveDir, m_status.fightWalkSpeed);
			}
			else
			{
				moveVec = VScale(m_targetMoveDir, m_moveSpeed);
			}
		}
		// 移動しない場合
		else
		{
			// 移動状態の場合
			if (m_currentState == CharacterBase::State::kRun)
			{
				// 待機状態にする
				nextState = CharacterBase::State::kFightIdle;
				m_moveSpeed = 0.0f;
			}

			// プレイヤーを減速させる
			if (m_moveSpeed > 0.0f)
			{
				m_moveSpeed -= m_status.deceleration;
				m_moveSpeed = (std::max)(0.0f, m_moveSpeed);
			}
			moveVec = VScale(m_targetMoveDir, m_moveSpeed);
		}
	}

	return nextState;
}


/// <summary>
/// プレイヤーの回転を制御する
/// </summary>
void Player::UpdateAngle(EnemyBase& enemy)
{
	// プレイヤーの角度を更新
	if (m_isFighting || m_isSpecialAttack)
	{
		// 敵の方を見続ける
		VECTOR dir = VSub(enemy.GetPos(), m_pos);
		m_angle = atan2f(dir.x, dir.z);
	}
	else
	{
		m_angle = atan2f(m_targetMoveDir.x, m_targetMoveDir.z);
	}
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, m_angle + DX_PI_F, 0.0f));
}


/// <summary>
/// 攻撃時の角度を更新する
/// </summary>
void Player::UpdateAttackAngle()
{
	// 敵から一定の角度内を向いている場合、敵の方向を向くようにする
	// 内積を使って角度を求める
	float dot = VDot(VNorm(m_pToEVec), VNorm(m_targetMoveDir));
	float angle = acos(dot) * (180.0f / DX_PI_F);
	if (angle <= 90.0f)
	{
		m_angle = atan2f(m_targetMoveDir.x, m_targetMoveDir.z);
	}
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, m_angle + DX_PI_F, 0.0f));
}


/// <summary>
/// パッドを振動させる
/// </summary>
void Player::VibrationPad()
{
	if (m_isSpecialAttack)
	{
		StartJoypadVibration(DX_INPUT_PAD1, kSpecialAttackVibrationPower, kSpecialAttackVibrationTime, -1);
	}
	else
	{
		StartJoypadVibration(DX_INPUT_PAD1, kVibrationPower, kVibrationTime, -1);
	}
}
