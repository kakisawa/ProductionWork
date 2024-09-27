#include "DxLib.h"
#include "Player.h"
#include "Stage.h"
#include "UIBattle.h"
#include "EffectManager.h"
#include "LoadData.h"
#include "DebugDraw.h"
#include "EnemyAbe.h"

// 定数
namespace
{
	// 敵情報
	const char* const kfileName = "data/Model/Chara/Abe.mv1";	// 敵のファイル名
	constexpr float kScale = 0.3f;								// 拡大率
	constexpr int kIntervalTime = 120;							// 状態を更新するまでの時間
}


/// <summary>
/// コンストラクタ
/// </summary>
EnemyAbe::EnemyAbe()
{
	// キャラクター情報を読み込む
	m_pLoadData = std::make_shared<LoadData>(*this, static_cast<int>(CharaType::kEnemyAbe));
	m_enemyType = static_cast<int>(CharaType::kEnemyAbe);
	m_pUIBattle = std::make_shared<UIBattle>(m_status.maxHp, m_enemyType);
	m_hp = m_status.maxHp;
	m_moveSpeed = m_status.maxMoveSpeed;
	m_modelHandle = MV1LoadModel(kfileName);
	MV1SetScale(m_modelHandle, VGet(kScale, kScale, kScale));
}


/// <summary>
/// デストラクタ
/// </summary>
EnemyAbe::~EnemyAbe()
{
	MV1DeleteModel(m_modelHandle);
}


/// <summary>
/// 初期化
/// </summary>
/// <param name="pos">初期位置</param>
void EnemyAbe::Init(std::shared_ptr<EffectManager> pEffect, VECTOR pos)
{
	m_pos = pos;
	m_pEffect = pEffect;
	MV1SetPosition(m_modelHandle, m_pos);
	m_pEffect->Init();	// エフェクトの初期化
}


/// <summary>
/// 更新
/// </summary>
/// <param name="player">プレイヤー参照</param>
/// <param name="stage">ステージ参照</param>
void EnemyAbe::Update(Player& player, Stage& stage, SceneStageBase& sceneStage)
{
	EnemyBase::Update(player, stage, sceneStage);
	m_pUIBattle->UpdateHpBar();		// HPバーの更新
}


/// <summary>
/// 描画
/// </summary>
void EnemyAbe::Draw()
{
	EnemyBase::Draw();
}


/// <summary>
/// UI描画
/// </summary>
void EnemyAbe::DrawUi()
{
	m_pUIBattle->DrawEnemySilhouette(static_cast<int>(CharacterBase::CharaType::kEnemyAbe)); // シルエット描画
	m_pUIBattle->DrawEnemyName(static_cast<int>(CharacterBase::CharaType::kEnemyAbe));		 // 敵の名前ゲージ表示
	m_pUIBattle->DrawEnemyHp(m_hp);	// HPゲージを表示
}
