#include "SceneTitle.h"
#include "SceneGame.h"
#include "DxLib.h"
#include "../Pad.h"
#include "../Camera.h"
#include "../Game.h"

namespace {
	const char* const kModelPlayer = "data/model/Title.mv1";	// モデル
	const char* const kTitle = "data/graph/Title6.png";			// タイトル
	const char* const kSelect1 = "data/graph/GameStart.png";	// セレクト1(ゲームスタート)
	const char* const kSelect2 = "data/graph/Instructions.png";	// セレクト2(操作説明)
	const char* const kSelect3 = "data/graph/Finish.png";		// セレクト3(ゲーム終了)

	constexpr int kFadeMax = 255;		// フェード値MAX
	constexpr float kCamera= 0.01f;		// カメラの回転量
	constexpr float kScale = 0.01f;		// モデルサイズ

	constexpr int kTitlePosX = 50;		// タイトルX座標
	constexpr int kTitlePosY = 20;		// タイトルY座標

	constexpr int kSelect1PosX = 100;	// セレクト1X座標
	constexpr int kSelect2PosX = 700;	// セレクト2X座標
	constexpr int kSelect3PosX = 1300;	// セレクト3X座標
	constexpr int kSelectPosY = 780;	// セレクトY座標
}

SceneTitle::SceneTitle():
	m_graph(LoadGraph(kTitle)),			// タイトルロゴのロード
	m_select(LoadGraph(kSelect1)),
	m_select2(LoadGraph(kSelect2)),
	m_select3(LoadGraph(kSelect3)),
	m_model(MV1LoadModel(kModelPlayer)),// モデルのロード
	m_fadeAlpha(kFadeMax),
	m_scele(kScale),
	m_angle(0.0f),
	m_isFadeIn(true),
	m_isFadeOut(false),
	m_isSceneEnd(false),
	m_pos(VGet(0.0f, -5.0f, -13.0f))
{
	MV1SetScale(m_model, VGet(m_scele, m_scele, m_scele));	// モデルのサイズ設定
	MV1SetPosition(m_model, m_pos);							// モデルの座標設定

	// カメラの初期化
	m_pCamera->Init_Title(m_pos);
}

SceneTitle::~SceneTitle()
{
	// 画像削除
}

shared_ptr<SceneBase> SceneTitle::Update()
{
	Pad::Update();	// パッドの更新

	m_pCamera->AddCameraAngle(kCamera);	// カメラの回転量追加
	m_pCamera->Update();// カメラの更新


	m_angle++;
	float angle2 = m_angle * (DX_PI_F / 180.0f);


	// フェードイン
	if (m_isFadeIn)
	{
		m_fadeAlpha -= 8;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
			m_isFadeIn = false;
		}
	}
	if (Pad::IsTrigger(PAD_INPUT_10))	// もしSpaceキーが押されたら
	{									// フェードアウトを始める
		m_isFadeOut = true;
		m_isFadeIn = false;
	}

	// フェードアウト
	if (m_isFadeOut)
	{
		if (m_isSceneEnd)
		{
			return make_shared<SceneGame>();	// ゲームシーンへ行く
		}
		m_fadeAlpha += 8;
		if (m_fadeAlpha >= kFadeMax)
		{
			m_isSceneEnd = true;
			m_fadeAlpha = kFadeMax;
		}
	}
	return shared_from_this();
}

void SceneTitle::Draw()
{
	// モデル描画
	MV1DrawModel(m_model);

	// タイトルロゴ描画
	DrawGraph(kTitlePosX, kTitlePosY, m_graph, true);

	// セレクト描画
	DrawGraph(kSelect1PosX, kSelectPosY, m_select,true);
	DrawGraph(kSelect2PosX, kSelectPosY, m_select2, true);
	DrawGraph(kSelect3PosX, kSelectPosY, m_select3, true);

	//angle2
	
	// フェードイン・フェードアウト描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// 半透明で表示開始
	DrawBox(0, 0, kScreenWidth, kScreenHeight, 0x00000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく

#ifdef DEBUG
	DrawString(0, 0, "SceneTitle", 0xffffff);
#endif // DEBUG
}

void SceneTitle::End()
{
	// モデルのアンロード
	MV1DeleteModel(m_model);
	m_model = -1;
	// タイトルロゴの削除
	DeleteGraph(m_graph);
}
