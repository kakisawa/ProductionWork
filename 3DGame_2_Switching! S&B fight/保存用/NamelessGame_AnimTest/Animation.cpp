#include "Animation.h"
#include "DxLib.h"

namespace {

	// アニメーション関係
	constexpr float kAnimChangeFrame = 8.0f;		// アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeRateSpeed = 0.2f;	// アニメーション切り替えにかかる速度
	constexpr float kAnimBlendAdd = 0.5f;			// アニメーションブレンドの増加値
	constexpr float kAnimBlendMax = 1.0f;			// アニメーションの切り替えにかかる最大フレーム数
}

Animation::Animation():
	m_animChangeFrame(0),
	m_animChangeFrameTotal(0),
	m_animSpeed(0.0f)
{
}

Animation::~Animation()
{
}

void Animation::Init()
{
	// アニメーション状態の初期化
	InitAnim(m_prev);
	InitAnim(m_current);
}

void Animation::Update(const int& model)
{
	// モデルのアップデート
	m_animSpeed++;
	if (m_animSpeed >= m_animChangeFrameTotal)
	{
		UpdateAnimation(model,m_prev);
		UpdateAnimation(model, m_current);
		m_animSpeed = 0.0f;
	}

	// 指定フレームに掛けてアニメーションを変更する
	m_animChangeFrame += kAnimChangeRateSpeed;
	if (m_animChangeFrame >= m_animChangeFrameTotal)
	{
		m_animChangeFrame = m_animChangeFrameTotal;
	}

	// アニメーションのブレンド率を変更する
	UpdateAnimBlendRate(model);
}

void Animation::Draw()
{
}

void Animation::SetAnim(const int& model,int animNo, bool isLoop, bool isForceChange)
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
		MV1DetachAnim(model, m_prev.attachNo);	// 前のアニメーションを外す
		InitAnim(m_prev);							// 前のアニメーション情報を初期化させる
	}
	if (m_current.attachNo != -1)
	{
		MV1DetachAnim(model, m_current.attachNo);	// 現在のアニメーションを外す
		InitAnim(m_current);						// 現在のアニメーション情報を初期化させる
	}

	// 新しくアニメーションを設定する
	m_current.animNo = animNo;
	m_current.attachNo = MV1AttachAnim(model, animNo, -1, false);
	m_current.totalTime = MV1GetAttachAnimTotalTime(model, m_current.attachNo);
	m_current.isLoop = isLoop;

	// 変更にかけるフレーム数を覚えておく
	m_animChangeFrameTotal = 1;
	m_animChangeFrame = 1;
}

void Animation::ChangeAnim(const int& model, int animNo, bool isLoop, bool isForceChange, bool isChangeFrame)
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
		MV1DetachAnim(model, m_prev.attachNo);	// 前のアニメーションを外す
		InitAnim(m_prev);							// 前のアニメーション情報を初期化させる
	}

	// 現在再生中のアニメーションを前のアニメーションに移行する
	m_prev = m_current;

	// 新しくアニメーションを設定する
	m_current.animNo = animNo;
	m_current.attachNo = MV1AttachAnim(model, animNo, -1, false);
	m_current.totalTime = MV1GetAttachAnimTotalTime(model, m_current.attachNo);
	m_current.isLoop = isLoop;

	// 変更にかけるフレーム数を覚えておく
	m_animChangeFrameTotal = isChangeFrame;
	m_animChangeFrame = 0;

	// アニメーションのブレンド率を設定する
	UpdateAnimBlendRate(model);
}

bool Animation::IsAnimEnd(const int& model)
{
	// Loopアニメーションの場合は常にfalseを返す
	if (m_current.isLoop)return false;

	float time = MV1GetAttachAnimTime(model, m_current.attachNo);
	if (time >= m_current.totalTime)
	{
		return true;
	}

	return false;
}

void Animation::InitAnim(AnimData& anim)
{
	anim.animNo = -1;
	anim.attachNo = -1;
	anim.totalTime = 0.0f;
	anim.elapsedTime = 0.0f;
	anim.isLoop = false;
}

void Animation::UpdateAnimation(int model, AnimData anim, float dt)
{
	// アニメーションが設定されていない場合は何もしない
	if (anim.attachNo == -1)	return;

	// アニメーションの更新
	float time = MV1GetAttachAnimTime(model, anim.attachNo);
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
	MV1SetAttachAnimTime(model, anim.attachNo, time);

}

void Animation::UpdateAnimBlendRate(int model)
{
	// アニメーション変化のフレーム数に応じたブレンド率を設定する
	float rate = static_cast<float>(m_animChangeFrame) / static_cast<float>(m_animChangeFrameTotal);
	if (rate > kAnimBlendMax)	rate = kAnimBlendMax;

	// アニメーションのブレンド率を設定する
	MV1SetAttachAnimBlendRate(model, m_prev.animNo, kAnimBlendMax - rate);
	// アニメーションのブレンド率を設定する
	MV1SetAttachAnimBlendRate(model, m_current.animNo, rate);
}
