#include "EnemyLeft.h"
#include "EnemyState.h"
#include "../Player/Player.h"
#include "../../Util/Pad.h"
#include "../GameMap.h"
#include <math.h>
#include <cassert>

namespace {
	const char* kModelEnemy = "data/model/EnemyModel/Enemy2.mv1";
	const char* kSord = "data/model/EnemyModel/Blade.mv1";

	const char* kParts = "handIK.r";

	const char* const kUI[4]{
		"data/UI/GameScene/EnemyLeft/HPOurGauge.png",	// HPUI(外側)
		"data/UI/GameScene/EnemyLeft/HPInGauge.png",	// HPUI(内側)
		"data/UI/GameScene/EnemyLeft/NameBg.png",		// 名前背景UI
		"data/UI/GameScene/EnemyLeft/Shimane.png",		// 名前UI
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


	constexpr int	kMaxHp = 100;				// 体力最大値

	VECTOR kSordSize = VGet(0.01f, 0.01f, 0.01f);
	VECTOR kInitPos= VGet(-10.0f, 0.0f, 10.0f);

	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// ベクトルの初期化
	
	const VECTOR kUpPos = VGet(0.0f, 20.0f, 0.0f);
	const VECTOR kAttackRange = VGet(0.0f, 0.0f, 0.0f);
	constexpr float kColRadius = 5.0;
	constexpr float kAttackColRadius = 0.0;
	
}

EnemyLeft::EnemyLeft():
	EnemyBase(kModelEnemy,kInitPos),
	m_sordModel(-1),
	m_isWalk(false),
	m_upPos(kInitVec)
{
	m_sordModel = MV1LoadModel(kSord);

	// UI画像の読み込み
	for (int i = 0; i < m_uiGraph.size(); i++)
	{
		m_uiGraph[i] = LoadGraph(kUI[i]);
		assert(m_uiGraph[i] != -1);
	}

	m_pState = std::make_shared<EnemyState>();

	m_pState->AddState([=] { IdleStateUpdate(); }, [=] { IdleStateInit(); }, EnemyState::State::kIdle);
	m_pState->AddState([=] { WalkStateUpdate(); }, [=] { WalkStateInit(); }, EnemyState::State::kWalk);

	//初期ステイトセット
	m_pState->SetState(EnemyState::State::kIdle);	//ステイトセット(最初はIdle状態)
}

EnemyLeft::~EnemyLeft()
{
	MV1DeleteModel(m_sordModel);
}

void EnemyLeft::Init(std::shared_ptr<GameMap> pMap)
{
	mp.leftBack = pMap->GetMapLeftBack();
	mp.rightFront = pMap->GetMapRightFront();
}

void EnemyLeft::Update(const Player& player)
{
	m_pState->Update();

	m_pModel->Update();
	
	if (player.GetAttackLeft())
	{
		m_hp -= player.GetAddDamage();
		m_hp=max(m_hp, 0);
	}

	Move();

	SetModelFramePosition(m_pModel->GetModel(), kParts, m_sordModel);

	// 当たり判定用カプセル型の座標更新
	m_upPos = VAdd(m_pos, kUpPos);
	m_colSphere.UpdateCol(m_pos, m_upPos,kInitPos, kColRadius,kAttackColRadius);
}

void EnemyLeft::Draw()
{
	if (m_hp > 0)
	{
		// モデルの描画
		m_pModel->Draw();
		// 棒モデルの描画
		MV1DrawModel(m_sordModel);
	}

#ifdef _DEBUG
	if (m_hp > 0)
	{
		m_colSphere.DrawMain(0x00ff00, false);	// 当たり判定描画
	}
#endif
}

void EnemyLeft::End()
{
	// UI画像の削除
	for (int i = 0; i < m_uiGraph.size(); i++)
	{
		DeleteGraph(m_uiGraph[i]);
	}
}

void EnemyLeft::Move()
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
}

void EnemyLeft::UIDraw()
{
	// HPゲージ描画
	DrawExtendGraph(kHpGaugeUIPosX, kHpGaugeUIPosY,
		kHpGaugeUIPosX + (kHpGaugePosX * (m_hp * 0.01f)), kHpGaugePosY, m_uiGraph[1], true);
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
