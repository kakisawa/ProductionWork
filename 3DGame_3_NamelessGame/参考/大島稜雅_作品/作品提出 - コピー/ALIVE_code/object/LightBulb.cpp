#include "LightBulb.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/Easing.h"

#include <algorithm>

namespace 
{
	//�p�x��{�ɂ���
	constexpr int angle_scale = 2;

	//���Ԓ����p
	constexpr int time_adjustment = 4;

	//������
	constexpr float total_time = 360.0f;
}

//�R���X�g���N�^
LightBulb::LightBulb(const int handle, const  ObjectInfo& info)
{
	//���f���̐ݒ�
	model_ = std::make_shared<Model>(handle,Material::Iron);
	model_->SetPos(info.pos);
	model_->SetRot(info.rot);
	model_->SetScale(info.scale);

	rot_ = info.rot;
}

//�f�X�g���N�^
LightBulb::~LightBulb()
{
}

//�X�V
void LightBulb::Update()
{
	//���Ԍv��
	elapsedTime_ = (std::min)(elapsedTime_ + 1,total_time);

	//�p�x�̎擾
	angle_ = Easing::InOutSine(elapsedTime_, total_time, targetAngle_, angle_);

	if (elapsedTime_ >= total_time / time_adjustment)
	{
		//�o�ߎ��Ԃ̃��Z�b�g
		elapsedTime_ = 0.0f;

		//�ڕW�p�x��ύX����
		targetAngle_ -= targetAngle_ * angle_scale;
	}

	//Z�̉�]���ʓx�@�ɕύX����
	rot_.z = MathUtil::DegreeToRadian(angle_);

	//��]�̐ݒ�
	model_->SetRot(rot_);
}

//�`��
void LightBulb::Draw()
{
	model_->Draw();
}

//��]�x�N�g���̎擾
VECTOR LightBulb::GetRotVec() const
{
	MATRIX mtxRotZ = MGetRotZ(rot_.z);

	VECTOR frontVec = VTransform({ 0,-1,0 }, mtxRotZ);

	return frontVec;
}

//�t���[���̃|�W�V�����̎擾
VECTOR LightBulb::GetFramePos() const
{
	VECTOR pos = model_->GetFrameLocalPosition("LightBulb");
	return pos;
}
