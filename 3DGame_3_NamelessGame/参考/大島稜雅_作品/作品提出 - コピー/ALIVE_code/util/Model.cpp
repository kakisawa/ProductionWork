#include "Model.h"
#include <cassert>

namespace 
{
	//�����蔻��p�t���[��
	const char* const collision_frame_name = "Coll";
}

//�t�@�C���p�X�̃R���X�g���N�^
Model::Model(const std::string& fileName,const Material materialType)
{
	modelHandle_ = MV1LoadModel(fileName.c_str());
	assert(modelHandle_ != -1);

	ClearAnimData(animPrev_);
	ClearAnimData(animNext_);

	materialType_ = materialType;
}

//duplicate���f���̃R���X�g���N�^
Model::Model(const int orgModel,const Material materialType)
{
	modelHandle_ = MV1DuplicateModel(orgModel);
	assert(modelHandle_ != -1);

	ClearAnimData(animPrev_);
	ClearAnimData(animNext_);

	materialType_ = materialType;
}

//�f�X�g���N�^
Model::~Model()
{
	MV1DeleteModel(modelHandle_);
}

//collision�t���[����collision�Ƃ��Ďg��
void Model::SetUseCollision(const bool isUse, const bool isNeedUpdate, const std::string& collFrameName)
{
	assert(isUse | !isNeedUpdate);

	//collFrameName�Ɏw��̃t���[���������Ă��邩
	//������̃T�C�Y����邱�ƂŒ��ׂ�
	int size = static_cast<int>(collFrameName.size());

	//�w�肪������Ί���̃t���[�����Փ˔���Ɏg��
	std::string frameName = collision_frame_name;

	//�w��̃t���[�����������ꍇ�A���̃t���[����
	//�Փ˔���p�t���[���ɂ���
	if (size > 0)
	{
		frameName = collFrameName;
	}

	if (isUseCollision_ != isUse)
	{
		if (isUse) 
		{
			colFrameIndex_ = MV1SearchFrame(modelHandle_, frameName.c_str());
			if (colFrameIndex_ < 0) 
			{
				colFrameIndex_ = -1;
			}
			MV1SetupCollInfo(modelHandle_, colFrameIndex_, 8, 8, 8);
		}
		else 
		{
			MV1TerminateCollInfo(modelHandle_, -1);
		}
	}

	isUseCollision_ = isUse;
	isUpdateCollsion_ = isNeedUpdate;
}

//�X�V
void Model::Update()
{
	UpdateAnim(animPrev_);
	UpdateAnim(animNext_);

	animChangeFrame_++;
	if (animChangeFrame_ > animChangeFrameTotal_) 
	{
		animChangeFrame_ = animChangeFrameTotal_;
	}

	UpdateAnimBlendRate();

	if (isUpdateCollsion_ && isUpdateCollsion_)
	{
		MV1RefreshCollInfo(modelHandle_, colFrameIndex_);
	}
}

//�`��
void Model::Draw()
{
	MV1DrawModel(modelHandle_);
}

//�A�j���[�V�����ύX
void Model::ChangeAnimation(int animNo, bool isLoop, bool isForceChange, int changeFrame)
{
	if (!isForceChange)
	{
		if (animNext_.animNo == animNo)	return;
	}

	if (animPrev_.attachNo != -1)
	{
		MV1DetachAnim(modelHandle_, animPrev_.attachNo);
		ClearAnimData(animPrev_);
	}
	animPrev_ = animNext_;

	animNext_.animNo = animNo;
	animNext_.attachNo = MV1AttachAnim(modelHandle_, animNo, -1, false);
	animNext_.totalTime = MV1GetAttachAnimTotalTime(modelHandle_, animNext_.attachNo);
	animNext_.isLoop = isLoop;

	animChangeFrameTotal_ = changeFrame;
	animChangeFrame_ = 0;

	UpdateAnimBlendRate();
}

//�A�j���[�V�����̏I�����擾
bool Model::IsAnimEnd()
{
	if (animNext_.isLoop) return false;

	float time = MV1GetAttachAnimTime(modelHandle_, animNext_.attachNo);

	if (time >= animNext_.totalTime) return true;
	return false;
}

