#include "Player.h"
#include "DxLib.h"
#include "PlayerState.h"
#include "../Model.h"
#include "../GameMap.h"
#include "../../Util/Pad.h"
#include "../../Object/Camera.h"
#include "../../Util/Effect.h"
#include "../Enemy/EnemyRight.h"
#include "../Enemy/EnemyLeft.h"
#include "../../Manager/SoundManager.h"
#include <cmath>
#include <vector>
#include <cassert>

namespace {
	// UI画像
	const char* const kUI[6]{
		"data/Image/GameScene/UI/Player/HPOurGauge.png",// HPUI(外側)
		"data/Image/GameScene/UI/Player/HPInGauge.png",	// HPUI(内側)
		"data/Image/GameScene/UI/Player/NameBg.png",	// 名前背景UI
		"data/Image/GameScene/UI/Player/Fukuoka.png",	// 名前UI
		"data/Image/GameScene/UI/Player/Face.png",		// 顔アイコン
		"data/Image/GameScene/UI/Player/Target.png",	// 照準
	};

	const char* const kModelPlayer = "data/model/RogueHooded.mv1";	// モデルのファイル名

	constexpr int	kMaxHp = 100;				// 体力最大値
	constexpr int	kSordDamage = 20;			// 剣の攻撃力
	constexpr int	kBowDamage = 1;				// 弓の攻撃力

	constexpr float kInitAngle = -DX_PI_F / 2.0f * 90.0f;	// プレイヤーの初期角度*90(向きを反対にする)
	constexpr float kModelSize = 5.0f;			// モデルのサイズ
	constexpr float kSpeed = 0.7f;				// プレイヤー移動速度
	constexpr float kAttackSpeed = 0.5f;		// プレイヤー攻撃時の加速度
	constexpr float	kAngleSpeed = 0.2f;			// 角度変化速度
	constexpr float	kJumpPower = 1.8f;			// ジャンプ力
	constexpr float	kGravity = 0.05f;			// 重力
	constexpr float kEffectHeight = 10.0f;		// エフェクトを表示する高さ

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

	const VECTOR kUpPos = VGet(0.0f, 7.0f, 0.0f);		// プレイヤーの当たり判定上
	const VECTOR kAttackRange = VGet(0.0f, 0.0f,13.0f);	// プレイヤーの攻撃当たり判定
	constexpr float kColRadius = 2.5;					// プレイヤーの当たり判定半径
	constexpr float kAttackColRadius = 3.0;				// プレイヤーの攻撃当たり判定半径

	// 初期化用値
	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// ベクトルの初期化
	constexpr float kInitFloat = 0.0f;				// float値の初期化
	constexpr int kInitInt = 0;						// int値の初期化

