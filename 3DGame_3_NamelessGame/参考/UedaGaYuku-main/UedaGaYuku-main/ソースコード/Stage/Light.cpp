#include "DxLib.h"
#include "Player.h"
#include "Light.h"

// 定数
namespace
{
	/*スポットライト*/
	constexpr float kSpotLightOutAngle = 45.0f;	 // スポットライトの影響角度
	constexpr float kSpotLightInAngle = 30.0f;	 // スポットライトの影響が減衰を始める角度
	constexpr float kSpotLightRange = 90.0f;	 // 有効距離
	constexpr float kSpotLighAtten0 = 0.5f;		 // 距離減衰パラメータ0
	constexpr float kSpotLighAtten1 = 0.01f;	 // 距離減衰パラメータ1
	constexpr float kSpotLighAtten2 = 0.1f;		 // 距離減衰パラメータ2
	const VECTOR kSpotLightPos = VGet(0.0f, 75.0f, -60.0f); // スポットライト位置
	const VECTOR kSpotLightDir = VGet(1.5f, -0.4f, 1.6f);	// スポットライト方向
}


/// <summary>
/// ライト作成
/// </summary>
void Light::Create(std::shared_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	m_spotLight = CreateSpotLightHandle(kSpotLightPos, kSpotLightDir, kSpotLightOutAngle, kSpotLightInAngle,
		kSpotLightRange, kSpotLighAtten0, kSpotLighAtten1, kSpotLighAtten2);
	m_dirLight = CreateDirLightHandle(VGet(0.0f, -1.0f, 0.0f));
}


/// <summary>
/// ライト削除
/// </summary>
void Light::Delete()
{
	DeleteLightHandleAll();
}


/// <summary>
/// ライト更新
/// </summary>
void Light::Update()
{
	const VECTOR cameraPos = GetCameraPosition();  // カメラ位置取得
	const VECTOR playerPos = m_pPlayer->GetPos();  // プレイヤー位置取得
	const VECTOR playerDir = VNorm(VSub(cameraPos, playerPos));  // カメラからプレイヤーの方向ベクトルを求める
	SetLightPositionHandle(m_spotLight, VAdd(playerPos, kSpotLightPos)); // スポットライト位置更新
}


#ifdef _DEBUG
/// <summary>
/// 描画
/// </summary>
void Light::Draw()
{
	DrawFormatString(0, 200, 0xffffff, "ライト位置(X:%.2f,Y:%.2f,Z;%.2f)", GetLightPositionHandle(m_spotLight).x, GetLightPositionHandle(m_spotLight).y, GetLightPositionHandle(m_spotLight).z);
	DrawFormatString(0, 220, 0xffffff, "ライト方向(X:%.2f,Y:%.2f,Z;%.2f)", GetLightDirectionHandle(m_spotLight).x, GetLightDirectionHandle(m_spotLight).y, GetLightDirectionHandle(m_spotLight).z);
}
#endif

