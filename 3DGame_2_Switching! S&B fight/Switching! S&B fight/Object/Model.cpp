#include "Model.h"
#include <cassert>
#include "DxLib.h"

namespace {

	// �A�j���[�V�����֌W
	constexpr float kAnimChangeFrame = 8.0f;		// �A�j���[�V�����̐؂�ւ��ɂ�����t���[����
	constexpr float kAnimChangeRateSpeed = 0.2f;	// �A�j���[�V�����؂�ւ��ɂ����鑬�x
	constexpr float kAnimBlendAdd = 0.5f;			// �A�j���[�V�����u�����h�̑����l
	constexpr float kAnimBlendMax = 1.0f;			// �A�j���[�V�����̐؂�ւ��ɂ�����ő�t���[����


	float rate = 0.0f;
}

Model::Model(const char* fileName) :
	m_model(MV1LoadModel(fileName)),
	m_animChangeFrame(0),
	m_animChangeFrameTotal(0),
	m_animSpeed(0.0f),
	m_animTime(0.0f)
{
	//�R�s�[�Ɏ��s�����ꍇ
	assert(m_model != -1);

	// �A�j���[�V������Ԃ̏�����
	InitAnim(m_prev);
	InitAnim(m_current);
}

Model::~Model()
{
	MV1DeleteModel(m_model);	// ���f�����̍폜
}

void Model::Init()
{

}

void Model::Update()
{
	// ���f���̃A�b�v�f�[�g
	m_animSpeed++;
	if (m_animSpeed >= m_animChangeFrameTotal)
	{
		UpdateAnimation(m_prev);
		UpdateAnimation(m_current);
		m_animSpeed = 0.0f;
	}

	// �w��t���[���Ɋ|���ăA�j���[�V������ύX����
	m_animChangeFrame += kAnimChangeRateSpeed;
	if (m_animChangeFrame >= m_animChangeFrameTotal)
	{
		m_animChangeFrame = m_animChangeFrameTotal;
	}

	// �A�j���[�V�����̃u�����h����ύX����
	UpdateAnimBlendRate();
}

void Model::Draw()
{
	// ���f���̕`��
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
	MV1SetScale(m_model, size);	// �v���C���[�̏����T�C�Y
}

void Model::SetRota(const VECTOR& rota)
{
	MV1SetRotationXYZ(m_model, rota);		// �v���C���[�̏����p�x
}

void Model::SetPos(const VECTOR& pos)
{
	m_pos = pos;
	MV1SetPosition(m_model, pos);									// �v���C���[�̏����ʒu	
}

void Model::SetAnim(int animNo, bool isLoop, bool isForceChange)
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
		MV1DetachAnim(m_model, m_prev.attachNo);	// �O�̃A�j���[�V�������O��
		InitAnim(m_prev);							// �O�̃A�j���[�V��������������������
	}
	if (m_current.attachNo != -1)
	{
		MV1DetachAnim(m_model, m_current.attachNo);	// ���݂̃A�j���[�V�������O��
		InitAnim(m_current);						// ���݂̃A�j���[�V��������������������
	}

	// �V�����A�j���[�V������ݒ肷��
	m_current.animNo = animNo;
	m_current.attachNo = MV1AttachAnim(m_model, animNo, -1, false);
	m_current.totalTime = MV1GetAttachAnimTotalTime(m_model, m_current.attachNo);
	m_current.isLoop = isLoop;

	// �ύX�ɂ�����t���[�������o���Ă���
	m_animChangeFrameTotal = 1;
	m_animChangeFrame = 1;
}

void Model::ChangeAnim(int animNo, bool isLoop, bool isForceChange, bool isChangeFrame)
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
		MV1DetachAnim(m_model, m_prev.attachNo);	// �O�̃A�j���[�V�������O��
		InitAnim(m_prev);							// �O�̃A�j���[�V��������������������
	}

	// ���ݍĐ����̃A�j���[�V������O�̃A�j���[�V�����Ɉڍs����
	m_prev = m_current;

	 // �V�����A�j���[�V������ݒ�
    m_current.animNo = animNo;
    m_current.attachNo = MV1AttachAnim(m_model, animNo, -1, false);
    m_current.totalTime = MV1GetAttachAnimTotalTime(m_model, m_current.attachNo);
    m_current.isLoop = isLoop;

	// �ύX�ɂ�����t���[�������o���Ă���
	m_animChangeFrameTotal = isChangeFrame;
	m_animChangeFrame = 0;

	// �A�j���[�V�����̃u�����h����ݒ肷��
	UpdateAnimBlendRate();
}

bool Model::IsAnimEnd()
{
	// Loop�A�j���[�V�����̏ꍇ�͏��false��Ԃ�
	if (m_current.isLoop) return false;

	m_animTime = MV1GetAttachAnimTime(m_model, m_current.attachNo);
<<<<<<< HEAD
	if (m_animTime >= m_current.totalTime){
=======
	if (m_animTime >= m_current.totalTime)
	{
>>>>>>> 60071db8ef0a37b71e4c4659020bfdc349240f1e
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
	// �A�j���[�V�������ݒ肳��Ă��Ȃ��ꍇ�͉������Ȃ�
	if (anim.attachNo == -1)	return;

	// �A�j���[�V�����̍X�V
	float time = MV1GetAttachAnimTime(m_model, anim.attachNo);
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
	MV1SetAttachAnimTime(m_model, anim.attachNo, time);
}

void Model::UpdateAnimBlendRate()
{
	// �A�j���[�V�����ω��̃t���[�����ɉ������u�����h����ݒ肷��
	rate = static_cast<float>(m_animChangeFrame) / static_cast<float>(m_animChangeFrameTotal);
	if (rate > kAnimBlendMax)	rate = kAnimBlendMax;

	// �A�j���[�V�����̃u�����h����ݒ肷��
	MV1SetAttachAnimBlendRate(m_model, m_prev.animNo, kAnimBlendMax - rate);
	// �A�j���[�V�����̃u�����h����ݒ肷��
	MV1SetAttachAnimBlendRate(m_model, m_current.animNo);

//�����C��
}