	int waitTime = 0;								// 弓を連続で撃つ時のインターバル
}

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player() :
	m_angle(kInitFloat),
	m_gravity(kGravity),
	m_addDamage(kInitInt),
	m_jumpPower(kInitFloat),
	m_colRadius(kColRadius),
	m_multiAttack(kInitInt),
	m_hp(kMaxHp),
	m_isAttack(false), 
	m_isFirstAttack(false),
	m_isNextAttackFlag(false),
	m_isSordAttackDamage(false),
	m_isBowAttackDamage(false),
	m_isWalk(false),
	m_isJump(false),
	m_isDeath(false),
	m_isCol(false),
	m_isBodyCol(false),
	m_isSordAttackToLeftEnemy(false),
	m_isSordAttackToRightEnemy(false),
	m_isBowAttackToLeftEnemy(true),
	m_pos(kInitVec),
	m_move(kInitVec),
	m_UpPos(kInitVec),
	m_targetDir(kInitVec),
	m_vecToEnemy(kInitVec),
	m_attackRange(kInitVec),
	m_RightEnemyPos(kInitVec),
	m_LeftEnemyPos(kInitVec),
	m_attackKind(AttackKind::kNone)
{
	// UI画像の読み込み
	for (int i = 0; i < m_uiGraph.size(); i++){
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

	// ステイトクラスのインスタンス追加
	m_pState->AddState([=] { IdleStateUpdate(); }, [=] { IdleStateInit(); }, PlayerState::State::kIdle);					// 待機状態
	m_pState->AddState([=] { WalkStateUpdate(); }, [=] { WalkStateInit(); }, PlayerState::State::kWalk);					// 歩き状態
	m_pState->AddState([=] { JumpStateUpdate(); }, [=] { JumpStateInit(); }, PlayerState::State::kJump);					// ジャンプ状態
	m_pState->AddState([=] { AttackSordStateUpdate(); }, [=] { AttackSordStateInit(); }, PlayerState::State::kAttackSord);	// 剣攻撃状態
	m_pState->AddState([=] { AttackBowStateUpdate(); }, [=] { AttackBowStateInit(); }, PlayerState::State::kAttackBow);		// 弓攻撃状態

	//初期ステイトセット
	m_pState->SetState(PlayerState::State::kIdle);

	// サウンド
	m_pSound = new SoundManager;
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	delete m_pSound;
	m_pSound = nullptr;
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

	// マップの端
	mp.leftBack = pMap->GetMapLeftBack();
	mp.rightFront = pMap->GetMapRightFront();

	// サウンド
	m_pSound->Init();
	// 使用サウンドのロード
	m_pSound->LoadSE(SoundManager::SE_Type::kSordSE);
	m_pSound->LoadSE(SoundManager::SE_Type::kBowSE);

	// エフェクト
	m_pEffect->Init();
}


/// <summary>
/// 更新
/// </summary>
void Player::Update(const Camera& camera, const EnemyRight& enemyR, const EnemyLeft& enemyL)
{
	m_isCol = false;
	m_isBodyCol = false;
	// パッド入力によって移動パラメータを設定する
	VECTOR	upMoveVec;		// 方向ボタン「↑」を入力をしたときのプレイヤーの移動方向ベクトル
	VECTOR	leftMoveVec;	// 方向ボタン「←」を入力をしたときのプレイヤーの移動方向ベクトル

	m_RightEnemyPos = ConvWorldPosToScreenPos(enemyR.GetPos());
	m_LeftEnemyPos = ConvWorldPosToScreenPos(enemyL.GetPos());

	//現在の敵との距離を求める
	NearByEnemy(enemyR, enemyL);

	// 移動処理
	OldMoveValue(camera, upMoveVec, leftMoveVec);

	// プレイヤーの移動方向にモデルの方向を近づける
	Angle();

	// プレイヤーの状態更新
	// 攻撃処理
	Attack(enemyR, enemyL);

	// 座標更新
	Move(m_move, enemyR, enemyL);

	// 死亡確認処理
	Death();

	// 当たり判定の更新処理
	ColUpdate();

	// ステイトの更新
	m_pState->Update();

	// モデルの更新
	m_pModel->Update();

	// エフェクトの更新
	m_pEffect->Update();
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
	DrawGraph(kNameBgX, kNameBgY, m_uiGraph[2], true);		// 名前背景
	DrawGraph(kNameX, kNameY, m_uiGraph[3], true);			// 名前
	DrawGraph(kFaceUIPosX, kFaceUIPosY, m_uiGraph[4], true);// アイコン

	// モデルの描画
	m_pModel->Draw();

	// 照準UIの描画
	if (Pad::IsPress(PAD_INPUT_Z))
	{
		if(m_isBowAttackToLeftEnemy)
		{
			DrawExtendGraph(m_LeftEnemyPos.x - 50, m_LeftEnemyPos.y - 200,
				m_LeftEnemyPos.x + 50, m_LeftEnemyPos.y,
				m_uiGraph[5], true);
		}
		else {
			DrawExtendGraph(m_RightEnemyPos.x - 50, m_RightEnemyPos.y - 200,
				m_RightEnemyPos.x + 50, m_RightEnemyPos.y,
				m_uiGraph[5], true);
		}
	}


	// エフェクトの描画
	m_pEffect->Draw();

#ifdef _DEBUG
	m_colSphere.DrawMain(0xff0000, false);	// 当たり判定描画
	m_colSphere.DrawAttack(0x0000ff, false);	// 当たり判定描画

	DrawFormatString(0, 200, 0xffffff, "Player:m_move.x,y,z=%.2f,=%.2f,=%.2f", m_move.x, m_move.y, m_move.z);

	DrawFormatString(0, 180, 0xffffff, "Player:m_pos.x,y,z=%.2f,=%.2f,=%.2f", m_pos.x, m_pos.y, m_pos.z);
	DrawFormatString(0, 220, 0xffffff, "Player:m_hp=%d", m_hp);

	DrawFormatString(0, 280, 0xffffff, "State=%d", m_pState->GetState());
	DrawFormatString(0, 300, 0xffffff, "m_isWalk=%d", m_isWalk);
	DrawFormatString(0, 320, 0xffffff, "m_angle=%.2f", m_angle);

	DrawFormatString(0, 360, 0xffffff, "m_isLeftEnemyAttack=%d", m_isBowAttackToLeftEnemy);

	DrawFormatString(0, 420, 0xffffff, "m_animSpeed=%.2f", m_pModel->GetAnimSpeed());

	if(m_isBodyCol) {
		DrawString(0, 500, "当たっている", 0xffffff);
	}
	else{
		DrawString(0, 500, "当たっていない", 0xffffff);
	}

	if (m_isCol){
		DrawString(0, 520, "当たっている", 0xffffff);
	}
	else{
		DrawString(0, 520, "当たっていない", 0xffffff);
	}
#endif // DEBUG
}

/// <summary>
/// 終了
/// </summary>
void Player::End()
{
	// UI画像の削除
	for (int i = 0; i < m_uiGraph.size(); i++){
		DeleteGraph(m_uiGraph[i]);
	}

	// サウンドの解放
	m_pSound->ReleaseSound();
}

/// <summary>
/// 距離の近い敵を探す
/// </summary>
/// <param name="enemyR">右の敵</param>
/// <param name="enemyL">左の敵</param>
void Player::NearByEnemy(const EnemyRight& enemyR, const EnemyLeft& enemyL)
{
	// プレイヤーから右の敵までの距離
	double distance1 = sqrt((enemyR.GetPos().x - m_pos.x) * (enemyR.GetPos().x - m_pos.x) +
		(enemyR.GetPos().y - m_pos.y) * (enemyR.GetPos().y - m_pos.y)+
		(enemyR.GetPos().z - m_pos.z) * (enemyR.GetPos().z - m_pos.z));

	// プレイヤーから左の敵までの距離
	double distance2 = sqrt((enemyL.GetPos().x - m_pos.x) * (enemyL.GetPos().x - m_pos.x) +
		(enemyL.GetPos().y - m_pos.y) * (enemyL.GetPos().y - m_pos.y) +
		(enemyL.GetPos().z - m_pos.z) * (enemyL.GetPos().z - m_pos.z));

	// 距離が小さいほうの値を入れる
	double nearPos = min(distance1, distance2);

	// 右の敵の方が近かった場合
	if (nearPos == distance1)
	{
		// 右の敵が生存していたら
		if(enemyR.GetAlive())
		{
			// 弓の狙いを右の敵にする
			m_isBowAttackToLeftEnemy = false;
		}
		else { // 生きていなかったら
			// 弓の狙いを左の敵にする
			m_isBowAttackToLeftEnemy = true;
		}
	}
	else{ // 右の敵の方が近かったら
		
		// 左の敵が生存していたら
		if (enemyL.GetAlive()) {
			// 弓の狙いを左の敵にする
			m_isBowAttackToLeftEnemy = true;
		}
		else { // 生きていなかったら
			// 弓の狙いを右の敵にする
			m_isBowAttackToLeftEnemy = false;
		}
	}
}

/// <summary>
/// ジャンプステイトの初期化
/// </summary>
void Player::JumpStateInit()
{
	m_isJump = true;
	m_jumpPower = kJumpPower;
}

/// <summary>
/// 剣攻撃の初期化
/// </summary>
void Player::AttackSordStateInit()
{
	m_isAttack = true;
	m_multiAttack = 0;
	m_isNextAttackFlag = false;
	m_isFirstAttack = true;
	m_attackKind = AttackKind::kAttackSord;
}

/// <summary>
/// 弓攻撃の初期化
/// </summary>
void Player::AttackBowStateInit()
{
	m_isAttack = true;
	m_isNextAttackFlag = false;
	m_isFirstAttack = true;
	m_attackKind = AttackKind::kAttackBow;
}

/// <summary>
/// 待機状態の更新処理
/// </summary>
void Player::IdleStateUpdate()
{
	// アニメーションを待機モーションに変更
	m_pModel->ChangeAnim(m_animData.kIdle, true, false, 0.5f);
}

/// <summary>
/// 歩き状態の更新処理
/// </summary>
void Player::WalkStateUpdate()
{
	// アニメーションを歩きモーションに変更
	m_pModel->ChangeAnim(m_animData.kWalk, true, false, 0.5f);
}

/// <summary>
/// ジャンプ状態の更新処理
/// </summary>
void Player::JumpStateUpdate()
{
	// アニメーションをジャンプモーションに変更
	m_pModel->ChangeAnim(m_animData.kJump, false, false, 1.0f);
}

/// <summary>
/// 剣攻撃状態の更新
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

	if (Pad::IsTrigger(PAD_INPUT_X) && !m_isNextAttackFlag){
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
			m_isAttack = false;
			m_multiAttack = 0;
			m_pState->EndState();
		}

		// アニメーションが終わった段階で次の攻撃フラグがたっていたら
		if (m_isNextAttackFlag)
		{
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
			m_pSound->PlaySE(SoundManager::SE_Type::kBowSE, DX_PLAYTYPE_BACK);
			m_isBowAttackDamage = true;
			waitTime = 20;
		}
	}

	m_pModel->ChangeAnim(m_animData.kAttackBow, loop, false, 0.5f);

	// アニメーションが終わったら
	if (!loop)
	{
		m_isAttack = false;
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
	if (fabs(MoveVector.x) > 0.0f || fabs(MoveVector.z) > 0.0f){
		m_isWalk = true;
	}
	else{
		m_isWalk = false;
	}

	Collision enemyRightCol = enemyR.GetColSphere();
	Collision enemyLeftCol = enemyL.GetColSphere();

	if (m_colSphere.IsBodyCollision(enemyRightCol))
	{
		VECTOR colNormal = VNorm(VSub(m_pos, enemyR.GetPos()));
		m_pos = VAdd(m_pos, VScale(colNormal, 0.7f));
		m_isBodyCol = true;
	}
	if (m_colSphere.IsBodyCollision(enemyLeftCol))
	{
		VECTOR colNormal = VNorm(VSub(m_pos, enemyL.GetPos()));
		m_pos = VAdd(m_pos, VScale(colNormal, 0.7f));
		m_isBodyCol = true;
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

/// <summary>
/// 攻撃処理
/// </summary>
/// <param name="enemyR">右の敵情報</param>
/// <param name="enemyL">左の敵情報</param>
void Player::Attack(const EnemyRight& enemyR, const EnemyLeft& enemyL)
{
	// 剣攻撃
	Collision enemyRightCol = enemyR.GetColSphere();
	Collision enemyLeftCol = enemyL.GetColSphere();

	m_addDamage = 0;
	m_isSordAttackToLeftEnemy = false;
	m_isSordAttackToRightEnemy = false;
	
	if (m_colSphere.IsAttackCollision(enemyLeftCol))
	{
		m_isCol = true;
		m_isSordAttackToLeftEnemy = true;
	}
	if (m_colSphere.IsAttackCollision(enemyRightCol))
	{
		m_isCol = true;
		m_isSordAttackToRightEnemy = true;
	}

	if (m_isAttack && m_isCol) {
		if (m_pModel->GetAnimSpeed() >= 5.0f && m_pModel->GetAnimSpeed() < 6.0f)
		{
			m_pSound->PlaySE(SoundManager::SE_Type::kSordSE, DX_PLAYTYPE_BACK);
			m_isSordAttackDamage = true;
			m_pEffect->PlayDamageEffect(VGet(m_pos.x , m_pos.y + kEffectHeight, m_pos.z));
		}
	}

	if (m_isCol && m_isSordAttackDamage)
	{
		m_addDamage = kSordDamage;
		m_isSordAttackDamage = false;
	}	

	if (Pad::IsTrigger(PAD_INPUT_A)) {
		m_isNextAttackFlag = false;
	}

	// RBを押したら敵の方向を向くプログラム書け
	if (Pad::IsPress(PAD_INPUT_Z))
	{
		if (m_isBowAttackToLeftEnemy) {
			m_vecToEnemy = VSub(m_pos, enemyL.GetPos());
			m_angle = atan2(-m_vecToEnemy.x, -m_vecToEnemy.z);
		}
		else{
			m_vecToEnemy = VSub(m_pos, enemyR.GetPos());
			m_angle = atan2(-m_vecToEnemy.x, -m_vecToEnemy.z);
		}
		MV1SetRotationXYZ(m_pModel->GetModel(), VGet(0.0f, m_angle + DX_PI_F, 0.0f));

		// 弓攻撃
		if (m_isBowAttackDamage)
		{
			if (!m_isBowAttackToLeftEnemy)	// 右の敵にダメージ
			{
				m_pEffect->PlayerDamageBowEffect(VGet(enemyR.GetPos().x, enemyR.GetPos().y + 10, enemyR.GetPos().z ));
				m_addDamage = kBowDamage;		//  + enemyR.GetMovePos().x*10//+ enemyR.GetMovePos().z * 100
				m_isBowAttackDamage = false;
				m_isSordAttackToRightEnemy = true;
			}
			else{
				m_pEffect->PlayerDamageBowEffect(VGet(enemyL.GetPos().x, enemyL.GetPos().y + 10, enemyL.GetPos().z));
				m_addDamage = kBowDamage;
				m_isBowAttackDamage = false;
				m_isSordAttackToLeftEnemy = true;
			}
		}
	}
}

void Player::Jump()
{
	if (!m_isJump)	return;

	if (m_pos.y < 0.0f) {
		m_pState->EndState();
		m_isJump = false;
		m_pos.y = 0.0f;
		m_gravity = kGravity;
	}
	else if (m_pos.y >= 0.0f)
	{
		// ジャンプ状態なら重力適用
		if (m_isJump)
		{
			// Ｙ軸方向の速度を重力分減算する
			m_jumpPower -= m_gravity;
			m_gravity += 0.0002f;
		}
		// 移動ベクトルのＹ成分をＹ軸方向の速度にする
		m_move.y = m_jumpPower;
	}
}

void Player::Death()
{
	if (m_hp <= 0)
	{
		m_isDeath = true;
	}
}

void Player::ColUpdate()
{
	// プレイヤー当たり判定用カプセル型の座標更新
	m_UpPos = VAdd(m_pos, kUpPos);

	MATRIX rotationMatrix = MGetRotY(m_angle);
	m_attackRange = VAdd(m_pos, VTransform(kAttackRange, rotationMatrix));

	m_colSphere.UpdateCol(m_pos, m_UpPos, m_attackRange,
		kColRadius, kAttackColRadius);
}
