#include "StreetLight.h"

#include "Player.h"

#include "../util/Util.h"
#include "../util/Model.h"

#include <string>
#include <DxLib.h>

namespace
{
	//���C�g�̊O���̊p�x
	constexpr float light_outer_angle = 130.0f;

	//���C�g�̓����̊p�x
	constexpr float light_inner_angle = 60.0f;

	//���C�g�͈̔�
	constexpr float light_range = 1000.0f;

	//�����֌W�Ȃ���������p�����[�^�[
	constexpr float attenuation_regardless_of_distance = 0.0f;

	//�����ɔ�Ⴕ�Č�������p�����[�^�[
	constexpr float attenuation_proportional_to_distance = 0.0009f;

	//������2��ɔ�Ⴕ�Č�������p�����[�^�[
	constexpr float attenuation_proportional_to_the_square_of_the_distance = 0.0f;

	//���C�g��L���ɂ��鋗��
	constexpr float light_enable_distance = 2500.0f;

	//���C�g��u���t���[���̖��O
	const std::string light_put_frame_name = "LightPos";

	//���C�g�̕���
	constexpr VECTOR light_dir = { 0.0f, -1.0f, 0.0f };

	//�X�|�b�g���C�g�̐F
	constexpr COLOR_F spot_light_color = { 0.87f, 0.72f, 0.52f, 1.0f };
}

//�R���X�g���N�^
StreetLight::StreetLight(const int handle, const Material materialType, const ObjectInfo objInfo) : OrnamentBase(handle, materialType, objInfo)
{
	//���C�g�̈ʒu���擾
	VECTOR lightPos = model_->GetFrameLocalPosition(light_put_frame_name);

	//���C�g�̊O���̊p�x�����W�A���ɕϊ�
	float outerAngle = MathUtil::DegreeToRadian(light_outer_angle);

	//���C�g�̓����̊p�x�����W�A���ɕϊ�
	float innerAngle = MathUtil::DegreeToRadian(light_inner_angle);

	//�X�|�b�g���C�g�̃n���h�����쐬
	lightHandle_ = CreateSpotLightHandle(lightPos,light_dir,
										 outerAngle, innerAngle,
										 light_range,
										 attenuation_regardless_of_distance,
										 attenuation_proportional_to_distance,
										 attenuation_proportional_to_the_square_of_the_distance);
	//�X�|�b�g���C�g�̐F��ݒ�
	SetLightDifColorHandle(lightHandle_, spot_light_color);

	//���C�g�𖳌��ɂ���
	SetLightEnableHandle(lightHandle_, false);
}

//�f�X�g���N�^
StreetLight::~StreetLight()
{
	//���C�g�̃n���h�����폜
	DeleteLightHandle(lightHandle_);
}

//�X�V
void StreetLight::Update(Player& player)
{
	//���C�g�̈ʒu���擾
	VECTOR lightPos = model_->GetFrameLocalPosition(light_put_frame_name);

	//���C�g�̈ʒu���X�V
	SetLightPositionHandle(lightHandle_, lightPos);

	//�v���C���[�̈ʒu���擾
	VECTOR playerPos = player.GetStatus().pos;

	//�v���C���[�ƃ��C�g�̋������擾
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(pos_, playerPos);

	//���C�g��_������
	LightOn(distanceSize);
}

//�v���C���[�̈��͈͓��ɓ������烉�C�g��_������
void StreetLight::LightOn(float distanceSize)
{
	//�v���C���[�ƃ��C�g�̋��������C�g��L���ɂ��鋗�����傫���ꍇ
	if (distanceSize > light_enable_distance)
	{
		//���C�g�𖳌��ɂ���
		SetLightEnableHandle(lightHandle_, false);
	}
	else
	{
		//���C�g��L���ɂ���
		SetLightEnableHandle(lightHandle_, true);
	}
}
