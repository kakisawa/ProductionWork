#include "EnemyRight.h"
#include "EnemyState.h"
#include "../Player/Player.h"
#include "../GameMap.h"
#include "../../Util/Effect.h"
#include "../../Manager/SoundManager.h"
#include <cassert>
#include <math.h>

namespace {
	const char* kModelEnemy = "data/model/EnemyModel/Enemy1.mv1";
	const char* kSord = "data/model/EnemyModel/Blade.mv1";

	const char* kParts = "handIK.r";

	const char* const kUI[4]{
		"data/Image/GameScene/UI/EnemyRight/HPOurGauge.png",	// HPUI(外側)
		"data/Image/GameScene/UI/EnemyRight/HPInGauge.png",	// HPUI(内側)
		"data/Image/GameScene/UI/EnemyRight/NameBg.png",		// 名前背景UI
		"data/Image/GameScene/UI/EnemyRight/Shimane.png",		// 名前UI
	};

	// HPゲージ(外側)位置
	constexpr int kHpGaugeUIPosX = 650;
	constexpr int kHpGaugeUIPosY = 1020;

	// HPゲージ(内側)右側位置(Exted用右端座標)
	constexpr int kHpGaugePosX = 852;
	constexpr int kHpGaugePosY = kHpGaugeUIPosY + 42;

	// 名前背景位置
	constexpr int kNameBgX = 300;
	constexpr int kNameBgY = 1000;

	// 名前位置
	constexpr int kNameX = kNameBgX + 80;
	constexpr int kNameY = kNameBgY + 10;

	VECTOR kSordSize = VGet(0.01f, 0.01f, 0.01f);
	VECTOR kInitPos = VGet(40.0f, 0.0f, 100.0f);

	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// ベクトルの初期化

	constexpr float kInitAngle = -DX_PI_F / 2.0f * 90.0f;	// プレイヤーの初期角度*90(向きを反対にする)

	const VECTOR kUpPos = VGet(0.0f, 20.0f, 0.0f);
	const VECTOR kAttackRange = VGet(0.0f, 0.0f, 0.0f);
	constexpr float kColRadius = 5.0;
	constexpr float kColMoveRadius = kColRadius + 3.0f;
	constexpr float kAttackColRadius = 0.0;

	constexpr float kSpeed = 0.05f;
}

EnemyRight::EnemyRight() :
	EnemyBase(kModelEnemy, kInitPos),
	m_sordModel(-1),
	m_angle(0.0f),
	m_isWalk(false),
	m_isEffect(false),
	m_isMoveFlag(false),
	m_upPos(kInitVec),
	m_vecToPlayer(kInitVec),
	m_targetPos(kInitVec)
{
	m_sordModel = MV1LoadModel(kSord);

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

EnemyRight::~EnemyRight()
{
	MV1DeleteModel(m_sordModel);
	m_pEffect->ClearEffect();
}

void EnemyRight::Init(std::shared_ptr<GameMap> pMap)
{
	m_pEffect->Init();
}

void EnemyRight::Update(const Player& player)
{
	if (m_isAlive) {
		m_move = kInitVec;

		m_pState->Update();

		m_pModel->Update();

		m_targetPos = player.GetPos();

		if (player.GetAttackRight())
		{
			m_hp -= player.GetAddDamage();
			m_hp = max(m_hp, 0);
			if (m_hp <= 0)
			{
				m_isAlive = false;
				m_isEffect = true;
			}
		}

		Move(player);

		SetModelFramePosition(m_pModel->GetModel(), "handIK.r", m_sordModel);

		// 当たり判定用カプセル型の座標更新
		m_upPos = VAdd(m_pos, kUpPos);
		m_colSphere.UpdateCol(m_pos, m_upPos, kInitPos, kColRadius, kAttackColRadius);
	}
	else
	{
		m_colSphere.Init();

		if (m_isEffect) {
			m_pSound->PlaySE(SoundManager::SE_Type::kExtinction, DX_PLAYTYPE_BACK);
			m_pEffect->PlayDeathEffect(m_pos);
			m_isEffect = false;
		}
	}
	m_pEffect->Update();
}

void EnemyRight::Draw()
{
	if (m_isAlive)
	{
		// モデルの描画
		m_pModel->Draw();
		// 棒モデルの描画
		MV1DrawModel(m_sordModel);

	}
	else
	{
		m_pEffect->Draw();			 // エフェクト描画
	}

#ifdef _DEBUG
	if (m_isAlive)
	{
		m_colSphere.DrawMain(0x00ff00, false);	// 当たり判定描画
	}
#endif
}

void EnemyRight::End()
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

void EnemyRight::UIDraw()
{
	// HPゲージ描画
	DrawExtendGraph(kHpGaugeUIPosX, kHpGaugeUIPosY,
		kHpGaugeUIPosX + (kHpGaugePosX * (m_hp * 0.00333f)), kHpGaugePosY, m_uiGraph[1], true);
	DrawGraph(kHpGaugeUIPosX, kHpGaugeUIPosY, m_uiGraph[0], true);

	// 敵情報描画
	DrawGraph(kNameBgX, kNameBgY, m_uiGraph[2], true);
	DrawGraph(kNameX, kNameY, m_uiGraph[3], true);


#ifdef _DEBUG
	DrawFormatString(0, 700, 0xffffff, "m_pos.x=%.2f:.y=%.2f:.z=%.2f", m_pos.x, m_pos.y, m_pos.z);
	DrawFormatString(0, 720, 0xffffff, "Getm_pos.x=%.2f:.y=%.2f:.z=%.2f", m_targetPos.x, m_targetPos.y, m_targetPos.z);
	DrawFormatString(0, 740, 0xffffff, "m_vecToEnemy.x=%.2f:.y=%.2f:.z=%.2f", m_vecToPlayer.x, m_vecToPlayer.y, m_vecToPlayer.z);
#endif // DEBUG
}

void EnemyRight::Move(const Player& player)
{
	// 敵が画面外に出ないようする処理
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

	// ベクトルの正規化					// 移動用
	m_distance = VNorm(m_vecToPlayer);

	m_move.x = m_distance.x * -kSpeed;
	m_move.z = m_distance.z * -kSpeed;

	if (!m_isMoveFlag)
	{
		m_pos = VAdd(m_pos, m_move);
	}

	// atan2 を使用して角度を取得					// 方向用
	m_angle = atan2(m_vecToPlayer.x, m_vecToPlayer.z);

	// atan2 で取得した角度に３Ｄモデルを正面に向かせるための補正値( DX_PI_F )を
		// 足した値を３Ｄモデルの Y軸回転値として設定
	MV1SetRotationXYZ(m_pModel->GetModel(), VGet(0.0f, m_angle + DX_PI_F+ kInitAngle, 0.0f));
	MV1SetPosition(m_pModel->GetModel(), m_pos);
}

void EnemyRight::SetModelFramePosition(int ModelHandle, const char* FrameName, int SetModelHandle)
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

	MV1SetPosition(m_model, m_pos);
}

void EnemyRight::IdleStateUpdate()
{
	m_pModel->ChangeAnim(m_animData.kIdle, true, false, 0.5f);
}

void EnemyRight::WalkStateUpdate()
{
	m_pModel->ChangeAnim(m_animData.kWalk, true, false, 0.5f);
}
