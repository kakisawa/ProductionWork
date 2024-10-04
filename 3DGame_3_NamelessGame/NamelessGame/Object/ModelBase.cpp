#include "ModelBase.h"

namespace {
	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// Vector値初期価値
	constexpr float kInitFloat = 0.0f;				// float値初期化
}

ModelBase::ModelBase() :
	m_model(-1),
	m_angle(kInitFloat),
	m_pos(kInitVec),
	m_move(kInitVec),
	m_targetDir(kInitVec),
	m_animChangeFrame(0),
	m_animChangeFrameTotal(0)
{
}

ModelBase::~ModelBase()
{
	MV1DeleteModel(m_model);
}

void ModelBase::Init()
{
	MV1SetPosition(m_model, m_pos);
}

void ModelBase::Update()
{
	MV1SetPosition(m_model, m_pos);

	UpdateAnim(m_animPrev);
	UpdateAnim(m_animNext);

	m_animChangeFrame++;

	
	if (m_animChangeFrame > m_animChangeFrameTotal)
	{
		m_animChangeFrame = m_animChangeFrameTotal;			// TODO:minで書き直しができる
	}

	UpdateAnimBlendRate();
	
}

void ModelBase::Draw()
{
	MV1DrawModel(m_model);
}

void ModelBase::ChangeAnimation(const int animNo, const bool isLoop, const bool isForceChange, const int changeFrame)
{
	if (!isForceChange)
	{
		if (m_animNext.animNo == animNo)	return;
	}

	if (m_animPrev.attachNo != -1)
	{
		MV1DetachAnim(m_model, m_animPrev.attachNo);
		InitAnim(m_animPrev);
	}
	m_animPrev = m_animNext;

	m_animNext.animNo = animNo;
	m_animNext.attachNo = MV1AttachAnim(m_model, animNo, -1, false);
	m_animNext.totalTime = MV1GetAttachAnimTotalTime(m_model, m_animNext.attachNo);
	m_animNext.isLoop = isLoop;

	m_animChangeFrameTotal = changeFrame;
	m_animChangeFrame = 0;

	UpdateAnimBlendRate();
}

void ModelBase::SetAnimation(const int animNo, const bool isLoop, const bool isForceChange)
{
	if (!isForceChange)
	{
		// すでに再生中のアニメーションは再生しない
		if (m_animNext.animNo == animNo)return;
	}

	// 以前のアニメーションが残っていれば終了
	if (m_animPrev.attachNo != -1)
	{
		MV1DetachAnim(m_model, m_animPrev.attachNo);
		InitAnim(m_animPrev);
	}
	if (m_animNext.attachNo != -1)
	{
		MV1DetachAnim(m_model, m_animNext.attachNo);
		InitAnim(m_animNext);
	}

	// 新しくアニメーションを設定
	m_animNext.animNo = animNo;
	m_animNext.attachNo = MV1AttachAnim(m_model, animNo, -1, false);
	m_animNext.totalTime = MV1GetAttachAnimTotalTime(m_model, m_animNext.attachNo);
	m_animNext.isLoop = isLoop;

	// 変更にかけるフレーム数を覚えておく
	m_animChangeFrameTotal = 1;
	m_animChangeFrame = 1;
}

bool ModelBase::IsAnimEnd()
{
	// Loopアニメーションの場合は常にfalseを返す
	if (m_animNext.isLoop)	return false;

	float time = MV1GetAttachAnimTime(m_model, m_animNext.attachNo);
	if (time >= m_animNext.totalTime)
	{
		return true;
	}

	return false;
}

void ModelBase::InitAnim(AnimData& anim)
{
	anim.animNo = -1;
	anim.attachNo = -1;
	anim.totalTime = 0.0f;
	anim.isLoop = false;
}

void ModelBase::UpdateAnim(const AnimData& anim, const float dt)
{
	// アニメーションが設定されていない場合は何もしない
	if (anim.animNo == -1)	return;

	// アニメーションの更新
	float time = MV1GetAttachAnimTime(m_model, anim.attachNo);
	time += dt;
	if (time > anim.totalTime)
	{
		if (anim.isLoop)
		{
			// アニメーションのループ
			time -= anim.totalTime;
		}
		else 
		{
			time = anim.totalTime;				// TODO:minで書き直しができる
		}
	}

	MV1SetAttachAnimTime(m_model, anim.attachNo, time);
}

void ModelBase::UpdateAnimBlendRate()
{
	float rate = static_cast<float> (m_animChangeFrame) / static_cast<float>(m_animChangeFrameTotal);
	if (rate > 1.0f)
	{
		rate = 1.0f;
	}

	MV1SetAttachAnimBlendRate(m_model, m_animPrev.attachNo, 1.0f - rate);
	MV1SetAttachAnimBlendRate(m_model, m_animNext.attachNo, rate);
}
