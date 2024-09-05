#include "EnemyRight.h"
#include "EnemyState.h"
#include "../Player/Player.h"

namespace {
	const char* kModelEnemy = "data/model/EnemyModel/Enemy1.mv1";
	const char* kSord = "data/model/EnemyModel/Blade.mv1";

	const char* kParts = "handIK.r";

	const char* const kHpOutGauge = "data/UI/EnemyGauge.png";			// HPゲージ外側UI
	const char* const kHpInGauge = "data/UI/EnemyRightGauge.png";		// HPゲージ内側UI

	constexpr int kHpGaugePosX = 500;
	constexpr int kHpGaugePosY = 890;

	VECTOR kSordSize = VGet(0.01f, 0.01f, 0.01f);
	VECTOR kInitPos = VGet(10.0f, 0.0f, 10.0f);

	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// ベクトルの初期化

	const VECTOR kUpPos = VGet(0.0f, 20.0f, 0.0f);
	const VECTOR kAttackRange = VGet(0.0f, 0.0f, 0.0f);
	constexpr float kColRadius = 5.0;
	constexpr float kAttackColRadius = 0.0;
}

EnemyRight::EnemyRight() :
	EnemyBase(kModelEnemy, kInitPos),
	m_sordModel(-1),
	m_upPos(kInitVec)
{
	m_sordModel = MV1LoadModel(kSord);

	m_pState = std::make_shared<EnemyState>();

	m_pState->AddState([=] { IdleStateUpdate(); }, [=] { IdleStateInit(); }, EnemyState::State::kIdle);
	m_pState->AddState([=] { WalkStateUpdate(); }, [=] { WalkStateInit(); }, EnemyState::State::kWalk);

	//初期ステイトセット
	m_pState->SetState(EnemyState::State::kIdle);	//ステイトセット(最初はIdle状態)
}

EnemyRight::~EnemyRight()
{
	MV1DeleteModel(m_sordModel);
}

void EnemyRight::Init(std::shared_ptr<GameMap> pMap)
{
	m_outGauge = LoadGraph(kHpOutGauge);
	m_inGauge = LoadGraph(kHpInGauge);
}

void EnemyRight::Update(const Player& player)
{
	m_pState->Update();

	m_pModel->Update();

	if (player.GetAttackRight())
	{
		m_hp -= player.GetAddDamage();
		m_hp = max(m_hp, 0);
	}


	// move
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
	}


	SetModelFramePosition(m_pModel->GetModel(), "handIK.r", m_sordModel);

	// 当たり判定用カプセル型の座標更新
	m_upPos = VAdd(m_pos, kUpPos);
	m_colSphere.UpdateCol(m_pos, m_upPos, kInitPos, kColRadius, kAttackColRadius);


}

void EnemyRight::Draw()
{
	if (m_hp > 0)
	{
		// モデルの描画
		m_pModel->Draw();
		// 棒モデルの描画
		MV1DrawModel(m_sordModel);
	}

	DrawExtendGraph(kHpGaugePosX, kHpGaugePosY,
		kHpGaugePosX + (852 * (m_hp * 0.01f)), (kHpGaugePosY + 42),
		m_inGauge, true);
	DrawGraph(kHpGaugePosX, kHpGaugePosY, m_outGauge, true);


#ifdef _DEBUG
	if (m_hp > 0)
	{
		m_colSphere.DrawMain(0x00ff00, false);	// 当たり判定描画
	}
#endif
}

void EnemyRight::End()
{
	DeleteGraph(m_inGauge);
	DeleteGraph(m_outGauge);
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
}

void EnemyRight::IdleStateUpdate()
{
	m_pModel->ChangeAnim(m_animData.kIdle, true, false, 0.5f);
}

void EnemyRight::WalkStateUpdate()
{
	m_pModel->ChangeAnim(m_animData.kWalk, true, false, 0.5f);
}
