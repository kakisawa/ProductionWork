#include "DxLib.h"
#include "Player.h"
#include "Stage.h"
#include "UIBattle.h"
#include "EffectManager.h"
#include "LoadData.h"
#include "DebugDraw.h"
#include "EnemyNinja.h"

// 定数
namespace
{
	// 敵情報
	const char* const kfileName = "data/Model/Chara/Ninja.mv1";	// 敵のファイル名
	constexpr float kScale = 0.3f;								// 拡大率
	constexpr int kIntervalTime = 180;							// 状態を更新するまでの時間
	const VECTOR kInitPos = VGet(2660, 69.0f, 4280.0f);			// 初期位置
}

/// <summary>
/// コンストラクタ
/// </summary>
EnemyNinja::EnemyNinja()
{
	// キャラクター情報を読み込む
	m_pLoadData = std::make_shared<LoadData>(*this, static_cast<int>(CharaType::kEnemyNinja));
	m_enemyType = static_cast<int>(CharaType::kEnemyNinja);
	m_pUIBattle = std::make_shared<UIBattle>(m_status.maxHp, m_enemyType);
	m_hp = m_status.maxHp;
	m_pos = kInitPos;
	m_moveSpeed = m_status.maxMoveSpeed;
	m_modelHandle = MV1LoadModel(kfileName);
	MV1SetScale(m_modelHandle, VGet(kScale, kScale, kScale));
	MV1SetPosition(m_modelHandle, kInitPos);
}


/// <summary>
/// デストラクタ
/// </summary>
EnemyNinja::~EnemyNinja()
{
	MV1DeleteModel(m_modelHandle);
}


/// <summary>
///  初期化
/// </summary>
/// <param name="pos">初期位置</param>
void EnemyNinja::Init(std::shared_ptr<EffectManager> pEffect, VECTOR pos)
{
	m_pos = pos;
	m_pEffect = pEffect;
	MV1SetPosition(m_modelHandle, m_pos);
	m_pEffect->Init();	// エフェクトの初期化
	//m_pUIBattle->SetEnemyKind(static_cast<int>(CharaType::kEnemyNinja));
}


/// <summary>
/// 更新
/// </summary>
/// <param name="player">プレイヤー参照</param>
/// <param name="stage">ステージ参照</param>
void EnemyNinja::Update(Player& player, Stage& stage, SceneStageBase& sceneStage)
{
	EnemyBase::Update(player, stage, sceneStage);
	m_pUIBattle->UpdateHpBar();		// HPバーの更新
}


/// <summary>
/// 描画
/// </summary>
void EnemyNinja::Draw()
{
	EnemyBase::Draw();
}


/// <summary>
/// UI描画
/// </summary>
void EnemyNinja::DrawUi()
{
	m_pUIBattle->DrawEnemySilhouette(static_cast<int>(CharacterBase::CharaType::kEnemyNinja)); // シルエット描画
	m_pUIBattle->DrawEnemyName(static_cast<int>(CharacterBase::CharaType::kEnemyNinja));	   // 敵の名前ゲージ表示
	m_pUIBattle->DrawEnemyHp(m_hp);	// HPゲージを表示
}
