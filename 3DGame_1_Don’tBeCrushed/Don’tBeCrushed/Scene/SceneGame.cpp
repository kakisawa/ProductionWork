#include "SceneGame.h"
#include "SceneGameClear.h"
#include "SceneGameOver.h"
#include "DxLib.h"
#include "../Enemy.h"
#include "../Player.h"
#include "../Stage.h"
#include "../Camera.h"
#include "../Pad.h"
#include "../Game.h"


namespace {
	constexpr int kEnemyNum = 4;					// 敵の数
	constexpr int kGameTime = 1200;					// 制限時間(現在は仮)
	constexpr int kFade = 255;						// フェード最大値

	constexpr float kEnemyPlaceX = 35.0f;			// 敵X座標
	constexpr float kEnemyPlaceZ = 24.0f;			// 敵Z座標
	constexpr float kEnemyRota = 1.58f;				// 敵回転量
	constexpr float kEnemyAddScaleX = 25.0f;		// 敵追加サイズ(調整用)
	constexpr float kEnemyAddScaleY = 50.0f;		// 敵追加サイズ(調整用)
	constexpr float kEnemyMove = 0.05f;				// 敵移動量
	constexpr float kEnemyAttackInterval = 240.0f;	// 敵攻撃の間隔

	constexpr float kEnemySizeWidth = 5.2f;			// 本のサイズ(背表紙分)
	constexpr float kEnemySizeHeight = 30.0f;		// 本のサイズ(縦の長さ)
	constexpr float kEnemySizeDepth = 19.0f;		// 本のサイズ(奥行/横幅)

}

SceneGame::SceneGame() :
	m_enemyInterval(0),
	m_enemyAttckNum(0),
	m_enemyNum(kEnemyNum),
	m_gameTime(kGameTime), 
	m_fadeAlpha(kFade),
	m_col(false),
	m_isFadeIn(false),
	m_isFadeOut(false),
	m_isSceneEnd(false),
	m_enemyAttackPos(VGet(0.0f, 0.0f, 0.0f))
{
	m_pEnemy.resize(m_enemyNum);

	// 敵の座標
	m_pEnemy[0] = make_shared<Enemy>(VGet(1.0f, 1.0f, kEnemyPlaceZ));	// 奥
	m_pEnemy[1] = make_shared<Enemy>(VGet(1.0f, 1.0f, -kEnemyPlaceZ));	// 前
	m_pEnemy[2] = make_shared<Enemy>(VGet(-kEnemyPlaceX, 1.0f, 2.0f));	// 左
	m_pEnemy[3] = make_shared<Enemy>(VGet(kEnemyPlaceX, 1.0f, 2.0f));	// 右

	// 敵モデル回転
	m_pEnemy[0]->SetModelAngle(VGet(0.0f, 0.0f, kEnemyRota));		// 奥
	m_pEnemy[1]->SetModelAngle(VGet(0.0f, 0.0f, kEnemyRota));		// 前
	m_pEnemy[2]->SetModelAngle(VGet(kEnemyRota, 0.0f, kEnemyRota));	// 左
	m_pEnemy[3]->SetModelAngle(VGet(kEnemyRota, 0.0f, kEnemyRota));	// 右

	// 敵のモデルサイズ調整
	m_pEnemy[0]->SetAddModelScale(kEnemyAddScaleX,kEnemyAddScaleY);
	m_pEnemy[1]->SetAddModelScale(kEnemyAddScaleX,kEnemyAddScaleY);
	m_pEnemy[2]->SetAddModelScale(kEnemyAddScaleX*2.7, 0.0f);
	m_pEnemy[3]->SetAddModelScale(kEnemyAddScaleX*2.7, 0.0f);

	// 敵の移動量
	m_pEnemy[2]->SetMove(VGet(0, 0, kEnemyMove));
	m_pEnemy[3]->SetMove(VGet(0, 0, kEnemyMove));

	// 敵が移動するかどうか
	m_pEnemy[2]->SetAddMove(true);
	m_pEnemy[3]->SetAddMove(true);
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	// カメラの初期化
	m_pCamera->Init_Game();
	// プレイヤーの初期化
	m_pPlayer->Init(m_pStage);
	// 敵の初期化
	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		m_pEnemy[i]->Init();
	}
	// ステージの初期化
	m_pStage->Init();
}

