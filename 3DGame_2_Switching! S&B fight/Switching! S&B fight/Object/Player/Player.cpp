#include "Player.h"
#include "DxLib.h"
#include "../Model.h"
#include "../../Util/Pad.h"
#include "../../Object/Camera.h"
#include "PlayerState.h"
#include "../GameMap.h"
#include "../Enemy/EnemyRight.h"
#include "../Enemy/EnemyLeft.h"
#include "../../Manager/SoundManager.h"
#include "../../Util/Effect.h"
#include <cassert>
#include <cmath>
#include <vector>

bool col;

namespace {

	const char* const kUI[5]{
		"data/UI/GameScene/Player/HPOurGauge.png",// HPUI(外側)
		"data/UI/GameScene/Player/HPInGauge.png",	// HPUI(内側)
		"data/UI/GameScene/Player/NameBg.png",	// 名前背景UI
		"data/UI/GameScene/Player/Fukuoka.png",	// 名前UI
		"data/UI/GameScene/Player/Face.png"		// 顔アイコン
	};

	const char* const kModelPlayer = "data/model/RogueHooded.mv1";	// モデルのファイル名

	constexpr float kInitAngle = -DX_PI_F / 2.0f * 90.0f;	// プレイヤーの初期角度*90(向きを反対にする)
	constexpr float kModelSize = 5.0f;			// モデルのサイズ
	constexpr float kSpeed = 0.7f;				// プレイヤー移動速度
	constexpr float kAttackSpeed = 0.5f;		// プレイヤー攻撃時の加速度
	constexpr float	kAngleSpeed = 0.2f;			// 角度変化速度
	constexpr float	kJumpPower = 1.8f;			// ジャンプ力
	constexpr float	kGravity = 0.05f;			// 重力
	constexpr int	kMaxHp = 100;				// 体力最大値
	constexpr int	kAttack = 20;				// 攻撃力

	constexpr float kEffectHeight = 10.0f;			// エフェクトを表示する高さ

	// アイコン位置
	constexpr int kFaceUIPosX = 0;
	constexpr int kFaceUIPosY = 30;

	// HPゲージ(外側)位置
	constexpr int kHpGaugeUIPosX = 130;
	constexpr int kHpGaugeUIPosY = 85;

	// HPゲージ(内側)右側位置(Exted用右端座標)
	constexpr int kHpGaugePosX = 852;
	constexpr int kHpGaugePosY = kHpGaugeUIPosY + 42;

	// 名前背景位置
	constexpr int kNameBgX = 130;
	constexpr int kNameBgY = 10;

	// 名前位置
	constexpr int kNameX = kNameBgX + 80;
	constexpr int kNameY = kNameBgY + 10;


	const VECTOR kUpPos = VGet(0.0f, 7.0f, 0.0f);
	const VECTOR kAttackRange = VGet(0.0f, 0.0f, 8.0f);
	constexpr float kColRadius = 2.5;
	constexpr float kAttackColRadius = 3.0;

	// 初期化用値
	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// ベクトルの初期化
	constexpr float kInitFloat = 0.0f;				// float値の初期化

