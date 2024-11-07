#include "EnemyLeft.h"
#include "EnemyState.h"
#include "../Player/Player.h"
#include "../../Util/Pad.h"
#include "../GameMap.h"
#include "../../Util/Effect.h"
#include "../../Manager/SoundManager.h"
#include <math.h>
#include <cassert>

namespace {
	const char* kModelEnemy = "data/model/EnemyModel/Enemy2.mv1";
	const char* kSord = "data/model/EnemyModel/Blade.mv1";

	const char* kParts = "handIK.r";

	const char* const kUI[4]{
		"data/Image/GameScene/UI/EnemyLeft/HPOurGauge.png",	// HPUI(外側)
		"data/Image/GameScene/UI/EnemyLeft/HPInGauge.png",	// HPUI(内側)
		"data/Image/GameScene/UI/EnemyLeft/NameBg.png",		// 名前背景UI
		"data/Image/GameScene/UI/EnemyLeft/Shimane.png",	// 名前UI
	};

	// HPゲージ(外側)位置
	constexpr int kHpGaugeUIPosX = 650;
	constexpr int kHpGaugeUIPosY = 950;

	// HPゲージ(内側)右側位置(Exted用右端座標)
	constexpr int kHpGaugePosX = 852;
	constexpr int kHpGaugePosY = kHpGaugeUIPosY + 42;

	// 名前背景位置
	constexpr int kNameBgX = 300;
	constexpr int kNameBgY = 930;

	// 名前位置
	constexpr int kNameX = kNameBgX + 80;
	constexpr int kNameY = kNameBgY + 10;

	constexpr float kInitAngle = -DX_PI_F / 2.0f * 90.0f;	// プレイヤーの初期角度*90(向きを反対にする)

	constexpr int	kMaxHp = 100;				// 体力最大値

	VECTOR kSordSize = VGet(0.01f, 0.01f, 0.01f);
	VECTOR kInitPos= VGet(-40.0f, 0.0f, 100.0f);

	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// ベクトルの初期化
	
	const VECTOR kUpPos = VGet(0.0f, 20.0f, 0.0f);
	const VECTOR kAttackRange = VGet(0.0f, 0.0f, 0.0f);
	constexpr float kColRadius = 5.0;
	constexpr float kColMoveRadius = kColRadius+3.0f;	
	constexpr float kAttackColRadius = 0.0;

	constexpr float kSpeed = 0.1f;
}

EnemyLeft::EnemyLeft() :
	EnemyBase(kModelEnemy, kInitPos),
	m_sordModel(-1),
	m_isWalk(false),
	m_isEffect(false),
	m_isMoveFlag(false),
	m_angle(0.0f),
	m_upPos(kInitVec),
	m_vecToPlayer(kInitVec),
	m_targetPos(kInitVec),
	m_distance(kInitVec)
{
	m_sordModel = MV1LoadModel(kSord);
	m_pos = m_pModel->GetPos();

	// UI画像の読み込み
	for (int i = 0; i < m_uiGraph.size(); i++)
	{
		m_uiGraph[i] = LoadGraph(kUI[i]);
		assert(m_uiGraph[i] != -1);
	}

	m_isAlive = true;

	m_pState = std::make_shared<EnemyState>();
	m_pEffect = std::make_shared<Effect>();

	m_pState->AddState([=] { IdleStateUpdate(); }, [=] { IdleStateInit(); }, EnemyState::State::kIdle);
	m_pState->AddState([=] { WalkStateUpdate(); }, [=] { WalkStateInit(); }, EnemyState::State::kWalk);

	//初期ステイトセット
	m_pState->SetState(EnemyState::State::kIdle);	//ステイトセット(最初はIdle状態)

	m_pSound = new SoundManager;
	m_pSound->Init();
	m_pSound->LoadSE(SoundManager::SE_Type::kExtinction);
}

EnemyLeft::~EnemyLeft()
{
	MV1DeleteModel(m_sordModel);
	m_pEffect->ClearEffect();
}

void EnemyLeft::Init(std::shared_ptr<GameMap> pMap)
{
	mp.leftBack = pMap->GetMapLeftBack();
	mp.rightFront = pMap->GetMapRightFront();

	m_pEffect->Init();
}

void EnemyLeft::Update(const Player& player)
{
	if (m_isAlive)
	{
		m_move = kInitVec;

		m_pState->Update();

		m_pModel->Update();

		m_targetPos = player.GetPos();

		if (player.GetAttackLeft())
		{
			m_hp -= player.GetAddDamage();
			m_hp = max(m_hp, 0);
			if (m_hp <= 0)
			{
				m_isEffect = true;
				m_isAlive = false;
			}
		}

		Move(player);

		SetModelFramePosition(m_pModel->GetModel(), kParts, m_sordModel);

		// 当たり判定用カプセル型の座標更新
		m_upPos = VAdd(m_pos, kUpPos);
		m_colSphere.UpdateCol(m_pos, m_upPos, kInitPos, kColRadius, kAttackColRadius);
	}
	else
	{
		m_colSphere.Init();

		if (m_isEffect)
		{
			m_pSound->PlaySE(SoundManager::SE_Type::kExtinction, DX_PLAYTYPE_BACK);
			m_pEffect->PlayDeathEffect(m_pos);
			m_isEffect = false;
		}
	}

	m_pEffect->Update();
}

