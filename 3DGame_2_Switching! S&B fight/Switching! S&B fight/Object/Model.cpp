#include "Model.h"
#include <cassert>
#include "DxLib.h"

namespace {

	// アニメーション関係
	constexpr float kAnimChangeFrame = 8.0f;		// アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeRateSpeed = 0.2f;	// アニメーション切り替えにかかる速度
	constexpr float kAnimBlendAdd = 0.5f;			// アニメーションブレンドの増加値
	constexpr float kAnimBlendMax = 1.0f;			// アニメーションの切り替えにかかる最大フレーム数


	float rate = 0.0f;
}

Model::Model(const char* fileName) :
	m_model(MV1LoadModel(fileName)),
	m_animChangeFrame(0),
	m_animChangeFrameTotal(0),
	m_animSpeed(0.0f),
	m_animTime(0.0f)
{
	//コピーに失敗した場合
	assert(m_model != -1);

	// アニメーション状態の初期化
	InitAnim(m_prev);
	InitAnim(m_current);
}

Model::~Model()
{
	MV1DeleteModel(m_model);	// モデル情報の削除
}

void Model::Init()
{

}

void Model::Update()
{
	// モデルのアップデート
	m_animSpeed++;
	if (m_animSpeed >= m_animChangeFrameTotal)
	{
		UpdateAnimation(m_prev);
		UpdateAnimation(m_current);
		m_animSpeed = 0.0f;
	}

	// 指定フレームに掛けてアニメーションを変更する
	m_animChangeFrame += kAnimChangeRateSpeed;
	if (m_animChangeFrame >= m_animChangeFrameTotal)
	{
		m_animChangeFrame = m_animChangeFrameTotal;
	}

	// アニメーションのブレンド率を変更する
	UpdateAnimBlendRate();
}

void Model::Draw()
{
	// モデルの描画
	MV1DrawModel(m_model);


#ifdef _DEBUG
	DrawFormatString(0, 700, 0xffffff, "m_current.animNo=%d", m_current.animNo);
	DrawFormatString(0, 720, 0xffffff, "m_current.attachNo=%d", m_current.attachNo);
	DrawFormatString(0, 740, 0xffffff, "m_current.elapsedTime=%.2f", m_current.elapsedTime);
	DrawFormatString(0, 760, 0xffffff, "m_current.totalTime=%.2f", m_current.totalTime);

	DrawFormatString(0, 780, 0xffffff, "m_prev.animNo=%d", m_prev.animNo);
	DrawFormatString(0, 800, 0xffffff, "m_prev.attachNo=%d", m_prev.attachNo);
	DrawFormatString(0, 820, 0xffffff, "m_prev.elapsedTime=%.2f", m_prev.elapsedTime);
	DrawFormatString(0, 840, 0xffffff, "m_prev.totalTime=%.2f", m_prev.totalTime);
	DrawFormatString(0, 860, 0xffffff, "rate=%.2f", rate);
	DrawFormatString(0, 880, 0xffffff, "m_animChangeFrame=%.2f", m_animChangeFrame);
	DrawFormatString(0, 900, 0xffffff, "m_animSpeed=%.2f", m_animSpeed);
	
#endif // DEBUG
}

void Model::SetSize(const VECTOR& size)
{
	MV1SetScale(m_model, size);	// プレイヤーの初期サイズ
}

void Model::SetRota(const VECTOR& rota)
{
	MV1SetRotationXYZ(m_model, rota);		// プレイヤーの初期角度
}

void Model::SetPos(const VECTOR& pos)
{
	m_pos = pos;
	MV1SetPosition(m_model, pos);									// プレイヤーの初期位置	
}