	int waitTime = 0;
}

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player() :
//	m_uiGraph(-1),
	m_angle(kInitFloat),
	m_gravity(kGravity),
	m_addDamage(0),
	m_isAttack(false),
	m_isNextAttackFlag(false),
	m_isFirstAttack(false),
	m_isAttackDamage(false),
	m_isWalk(false),
	m_isJump(false),
	m_isDeath(false),
	m_isAttackLeft(false),
	m_isAttackRight(false),
	m_isAttackEffect(false),
	m_jumpPower(0.0f),
	m_multiAttack(0),
	m_pos(kInitVec),
	m_move(kInitVec),
	m_UpPos(kInitVec),
	m_hp(kMaxHp),
	m_targetDir(VGet(0.0f, 0.0f, 0.0f)),
	isCol(false)
{
	// UI画像の読み込み
	for (int i = 0; i < m_uiGraph.size(); i++)
	{
		m_uiGraph[i] = LoadGraph(kUI[i]);
		assert(m_uiGraph[i] != -1);
	}

	//モデルインスタンス作成
	m_pModel = std::make_shared<Model>(kModelPlayer);
	// アイドル状態のアニメーションを再生させる
	m_pModel->SetAnim(m_animData.kIdle, false, true);

	//ステイトクラスのインスタンス生成
	m_pState = std::make_shared<PlayerState>();
	m_pEffect = std::make_shared<Effect>();

	m_pState->AddState([=] { IdleStateUpdate(); }, [=] { IdleStateInit(); }, PlayerState::State::kIdle);
	m_pState->AddState([=] { WalkStateUpdate(); }, [=] { WalkStateInit(); }, PlayerState::State::kWalk);
	m_pState->AddState([=] { JumpStateUpdate(); }, [=] { JumpStateInit(); }, PlayerState::State::kJump);
	m_pState->AddState([=] { AttackSordStateUpdate(); }, [=] { AttackSordStateInit(); }, PlayerState::State::kAttackSord);
	m_pState->AddState([=] { AttackBowStateUpdate(); }, [=] { AttackBowStateInit(); }, PlayerState::State::kAttackBow);

	//初期ステイトセット
	m_pState->SetState(PlayerState::State::kIdle);	//ステイトセット(最初はIdle状態)

	m_pSound = new SoundManager;
	m_pSound->InitSound();
	m_pSound->LoadSE(SoundManager::SE_Type::kSord1SE);
	m_pSound->LoadSE(SoundManager::SE_Type::kSord2SE);
	m_pSound->LoadSE(SoundManager::SE_Type::kSord3SE);

	

	col = false;
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
}

/// <summary>
/// 初期化
/// </summary>
void Player::Init(std::shared_ptr<GameMap> pMap)
{
	// プレイヤー初期設定
	m_pModel->SetSize(VGet(kModelSize, kModelSize, kModelSize));
	m_pModel->SetRota(VGet(0.0f, kInitAngle, 0.0f));
	m_pModel->SetPos(m_pos);

	mp.leftBack = pMap->GetMapLeftBack();
	mp.rightFront = pMap->GetMapRightFront();

	m_pEffect->Init();
}


