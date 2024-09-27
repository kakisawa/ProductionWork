#include "Model.h"
#include <cassert>

namespace 
{
	//当たり判定用フレーム
	const char* const collision_frame_name = "Coll";
}

//ファイルパスのコンストラクタ
Model::Model(const std::string& fileName,const Material materialType)
{
	modelHandle_ = MV1LoadModel(fileName.c_str());
	assert(modelHandle_ != -1);

	ClearAnimData(animPrev_);
	ClearAnimData(animNext_);

	materialType_ = materialType;
}

//duplicateモデルのコンストラクタ
Model::Model(const int orgModel,const Material materialType)
{
	modelHandle_ = MV1DuplicateModel(orgModel);
	assert(modelHandle_ != -1);

	ClearAnimData(animPrev_);
	ClearAnimData(animNext_);

	materialType_ = materialType;
}

//デストラクタ
Model::~Model()
{
	MV1DeleteModel(modelHandle_);
}

//collisionフレームをcollisionとして使う
void Model::SetUseCollision(const bool isUse, const bool isNeedUpdate, const std::string& collFrameName)
{
	assert(isUse | !isNeedUpdate);

	//collFrameNameに指定のフレームが入っているか
	//文字列のサイズを取ることで調べる
	int size = static_cast<int>(collFrameName.size());

	//指定が無ければ既定のフレームを衝突判定に使う
	std::string frameName = collision_frame_name;

	//指定のフレームがあった場合、そのフレームを
	//衝突判定用フレームにする
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

//更新
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

//描画
void Model::Draw()
{
	MV1DrawModel(modelHandle_);
}

//アニメーション変更
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

//アニメーションの終わりを取得
bool Model::IsAnimEnd()
{
	if (animNext_.isLoop) return false;

	float time = MV1GetAttachAnimTime(modelHandle_, animNext_.attachNo);

	if (time >= animNext_.totalTime) return true;
	return false;
}

//特定フレームの座標を取得
VECTOR Model::GetFrameLocalPosition(const std::string& frameName) const
{
	int frameNo = MV1SearchFrame(modelHandle_, frameName.c_str());
	auto name = MV1GetFrameName(modelHandle_, frameNo);
	VECTOR localPos = MV1GetFramePosition(modelHandle_, frameNo);

	return localPos;
}

//アニメーションの特定フレームと現在のアニメーションフレーム数が同じかを取得する
bool Model::GetSpecifiedAnimTime(const int specifiedTime) const
{

	float currentAnimTime = MV1GetAttachAnimTime(modelHandle_, animNext_.attachNo);

	if (currentAnimTime == specifiedTime) 
	{
		return true;
	}

	return false;
}


//ポジション設定
void Model::SetPos(const VECTOR& pos)
{
	pos_ = pos;
	MV1SetPosition(modelHandle_, pos);
}

//角度設定
void Model::SetRot(const VECTOR& rot)
{
	rot_ = rot;
	MV1SetRotationXYZ(modelHandle_, rot);
}

//拡縮率設定
void Model::SetScale(const VECTOR& scale)
{
	MV1SetScale(modelHandle_,scale);
}

//collisionフレーム設定
void Model::SetCollFrame(const std::string& collFrameName)
{
	colFrameIndex_ = MV1SearchFrame(modelHandle_, collFrameName.c_str());
	if (colFrameIndex_ < 0)
	{
		colFrameIndex_ = -1;
	}
	MV1SetupCollInfo(modelHandle_, colFrameIndex_, 8, 8, 8);
}

//アニメーション設定
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

//アニメーションの終わりを設定
void Model::SetAnimEndFrame(const int animNo)
{
	SetAnimation(animNo, false, true);
	MV1SetAttachAnimTime(modelHandle_, animNext_.attachNo, animNext_.totalTime);
}

//アニメーションのフレームを設定する
void Model::SetAnimationFrame(const float value)
{
	MV1SetAttachAnimTime(modelHandle_, animNext_.attachNo, value);
}

//アニメーション情報の初期化
void Model::ClearAnimData(AnimData& anim)
{
	anim.animNo = -1;
	anim.attachNo = -1;
	anim.totalTime = 0.0f;
	anim.isLoop = false;
}

//アニメーションの更新
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

//アニメーションのブレンド
void Model::UpdateAnimBlendRate()
{
	float rate = static_cast<float> (animChangeFrame_) / static_cast<float>(animChangeFrameTotal_);
	if (rate > 1.0f)	rate = 1.0f;
	MV1SetAttachAnimBlendRate(modelHandle_, animPrev_.attachNo, 1.0f - rate);
	MV1SetAttachAnimBlendRate(modelHandle_, animNext_.attachNo, rate);
}
