#include "Animation.h"
#include "DxLib.h"

namespace {

	// �A�j���[�V�����֌W
	constexpr float kAnimChangeFrame = 8.0f;		// �A�j���[�V�����̐؂�ւ��ɂ�����t���[����
	constexpr float kAnimChangeRateSpeed = 0.2f;	// �A�j���[�V�����؂�ւ��ɂ����鑬�x
	constexpr float kAnimBlendAdd = 0.5f;			// �A�j���[�V�����u�����h�̑����l
	constexpr float kAnimBlendMax = 1.0f;			// �A�j���[�V�����̐؂�ւ��ɂ�����ő�t���[����
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
	// �A�j���[�V������Ԃ̏�����
	InitAnim(m_prev);
	InitAnim(m_current);
}

void Animation::Update(const int& model)
{
	// ���f���̃A�b�v�f�[�g
	m_animSpeed++;
	if (m_animSpeed >= m_animChangeFrameTotal)
	{
		UpdateAnimation(model,m_prev);
		UpdateAnimation(model, m_current);
		m_animSpeed = 0.0f;
	}

	// �w��t���[���Ɋ|���ăA�j���[�V������ύX����
	m_animChangeFrame += kAnimChangeRateSpeed;
	if (m_animChangeFrame >= m_animChangeFrameTotal)
	{
		m_animChangeFrame = m_animChangeFrameTotal;
	}

	// �A�j���[�V�����̃u�����h����ύX����
	UpdateAnimBlendRate(model);
}

void Animation::Draw()
{
}

void Animation::SetAnim(const int& model,int animNo, bool isLoop, bool isForceChange)
{	
	// �A�j���[�V�������Đ������邩
	if (!isForceChange)
	{
		// ���ɍĐ�����Ă���A�j���[�V�����͍Đ����Ȃ�
		if (m_current.animNo == animNo)	return;
	}

	// �O�̃A�j���[�V�������c���Ă�����I��������
	if (m_prev.attachNo != -1)
	{
		MV1DetachAnim(model, m_prev.attachNo);	// �O�̃A�j���[�V�������O��
		InitAnim(m_prev);							// �O�̃A�j���[�V��������������������
	}
	if (m_current.attachNo != -1)
	{
		MV1DetachAnim(model, m_current.attachNo);	// ���݂̃A�j���[�V�������O��
		InitAnim(m_current);						// ���݂̃A�j���[�V��������������������
	}

	// �V�����A�j���[�V������ݒ肷��
	m_current.animNo = animNo;
	m_current.attachNo = MV1AttachAnim(model, animNo, -1, false);
	m_current.totalTime = MV1GetAttachAnimTotalTime(model, m_current.attachNo);
	m_current.isLoop = isLoop;

	// �ύX�ɂ�����t���[�������o���Ă���
	m_animChangeFrameTotal = 1;
	m_animChangeFrame = 1;
}

void Animation::ChangeAnim(const int& model, int animNo, bool isLoop, bool isForceChange, bool isChangeFrame)
{
	// �A�j���[�V�������Đ������邩
	if (!isForceChange)
	{
		// ���ɍĐ�����Ă���A�j���[�V�����͍Đ����Ȃ�
		if (m_current.animNo == animNo)	return;
	}

	// �O�̃A�j���[�V�������c���Ă�����I��������
	if (m_prev.attachNo != -1)
	{
		MV1DetachAnim(model, m_prev.attachNo);	// �O�̃A�j���[�V�������O��
		InitAnim(m_prev);							// �O�̃A�j���[�V��������������������
	}

	// ���ݍĐ����̃A�j���[�V������O�̃A�j���[�V�����Ɉڍs����
	m_prev = m_current;

	// �V�����A�j���[�V������ݒ肷��
	m_current.animNo = animNo;
	m_current.attachNo = MV1AttachAnim(model, animNo, -1, false);
	m_current.totalTime = MV1GetAttachAnimTotalTime(model, m_current.attachNo);
	m_current.isLoop = isLoop;

	// �ύX�ɂ�����t���[�������o���Ă���
	m_animChangeFrameTotal = isChangeFrame;
	m_animChangeFrame = 0;

	// �A�j���[�V�����̃u�����h����ݒ肷��
	UpdateAnimBlendRate(model);
}

bool Animation::IsAnimEnd(const int& model)
{
	// Loop�A�j���[�V�����̏ꍇ�͏��false��Ԃ�
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
	// �A�j���[�V�������ݒ肳��Ă��Ȃ��ꍇ�͉������Ȃ�
	if (anim.attachNo == -1)	return;

	// �A�j���[�V�����̍X�V
	float time = MV1GetAttachAnimTime(model, anim.attachNo);
	time += dt;
	if (time >= anim.totalTime)
	{
		if (anim.isLoop)
		{
			// �A�j���[�V�����̃��[�v
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
	// �A�j���[�V�����ω��̃t���[�����ɉ������u�����h����ݒ肷��
	float rate = static_cast<float>(m_animChangeFrame) / static_cast<float>(m_animChangeFrameTotal);
	if (rate > kAnimBlendMax)	rate = kAnimBlendMax;

	// �A�j���[�V�����̃u�����h����ݒ肷��
	MV1SetAttachAnimBlendRate(model, m_prev.animNo, kAnimBlendMax - rate);
	// �A�j���[�V�����̃u�����h����ݒ肷��
	MV1SetAttachAnimBlendRate(model, m_current.animNo, rate);
}