void EnemyLeft::Draw()
{
	if (m_isAlive)
	{
		// モデルの描画
		m_pModel->Draw();
		// 棒モデルの描画
		MV1DrawModel(m_sordModel);
	}

	m_pEffect->Draw();			 // エフェクト描画
	
#ifdef _DEBUG
	////DrawFormatString(0, 800, 0xffffff, "m_pos=%.2f:=%.2f:=%.2f", m_pos.x, m_pos.y, m_pos.z);
	//if (m_isAlive)
	//{
	//	DrawFormatString(0, 600, 0xffffff, "m_hp=%d", m_hp);
	//	m_colSphere.DrawMain(0x00ff00, false);	// 当たり判定描画
	//}
	//
	//DrawFormatString(0, 660, 0xffffff, "m_isMoveFlag=%d", m_isMoveFlag);
	//DrawFormatString(0, 680, 0xffffff, "m_vecToPlayer.x=%.2f.y=%.2f.z=%.2f", m_vecToPlayer.x,m_vecToPlayer.y,m_vecToPlayer.z);
#endif
}

void EnemyLeft::End()
{
	// UI画像の削除
	for (int i = 0; i < m_uiGraph.size(); i++)
	{
		DeleteGraph(m_uiGraph[i]);
	}

	m_pSound->ReleaseSound();

	delete m_pSound;
	m_pSound = nullptr;
}

void EnemyLeft::Move(const Player& player)
{
	//敵が画面外に出ないようする処理
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

	// プレイヤーの当たり判定カプセルと敵の当たり判定カプセル
	m_isMoveFlag = HitCheck_Capsule_Capsule(player.GetPos(), player.GetPos(),
		player.GetColRadius(),
		m_pos, m_pos, kColMoveRadius);

	m_vecToPlayer = VSub(m_pos, m_targetPos);

	// ベクトルの正規化
	m_distance = VNorm(m_vecToPlayer);

	m_move.x = m_distance.x * -kSpeed;
	m_move.z = m_distance.z * -kSpeed;

	if (!m_isMoveFlag)
	{
		m_pos = VAdd(m_pos, m_move);
	}

	// atan2 を使用して角度を取得						// 方向用
	m_angle = atan2(m_vecToPlayer.x, m_vecToPlayer.z);

	// atan2 で取得した角度に３Ｄモデルを正面に向かせるための補正値( DX_PI_F )を
		// 足した値を３Ｄモデルの Y軸回転値として設定
	MV1SetRotationXYZ(m_pModel->GetModel(), VGet(0.0f, m_angle + DX_PI_F + kInitAngle, 0.0f));
	MV1SetPosition(m_pModel->GetModel(), m_pos);
}

void EnemyLeft::UIDraw()
{
	// HPゲージ描画
	DrawExtendGraph(kHpGaugeUIPosX, kHpGaugeUIPosY,
		kHpGaugeUIPosX + (kHpGaugePosX * (m_hp * 0.00333f)), kHpGaugePosY, m_uiGraph[1], true);
	DrawGraph(kHpGaugeUIPosX, kHpGaugeUIPosY, m_uiGraph[0], true);

	// 敵情報描画
	DrawGraph(kNameBgX, kNameBgY, m_uiGraph[2], true);
	DrawGraph(kNameX, kNameY, m_uiGraph[3], true);
}

void EnemyLeft::SetModelFramePosition(int ModelHandle, const char *FrameName, int SetModelHandle)
{
	MATRIX FrameMatrix;
	int FrameIndex;

	// フレーム名からフレーム番号を取得する
	FrameIndex = MV1SearchFrame(ModelHandle, FrameName);

	// フレームの現在のワールドでの状態を示す行列を取得する
	FrameMatrix = MV1GetFrameLocalWorldMatrix(ModelHandle, FrameIndex);

	// セットするモデルの状態を示す行列をフレームの状態を示す行列と同じにする
	// MV1SetMatrix(SetModelHandle, FrameMatrix);
	MV1SetMatrix(SetModelHandle, MMult(MGetScale(kSordSize), FrameMatrix));
}

void EnemyLeft::IdleStateUpdate()
{
	m_pModel->ChangeAnim(m_animData.kIdle, true, false, 0.5f);
}

void EnemyLeft::WalkStateUpdate()
{
	m_pModel->ChangeAnim(m_animData.kWalk, true, false, 0.5f);
}
