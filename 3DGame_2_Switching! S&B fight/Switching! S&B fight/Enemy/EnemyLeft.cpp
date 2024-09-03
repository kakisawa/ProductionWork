#include "EnemyLeft.h"
#include "EnemyState.h"
#include "../Player/Player.h"
#include "../Util/Pad.h"
#include <math.h>

namespace {
	const char* kModelEnemy = "data/model/EnemyModel/Enemy2.mv1";
	const char* kSord = "data/model/EnemyModel/Blade.mv1";

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

void EnemyLeft::Init()
{
}

void EnemyLeft::Update(const Player& player)
{
	m_pState->Update();

	m_pModel->Update();
	
	if (player.GetAttackLeft())
	{
		m_hp -= player.GetAddDamage();
	}

	SetModelFramePosition(m_pModel->GetModel(), "handIK.r", m_sordModel);

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

		m_colSphere.DrawMain(0x00ff00, false);	// 当たり判定描画
	}

#ifdef _DEBUG
	

	DrawFormatString(0, 260, 0xffffff, "EnemyLeft:m_hp=%d", m_hp);
#endif
}

void EnemyLeft::End()
{
}

void EnemyLeft::SetModelFramePosition(int ModelHandle, char *FrameName, int SetModelHandle)
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