shared_ptr<SceneBase> SceneGame::Update()
{
	Pad::Update();		// Padの更新

	m_gameTime--;		// 制限時間カウント
	m_enemyInterval++;	// 敵が倒れてくるインターバル

	if (m_enemyInterval >= kEnemyAttackInterval)	// ４秒経ったらランダムで敵を倒す
	{	
		// 敵を倒す処理
		m_enemyAttckNum= GetRand(3);				// どの敵が倒れてくるかランダムで獲得する
		m_pEnemy[m_enemyAttckNum]->SetAttackNum(m_enemyAttckNum);	// 倒れる敵の番号を敵に渡す 
		m_pEnemy[m_enemyAttckNum]->SetAttack(true);					// 倒れる敵のフラグをtrueにする
		
		if (m_enemyAttckNum < 2)
		{
			int flag = false;
			m_enemyAttackPos = m_pEnemy[m_enemyAttckNum]->GetPos();	// 倒れてくる敵の座標を獲得する

			if (m_enemyAttckNum == 0) {
				flag = false;
			}
			else if (m_enemyAttckNum == 1) {
				flag = true;
			}

			// 倒れてくる敵の当たり判定を設定する
			m_colRect.SetLandscape(m_enemyAttackPos, kEnemySizeWidth, kEnemySizeHeight, kEnemySizeDepth, flag);	
		}

		if (m_enemyAttckNum > 1)
		{
			int flag = false;
			m_enemyAttackPos = m_pEnemy[m_enemyAttckNum]->GetPos();		// 倒れてくる敵の座標を獲得する

			if (m_enemyAttckNum == 2) {
				flag = false;
			}
			else if (m_enemyAttckNum == 3) {
				flag = true;
			}

			// 倒れてくる敵の当たり判定を設定する
			m_colRect.SetPortrait(m_enemyAttackPos, kEnemySizeWidth, kEnemySizeHeight, kEnemySizeDepth, flag);
		}
		m_enemyInterval = 0;
	}

	// 敵とプレイヤーの当たり判定
	if (m_pEnemy[m_enemyAttckNum]->GetFall())
	{
		// 当たり判定処理
		Collision playerRect = m_pPlayer->GetColRect();
		Collision enemyRect = m_colRect;
		if (playerRect.IsCollision(m_enemyAttckNum, m_colRect)){
			m_col = true;
			m_isFadeOut = true;
		}
	}
	else {
		m_col = false;
	}

	if (m_gameTime <= 0)
	{
		m_isFadeOut = true;
	}

	// プレイヤーの更新
	m_pPlayer->Update();
	// 敵の更新
	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		m_pEnemy[i]->Update();
	}

	// フェードイン
	if (!m_isFadeIn)
	{
		m_fadeAlpha -= 8;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
			m_isFadeIn = true;			// フェードインフラグをtrueにする
		}
	}

	// フェードアウト
	if (m_isFadeOut)
	{
		// フェードアウト完了後
		if (m_isSceneEnd)
		{
			if (!m_col)
			{
				return make_shared<SceneGameClear>();	// ゲームクリアシーンへ向かう
			}
			if (m_col)
			{
				return make_shared<SceneGameOver>();	// ゲームオーバーシーンへ向かう
			}
		}

		m_fadeAlpha += 8;
		if (m_fadeAlpha >= 255)
		{
			m_fadeAlpha = 255;
			m_isSceneEnd = true;
		}
	}

#ifdef _DEBUG
	// デバッグ用
	if (Pad::IsTrigger(PAD_INPUT_1))	// ゲームオーバー画面へ遷移
	{
		m_col = true;
		m_isFadeOut = true;
	}
	if (Pad::IsTrigger(PAD_INPUT_2))	// ゲームクリア画面へ遷移
	{
		m_isFadeOut = true;
	}
#endif // DEBUG
	return shared_from_this();
}

void SceneGame::Draw()
{
	m_pStage->Draw();	// ステージ描画

	// 敵当たり判定描画
	if (m_pEnemy[m_enemyAttckNum]->GetAttack()){
		m_colRect.EnemyDraw(0x000000, true);
	}

	// 敵描画
	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		m_pEnemy[i]->Draw();
	}
	// プレイヤー描画
	m_pPlayer->Draw();

	DrawString(0, 0, "SceneGame", 0xffffff);
	DrawFormatString(0, 20, 0xffffff, "制限時間=%d", m_gameTime/60);

	// フェードイン・フェードアウト描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// 半透明で表示開始
	DrawBoxAA(0, 0, kScreenWidth, kScreenHeight, 0x00000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);			// 不透明に戻しておく
#ifdef _DEBUG

#ifdef DEBUG
	DrawFormatString(0, 0, 0xffffff, "m_enemyAttckNum=%d", m_enemyAttckNum);
	DrawFormatString(0, 70, 0xffffff, "m_enemyInterval=%d", m_enemyInterval);
	DrawFormatString(0, 200, 0xffffff,"ifPos.x=%.2f:z=%.2f", m_enemyAttackPos.x, m_enemyAttackPos.z);
	DrawFormatString(0, 360, 0xffffff, "col=%d", col);
#endif // DEBUG

}

void SceneGame::End()
{
	m_pPlayer->End();

	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		m_pEnemy[i]->End();
	}
	m_pStage->End();
}