void Model::SetAnim(int animNo, bool isLoop, bool isForceChange)
{
	// アニメーションを再生させるか
	if (!isForceChange)
	{
		// 既に再生されているアニメーションは再生しない
		if (m_current.animNo == animNo)	return;
	}

	// 前のアニメーションが残っていたら終了させる
	if (m_prev.attachNo != -1)
	{
		MV1DetachAnim(m_model, m_prev.attachNo);	// 前のアニメーションを外す
		InitAnim(m_prev);							// 前のアニメーション情報を初期化させる
	}
	if (m_current.attachNo != -1)
	{
		MV1DetachAnim(m_model, m_current.attachNo);	// 現在のアニメーションを外す
		InitAnim(m_current);						// 現在のアニメーション情報を初期化させる
	}

	// 新しくアニメーションを設定する
	m_current.animNo = animNo;
	m_current.attachNo = MV1AttachAnim(m_model, animNo, -1, false);
	m_current.totalTime = MV1GetAttachAnimTotalTime(m_model, m_current.attachNo);
	m_current.isLoop = isLoop;

	// 変更にかけるフレーム数を覚えておく
	m_animChangeFrameTotal = 1;
	m_animChangeFrame = 1;
}

void Model::ChangeAnim(int animNo, bool isLoop, bool isForceChange, bool isChangeFrame)
{
	// アニメーションを再生させるか
	if (!isForceChange)
	{
		// 既に再生されているアニメーションは再生しない
		if (m_current.animNo == animNo)	return;
	}

	// 前のアニメーションが残っていたら終了させる
	if (m_prev.attachNo != -1)
	{
		MV1DetachAnim(m_model, m_prev.attachNo);	// 前のアニメーションを外す
		InitAnim(m_prev);							// 前のアニメーション情報を初期化させる
	}

	// 現在再生中のアニメーションを前のアニメーションに移行する
	m_prev = m_current;

	 // 新しいアニメーションを設定
    m_current.animNo = animNo;
    m_current.attachNo = MV1AttachAnim(m_model, animNo, -1, false);
    m_current.totalTime = MV1GetAttachAnimTotalTime(m_model, m_current.attachNo);
    m_current.isLoop = isLoop;

	// 変更にかけるフレーム数を覚えておく
	m_animChangeFrameTotal = isChangeFrame;
	m_animChangeFrame = 0;

	// アニメーションのブレンド率を設定する
	UpdateAnimBlendRate();
}

bool Model::IsAnimEnd()
{
	// Loopアニメーションの場合は常にfalseを返す
	if (m_current.isLoop) return false;

	m_animTime = MV1GetAttachAnimTime(m_model, m_current.attachNo);
	if (m_animTime >= m_current.totalTime){
		return true;
	}

	return false;
}

void Model::InitAnim(AnimData& anim)
{
	anim.animNo = -1;
	anim.attachNo = -1;
	anim.totalTime = 0.0f;
	anim.elapsedTime = 0.0f;
	anim.isLoop = false;
}

void Model::UpdateAnimation(AnimData anim, float dt)
{
	// アニメーションが設定されていない場合は何もしない
	if (anim.attachNo == -1)	return;

	// アニメーションの更新
	float time = MV1GetAttachAnimTime(m_model, anim.attachNo);
	time += dt;
	if (time >= anim.totalTime)
	{
		if (anim.isLoop)
		{
			// アニメーションのループ
			time -= anim.totalTime;
		}
		else {
			time = anim.totalTime;
		}
	}
	MV1SetAttachAnimTime(m_model, anim.attachNo, time);
}

void Model::UpdateAnimBlendRate()
{
	// アニメーション変化のフレーム数に応じたブレンド率を設定する
	rate = static_cast<float>(m_animChangeFrame) / static_cast<float>(m_animChangeFrameTotal);
	if (rate > kAnimBlendMax)	rate = kAnimBlendMax;

	// アニメーションのブレンド率を設定する
	MV1SetAttachAnimBlendRate(m_model, m_prev.animNo, kAnimBlendMax - rate);
	// アニメーションのブレンド率を設定する
	MV1SetAttachAnimBlendRate(m_model, m_current.animNo);

//ここ修正
}
