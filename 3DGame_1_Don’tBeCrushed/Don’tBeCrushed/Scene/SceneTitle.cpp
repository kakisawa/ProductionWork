#include "SceneTitle.h"
#include "SceneGame.h"
#include "DxLib.h"
#include "../Pad.h"
#include "../Camera.h"
#include "../Game.h"

namespace {
	const char* const kModelPlayer = "data/model/Title.mv1";
	const char* const kTitle = "data/graph/Title4.png";

	constexpr int kFadeMax = 255;
	constexpr float kCamera= 0.01f;
	constexpr float kScale = 0.01f;
}

SceneTitle::SceneTitle():
	m_graph(LoadGraph(kTitle)),			// タイトルロゴのロード
	m_model(MV1LoadModel(kModelPlayer)),// モデルのロード
	m_fadeAlpha(kFadeMax),
	m_scele(kScale),
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
		if (m_fadeAlpha >= 255)
		{
			m_isSceneEnd = true;
			m_fadeAlpha = 255;
		}
	}
	return shared_from_this();
}

void SceneTitle::Draw()
{
	// モデル描画
	MV1DrawModel(m_model);

	// タイトルロゴ描画
	DrawGraph(50, 20, m_graph, true);

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
