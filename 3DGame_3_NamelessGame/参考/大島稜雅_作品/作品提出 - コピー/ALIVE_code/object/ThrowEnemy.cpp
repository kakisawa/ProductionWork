#include "ThrowEnemy.h"

#include "../object/Player.h"
#include "../object/ShotManager.h"

#include "../util/Util.h"
#include "../util/Model.h"

namespace
{
	//�F��ς���}�e���A���̔ԍ�
	constexpr int change_material_num = 8;

	//���𓊂��Ă���Ƃ��̃A�j���[�V�����t���[��
	constexpr int throw_frame_time = 73;

	//�A�j���[�V������ύX����̂ɂ����鎞��
	constexpr int anim_change_time = 10;

	//�E��̃t���[����
	const char* const hand_framename = "hand.R_end";

	//�F
	constexpr COLOR_F color = { 1.0f, 0.0f, 0.0f, 1.0f };
}

//�R���X�g���N�^
 ThrowEnemy::ThrowEnemy(const int handle, const Material materialType, const ObjectInfo& objInfo):EnemyBase(handle, materialType, objInfo)
{
	 //�Փ˔���̐ݒ�
	 isCollCheck_ = true;
	 model_->SetUseCollision(isCollCheck_, true);

	 //������̎擾
	 std::string str = StrUtil::GetStringAfterSign(objInfo.name, "-");

	 //������Fake�������瓊����ӂ������t���O�𗧂Ă�
	 if (str == "Fake")
	 {
		 isFakeThrow_ = true;
	 }

	 //�}�e���A���̐F��ς���
	 MV1SetMaterialDifColor(model_->GetModelHandle(), change_material_num, color);
}

 //�f�X�g���N�^
ThrowEnemy::~ThrowEnemy()
{
}

//�X�V
void ThrowEnemy::Update(Player& player)
{
	//�v���C���[�̍��W
	VECTOR playerPos = player.GetStatus().pos;

	//���f���̍X�V
	model_->Update();

	//�v���C���[�����G����
	SearchForPlayer(playerPos);

	//������A�j���[�V�������I�������
	//�����Ă���Ƃ����t���O��false�ɂ���
	if (model_->IsAnimEnd() && isThrow_)
	{
		model_->ChangeAnimation(static_cast<int>(PlayerAnimType::Idle), true, false, anim_change_time);
		isThrow_ = false;
	}

	//�v���C���[�����m������
	//�v���C���[�̕���������
	if (isDetection_ && !isThrow_)
	{
		//�v���C���[�ƓG�̋���
		VECTOR distance = VNorm(VSub(playerPos, pos_));

		//Y���͍l�����Ȃ�
		distance.y = 0.0f;

		//��]�s��Ɗg�k�s��̍���
		MATRIX mtx = CombiningRotAndScallMat(distance);

		//��]�s��Ɗg�k�s����|�����s��ɕ��s�ړ��s���������
		mtx = MMult(mtx, MGetTranslate(pos_));

		//�s������f���ɃZ�b�g����
		MV1SetMatrix(model_->GetModelHandle(), mtx);
	}
}

//�`��
void ThrowEnemy::Draw()
{
	//���f���̕`��
	model_->Draw();

	//�����e�̕`��
	DrawPolygon3D();
}

//�e��������
void ThrowEnemy::Shot(const std::shared_ptr<ShotManager>& shotManager, const VECTOR& playerPos, const float height)
{
	//�v���C���[�����m���Ă��邩�ǂ���
	//���m���Ă��Ȃ�������return
	if (!isDetection_)
	{
		if (model_->GetCurrentAnimNo() != static_cast<int>(PlayerAnimType::Throw))
		{
			return;
		}
	}

	//�A�j���[�V�����𓊂���A�j���[�V�����ɕύX����
	model_->ChangeAnimation(static_cast<int>(PlayerAnimType::Throw), false, false, anim_change_time);

	//������ӂ������G�������ꍇ���^�[������
	if (isFakeThrow_)
	{
		return;
	}

	//�����Ă���A�j���[�V�����̓���t���[����
	//�e�𔭎˂���
	if (model_->GetSpecifiedAnimTime(throw_frame_time))
	{
		VECTOR framePos = model_->GetFrameLocalPosition(hand_framename);
		shotManager->Fire(framePos, playerPos, height);
		isThrow_ = true;
	}
}