//����t���[���̍��W���擾
VECTOR Model::GetFrameLocalPosition(const std::string& frameName) const
{
	int frameNo = MV1SearchFrame(modelHandle_, frameName.c_str());
	auto name = MV1GetFrameName(modelHandle_, frameNo);
	VECTOR localPos = MV1GetFramePosition(modelHandle_, frameNo);

	return localPos;
}

//�A�j���[�V�����̓���t���[���ƌ��݂̃A�j���[�V�����t���[���������������擾����
bool Model::GetSpecifiedAnimTime(const int specifiedTime) const
{

	float currentAnimTime = MV1GetAttachAnimTime(modelHandle_, animNext_.attachNo);

	if (currentAnimTime == specifiedTime) 
	{
		return true;
	}

	return false;
}


//�|�W�V�����ݒ�
void Model::SetPos(const VECTOR& pos)
{
	pos_ = pos;
	MV1SetPosition(modelHandle_, pos);
}

//�p�x�ݒ�
void Model::SetRot(const VECTOR& rot)
{
	rot_ = rot;
	MV1SetRotationXYZ(modelHandle_, rot);
}

//�g�k���ݒ�
void Model::SetScale(const VECTOR& scale)
{
	MV1SetScale(modelHandle_,scale);
}

//collision�t���[���ݒ�
void Model::SetCollFrame(const std::string& collFrameName)
{
	colFrameIndex_ = MV1SearchFrame(modelHandle_, collFrameName.c_str());
	if (colFrameIndex_ < 0)
	{
		colFrameIndex_ = -1;
	}
	MV1SetupCollInfo(modelHandle_, colFrameIndex_, 8, 8, 8);
}

//�A�j���[�V�����ݒ�
void Model::SetAnimation(const int animNo, const  bool isLoop, const  bool IsForceChange)
{
	if (!IsForceChange) 
	{
		if (animNext_.animNo == animNo)return;
	}

	if (animPrev_.attachNo != -1) 
	{
		MV1DetachAnim(modelHandle_, animPrev_.attachNo);
		ClearAnimData(animPrev_);
	}
	if (animNext_.attachNo != -1) 
	{
		MV1DetachAnim(modelHandle_, animNext_.attachNo);
		ClearAnimData(animNext_);
	}

	animNext_.animNo = animNo;
	animNext_.attachNo = MV1AttachAnim(modelHandle_, animNext_.animNo, -1, false);
	animNext_.totalTime = MV1GetAttachAnimTotalTime(modelHandle_, animNext_.attachNo);
	animNext_.isLoop = isLoop;

	animChangeFrameTotal_ = 1;
	animChangeFrame_ = 1;
}

//�A�j���[�V�����̏I����ݒ�
void Model::SetAnimEndFrame(const int animNo)
{
	SetAnimation(animNo, false, true);
	MV1SetAttachAnimTime(modelHandle_, animNext_.attachNo, animNext_.totalTime);
}

//�A�j���[�V�����̃t���[����ݒ肷��
void Model::SetAnimationFrame(const float value)
{
	MV1SetAttachAnimTime(modelHandle_, animNext_.attachNo, value);
}

//�A�j���[�V�������̏�����
void Model::ClearAnimData(AnimData& anim)
{
	anim.animNo = -1;
	anim.attachNo = -1;
	anim.totalTime = 0.0f;
	anim.isLoop = false;
}

//�A�j���[�V�����̍X�V
void Model::UpdateAnim(const AnimData& anim, const  float dt)
{
	if (anim.attachNo == -1) return;

	float time = MV1GetAttachAnimTime(modelHandle_, anim.attachNo);
	time += dt;
	if (time > anim.totalTime)
	{
		if (anim.isLoop)
		{
			time -= anim.totalTime;
		}
		else
		{
			time = anim.totalTime;
		}
	}
	MV1SetAttachAnimTime(modelHandle_, anim.attachNo, time);
}

//�A�j���[�V�����̃u�����h
void Model::UpdateAnimBlendRate()
{
	float rate = static_cast<float> (animChangeFrame_) / static_cast<float>(animChangeFrameTotal_);
	if (rate > 1.0f)	rate = 1.0f;
	MV1SetAttachAnimBlendRate(modelHandle_, animPrev_.attachNo, 1.0f - rate);
	MV1SetAttachAnimBlendRate(modelHandle_, animNext_.attachNo, rate);
}