/// <summary>
/// 更新
/// </summary>
void Player::Update(const Camera& camera, const EnemyRight& enemyR, const EnemyLeft& enemyL)
{
	isCol = false;
	col = false;
	// パッド入力によって移動パラメータを設定する
	VECTOR	upMoveVec;		// 方向ボタン「↑」を入力をしたときのプレイヤーの移動方向ベクトル
	VECTOR	leftMoveVec;	// 方向ボタン「←」を入力をしたときのプレイヤーの移動方向ベクトル

	// ステイトの更新
	m_pState->Update();

	// プレイヤーの状態更新
	// 攻撃処理
	Attack(enemyR, enemyL);
	// 移動処理
	OldMoveValue(camera, upMoveVec, leftMoveVec);

	// モデルのアップデート
	m_pModel->Update();

	// プレイヤーの移動方向にモデルの方向を近づける
	Angle();

	// プレイヤーの座標更新
	Move(m_move, enemyR, enemyL);

	// プレイヤーの死亡確認処理
	Death();

	// プレイヤー当たり判定用カプセル型の座標更新
	m_UpPos = VAdd(m_pos, kUpPos);

	MATRIX rotationMatrix = MGetRotY(m_angle);
	m_attackRange = VAdd(m_pos, VTransform(kAttackRange, rotationMatrix));

	m_colSphere.UpdateCol(m_pos, m_UpPos, m_attackRange,
		kColRadius, kAttackColRadius);

	m_pEffect->Update();		// エフェクト更新
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw()
{
	// HPゲージ描画
	DrawExtendGraph(kHpGaugeUIPosX, kHpGaugeUIPosY, kHpGaugeUIPosX + (kHpGaugePosX * (m_hp * 0.01f)), kHpGaugePosY,m_uiGraph[1], true);
	DrawGraph(kHpGaugeUIPosX, kHpGaugeUIPosY, m_uiGraph[0], true);

	// プレイヤー情報描画
	DrawGraph(kNameBgX, kNameBgY, m_uiGraph[2], true);
	DrawGraph(kNameX, kNameY, m_uiGraph[3], true);
	DrawGraph(kFaceUIPosX, kFaceUIPosY, m_uiGraph[4], true);


	m_pModel->Draw();

	m_pEffect->Draw();			 // エフェクト描画

#ifdef _DEBUG
	m_colSphere.DrawMain(0xff0000, false);	// 当たり判定描画
	m_colSphere.DrawAttack(0x0000ff, false);	// 当たり判定描画

	DrawFormatString(0, 200, 0xffffff, "Player:m_move.x,y,z=%.2f,=%.2f,=%.2f", m_move.x, m_move.y, m_move.z);

	DrawFormatString(0, 180, 0xffffff, "Player:m_pos.x,y,z=%.2f,=%.2f,=%.2f", m_pos.x, m_pos.y, m_pos.z);
	DrawFormatString(0, 220, 0xffffff, "Player:m_hp=%d", m_hp);

	DrawFormatString(0, 280, 0xffffff, "State=%d", m_pState->GetState());
	DrawFormatString(0, 300, 0xffffff, "m_isWalk=%d", m_isWalk);
	DrawFormatString(0, 320, 0xffffff, "m_angle=%.2f", m_angle);

	DrawFormatString(0, 420, 0xffffff, "m_animSpeed=%.2f", m_pModel->GetAnimSpeed());

	if(col) {
		DrawString(0, 500, "当たっている", 0xffffff);
	}
	else
	{
		DrawString(0, 500, "当たっていない", 0xffffff);
	}

	if (isCol)
	{
		DrawString(0, 520, "当たっている", 0xffffff);
	}
	else
	{
		DrawString(0, 520, "当たっていない", 0xffffff);
	}
#endif // DEBUG
}

/// <summary>
/// 終了
/// </summary>
void Player::End()
{
	// UI画像の読み込み
	for (int i = 0; i < m_uiGraph.size(); i++)
	{
		DeleteGraph(m_uiGraph[i]);
	}

	m_pSound->ReleaseSound();

	delete m_pSound;
	m_pSound = nullptr;
}

void Player::JumpStateInit()
{
	m_isJump = true;
	m_jumpPower = kJumpPower;
}

/// <summary>
/// 剣攻撃
/// </summary>
void Player::AttackSordStateInit()
{
	m_isAttack = true;
	m_multiAttack = 0;
	m_isNextAttackFlag = false;
	m_isFirstAttack = true;
	m_attackKind = AttackKind::kAttackSord;
	m_pSound->PlaySE(SoundManager::SE_Type::kSord1SE, DX_PLAYTYPE_BACK);
}

/// <summary>
/// 弓攻撃
/// </summary>
void Player::AttackBowStateInit()
{
	m_isAttack = true;
	m_isNextAttackFlag = false;
	m_isFirstAttack = true;
	m_attackKind = AttackKind::kAttackBow;
}

void Player::IdleStateUpdate()
{
	// アニメーションを待機モーションに変更
	m_pModel->ChangeAnim(m_animData.kIdle, true, false, 0.5f);
}

void Player::WalkStateUpdate()
{
	// アニメーションを歩きモーションに変更
	m_pModel->ChangeAnim(m_animData.kWalk, true, false, 0.5f);
}

void Player::JumpStateUpdate()
{
	// アニメーションをジャンプモーションに変更
	m_pModel->ChangeAnim(m_animData.kJump, false, false, 1.0f);
}

/// <summary>
/// 剣攻撃
/// </summary>
void Player::AttackSordStateUpdate()
{
	// アニメーション変更
	switch (m_multiAttack)
	{
	case 0:
		m_pModel->ChangeAnim(m_animData.kAttack1, false, false, 1.0f);
		break;
	case 1:
		m_pModel->ChangeAnim(m_animData.kAttack2, false, false, 1.0f);
		break;
	case 2:
		m_pModel->ChangeAnim(m_animData.kAttack3, false, false, 1.0f);
		break;
	case 3:
		m_pModel->ChangeAnim(m_animData.kAttack4, false, false, 1.0f);
		break;
	default:
		break;
	}

	if (Pad::IsTrigger(PAD_INPUT_X) && !m_isNextAttackFlag)
	{
		if (!m_isFirstAttack)
		{
			m_isNextAttackFlag = true;
		}
		m_isFirstAttack = false;
	}

	if (m_pModel->IsAnimEnd())
	{
		// アニメーションが終わった段階で次の攻撃フラグがたっていなかったら
		if (!m_isNextAttackFlag)
		{
			//m_isAttackDamage = true;
			m_isAttack = false;
			m_multiAttack = 0;
			m_pState->EndState();
		}

		// アニメーションが終わった段階で次の攻撃フラグがたっていたら
		if (m_isNextAttackFlag)
		{
			//m_isAttackDamage = true;
			m_pSound->PlaySE(SoundManager::SE_Type::kSord1SE, DX_PLAYTYPE_BACK);
			m_isNextAttackFlag = false;
			m_multiAttack++;
		}
	}

	if (m_multiAttack >= 4)
	{
		m_multiAttack = 0;
	}
}

/// <summary>
/// 弓攻撃
/// </summary>
void Player::AttackBowStateUpdate()
{
	bool loop = false;
	if (Pad::IsPress(PAD_INPUT_B))	// Bボタン
	{
		loop = true;

		waitTime--;
		if (waitTime <= 0)
		{
			//m_isAttackDamage = true;
			waitTime = 20;
		}
	}

	m_pModel->ChangeAnim(m_animData.kAttackBow, loop, false, 0.5f);

	// アニメーションが終わったら
	if (!loop)
	{
		m_isAttack = false;
		//m_isAttackDamage = false;
		m_pState->EndState();
		waitTime = 0;
	}
}

/// <summary>
/// 移動パラメータの設定
/// </summary>
void Player::OldMoveValue(const Camera& camera, VECTOR& upMoveVec, VECTOR& leftMoveVec)
{
	// プレイヤーの移動方向のベクトルを算出
	// 方向ボタン「↑」を押したときのプレイヤーの移動ベクトルはカメラの視線方向からＹ成分を抜いたもの
	upMoveVec = VSub(camera.GetTarget(), camera.GetPosition());
	upMoveVec.y = 0.0f;

	// 方向ボタン「←」を押したときのプレイヤーの移動ベクトルは上を押したときの方向ベクトルとＹ軸のプラス方向のベクトルに垂直な方向
	leftMoveVec = VCross(upMoveVec, VGet(0.0f, kSpeed, 0.0f));

	// 移動値を初期値に戻す
	m_move = VGet(0.0f, 0.0f, 0.0f);

	// 移動したか(true:移動した)
	bool isPressMove = false;

	// 移動処理
	if (!m_isAttack)
	{
		if (Pad::IsPress(PAD_INPUT_RIGHT))						// 右方向
		{
			m_move = VAdd(m_move, VScale(leftMoveVec, -1.0f));
			isPressMove = true;
		}
		if (Pad::IsPress(PAD_INPUT_LEFT))						// 左方向
		{
			m_move = VAdd(m_move, leftMoveVec);
			isPressMove = true;
		}
		if (Pad::IsPress(PAD_INPUT_UP))							// 前方向
		{
			m_move = VAdd(m_move, upMoveVec);
			isPressMove = true;
		}
		if (Pad::IsPress(PAD_INPUT_DOWN))						// 後ろ方向
		{
			m_move = VAdd(m_move, VScale(upMoveVec, -1.0f));
			isPressMove = true;
		}

		// 正規化
		if (VSize(m_move) > 0.0f)
		{
			m_move = VNorm(m_move);
			m_targetDir = m_move;
			m_move = VScale(m_move, kSpeed);
		}
	}

	//ジャンプ処理
	Jump();
}

/// <summary>
/// プレイヤーの移動処理
/// </summary>
/// <param name="MoveVector">移動量</param>
void Player::Move(const VECTOR& MoveVector, const EnemyRight& enemyR, const EnemyLeft& enemyL)
{
	if (fabs(MoveVector.x) > 0.0f || fabs(MoveVector.z) > 0.0f)
	{
		m_isWalk = true;
	}
	else
	{
		m_isWalk = false;
	}

	Collision enemyRightCol = enemyR.GetColSphere();
	Collision enemyLeftCol = enemyL.GetColSphere();

	if (m_colSphere.IsBodyCollision(enemyRightCol))
	{
		VECTOR colNormal = VNorm(VSub(m_pos, enemyR.GetPos()));
		m_pos = VAdd(m_pos, VScale(colNormal, 0.7f));
		col = true;
	}
	if (m_colSphere.IsBodyCollision(enemyLeftCol))
	{
		VECTOR colNormal = VNorm(VSub(m_pos, enemyL.GetPos()));
		m_pos = VAdd(m_pos, VScale(colNormal, 0.7f));
		col = true;
	}

	// プレイヤーの位置に移動量を足す
	m_pos = VAdd(m_pos, m_move);

	


	// プレイヤーが画面外に出ないようする処理
	if (m_pos.x < mp.leftBack.x)
	{
		m_pos.x -= m_move.x;		// 左
	}
	if (m_pos.x > mp.rightFront.x)
	{
		m_pos.x -= m_move.x;		// 右
	}
	if (m_pos.z < mp.rightFront.z)
	{
		m_pos.z -= m_move.z;		// 前
	}
	if (m_pos.z > mp.leftBack.z)
	{
		m_pos.z -= m_move.z;		// 奥
	}

	// プレイヤーの位置セット
	MV1SetPosition(m_pModel->GetModel(), m_pos);
}

/// <summary>
/// プレイヤーの角度処理
/// </summary>
void Player::Angle()
{
	// プレイヤーの移動方向にモデルの方向を近づける
	float targetAngle;		// 目標角度
	float difference;		// 目標角度と現在の角度の差

	// 目標の方向ベクトルから角度値を算出する
	targetAngle = static_cast<float>(atan2(m_targetDir.x, m_targetDir.z));

	// 目標の角度と現在の角度との差を割り出す
	difference = targetAngle - m_angle;

	// 差の角度が180度以上になっていたら修正する
	if (difference < -DX_PI_F)
	{
		difference += DX_TWO_PI_F;
	}
	else if (difference > DX_PI_F)
	{
		difference -= DX_TWO_PI_F;
	}

	// 角度の差が0に近づける
	if (difference > 0.0f)
	{
		// 差がプラスの場合は引く
		difference -= kAngleSpeed;
		if (difference < 0.0f)
		{
			difference = 0.0f;
		}
	}
	else
	{
		// 差がマイナスの場合は足す
		difference += kAngleSpeed;
		if (difference > 0.0f)
		{
			difference = 0.0f;
		}
	}

	// モデルの角度を更新
	m_angle = targetAngle - difference;
	MV1SetRotationXYZ(m_pModel->GetModel(), VGet(0.0f, m_angle + DX_PI_F, 0.0f));
}


void Player::Attack(const EnemyRight& enemyR, const EnemyLeft& enemyL)
{
	Collision enemyRightCol = enemyR.GetColSphere();
	Collision enemyLeftCol = enemyL.GetColSphere();

	m_addDamage = 0;
	m_isAttackLeft = false;
	m_isAttackRight = false;

	/*if (m_isForward)
	{
		m_moveAttack = VAdd(m_moveAttack, VGet(0.0f, 0.0f, kAttackSpeed));
		m_isForward = false;
	}*/
	
	if (m_colSphere.IsAttackCollision(enemyLeftCol))
	{
		isCol = true;
		m_isAttackLeft = true;
	}
	if (m_colSphere.IsAttackCollision(enemyRightCol))
	{
		isCol = true;
		m_isAttackRight = true;
	}

	if (m_isAttack && isCol) {
		if (m_pModel->GetAnimSpeed() >= 5.0f && m_pModel->GetAnimSpeed() < 6.0f)
		{
			m_isAttackDamage = true;
			m_pEffect->PlayDamageEffect(VGet(m_pos.x, m_pos.y + kEffectHeight, m_pos.z));
		}
	}

	if (isCol && m_isAttackDamage)
	{
		m_addDamage = kAttack;
		m_isAttackDamage = false;
	}

	
}

void Player::Jump()
{
	if (!m_isJump)	return;

	if (m_pos.y >= 0.0f)
	{
		// ジャンプ状態なら重力適用
		if (m_isJump)
		{
			// Ｙ軸方向の速度を重力分減算する
			m_jumpPower -= m_gravity;
			m_gravity += 0.005f;
		}

		// 移動ベクトルのＹ成分をＹ軸方向の速度にする
		m_move.y = m_jumpPower;
	}
	else {
		m_isJump = false;
		m_pos.y = 0.0f;
		m_gravity = kGravity;
		m_pState->EndState();
	}
}

void Player::Death()
{
	if (m_hp <= 0)
	{
		m_isDeath = true;
	}
}
