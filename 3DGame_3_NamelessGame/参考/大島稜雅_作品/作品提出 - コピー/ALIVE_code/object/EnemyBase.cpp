#include "EnemyBase.h"

#include "Player.h"
#include "ShotManager.h"
#include "ObjectManager.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/Aster.h"

namespace {
	//�F��ύX����}�e���A���̔ԍ�
	constexpr int change_material_color_num = 8;

	//���𓊂��Ă���Ƃ��̃A�j���[�V�����t���[��
	constexpr int throw_frame_time = 73;

	//�A�j���[�V������ύX����̂ɂ����鎞��
	constexpr int change_anim_frame = 10;

	//�����e�̒��_��
	constexpr int shadow_vertex_num = 7;

	//�����e�ō��O�p�`�̐�
	constexpr int shadow_triangle_num = 6;

	//�O�p�`����鏇�Ԃ̐�
	constexpr int number_of_orders_to_form_a_triangle = 18;

	//�����𕪊����鋗��
	constexpr float distance_to_divide = 50.0f;

	//�����e�̔��a
	constexpr float shadow_radius = 25.0f;

	//�����e�̍����␳
	constexpr float correction_chadow_height = 5.0f;

	//�G�̎���p
	constexpr float viewing_angle = 45.0f;

	//�G���v���C���[�����F�ł���͈�
	constexpr float visible_range = 1300.0f;

	//�G�̃X�s�[�h
	constexpr float move_speed = 3.0f;

	//�G���f���̍���
	constexpr float model_height = 150.0f;

	//���[�`
	constexpr float within_reach = 80.0f;

	//�΂𓊂��鋗��
	constexpr float throw_distance = 800.0f;

	//�͈͓��ɓ�������v���C���[�̕�������������
	constexpr float peasonal_space_range = 200.0f;

	//�E��̃t���[����
	const char* const hand_framename = "mixamorig:RightHandIndex2";

	//���f���̏�����]�x�N�g��
	constexpr VECTOR model_front_vec = { 0.0f,0.0f,-1.0f };
}

//�R���X�g���N�^
EnemyBase::EnemyBase(const int handle, const Material materialType, const ObjectInfo& objInfo) : CharacterBase(handle, materialType, objInfo)
{
	//�C���X�^���X���i�g���Ă��Ȃ��j
	Aster_ = std::make_shared<Aster>();

	//Aster�̏�����
	Aster_->ArrayInit(objInfo.pos);

	//�A�j���[�V�����̐ݒ�
	model_->SetAnimation(static_cast<int>(PlayerAnimType::Idle), true, false);

	//���ʃx�N�g���̎擾
	MATRIX matRotY = MGetRotY(objInfo.rot.y);
	frontVec_ = VTransform(model_front_vec, matRotY);

	//�������ʃx�N�g���̐ݒ�
	initFrontVec_ = frontVec_;

	//�}�e���A���̐F��ς���
	MV1SetMaterialDifColor(model_->GetModelHandle(), change_material_color_num, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
}

//�f�X�g���N�^
EnemyBase::~EnemyBase()
{
}

//�X�V
void EnemyBase::Update(Player& player)
{
	//���f���̍X�V
	model_->Update();

	//Aster�̍X�V
	Aster_->Update();

	if (isThrow_)
	{
		return;
	}

	//�v���C���[�̍��W
	VECTOR playerPos = player.GetStatus().pos;

	//�v���C���[�����G����
	SearchForPlayer(playerPos);

	//�v���C���[�����m���Ă��Ȃ�������
	//�ҋ@�A�j���[�V�����ɕύX����
	if (!isDetection_)
	{
		model_->ChangeAnimation(static_cast<int>(PlayerAnimType::Idle), true, false, change_anim_frame);
	}

	//������A�j���[�V�������I�������
	//�����Ă���Ƃ����t���O��false�ɂ���
	if (model_->IsAnimEnd() && isThrow_)
	{
		model_->ChangeAnimation(static_cast<int>(PlayerAnimType::Idle), true, false, change_anim_frame);
		isThrow_ = false;
	}

	//���G
   	if (IsThereAnObject(playerPos))
	{
		if (isDetection_) {
			//�v���C���[��ǐՂ���
			TrackingUpdate(playerPos);
		}
	}
	else
	{
		if (Aster_->LocationInformation(playerPos, pos_)) {
			Aster_->RouteSearch();
		}
	}

	if (!Aster_->GetIsRouteEmpty()) {
		//���݂̃C���f�b�N�X�����X�V����
		Aster_->CurrentIndexUpdate(playerPos, pos_);

		//�ړ�
		RoutingUpdate(playerPos);
	}
}

//�`��
void EnemyBase::Draw()
{
	model_->Draw();
#ifdef _DEBUG
//	Aster_->DebugDraw();
#endif
}

// �v���C���[��ǐՂ���
void EnemyBase::TrackingUpdate(const VECTOR& playerPos)
{
	//�v���C���[�Ǝ����̍����Z�o����
	VECTOR distancePlayerAndEnemy = VSub(playerPos, pos_);

	//Y���W�̒ǐՂ͍l�����Ȃ�
	distancePlayerAndEnemy.y = 0;

	//�ړ��x�N�g���̐���
	VECTOR moveVec = VScale(VNorm(distancePlayerAndEnemy), move_speed);

	//��]�s��Ɗg�k�s��̍����s��
	MATRIX mtx = CombiningRotAndScallMat(distancePlayerAndEnemy);

	//�|�W�V�����̈ړ�
	pos_ = VAdd(pos_, moveVec);

	//��]�s��Ɗg�k�s����|�����s��ɕ��s�ړ��s���������
	mtx = MMult(mtx, MGetTranslate(pos_));

	//�s������f���ɃZ�b�g����
	MV1SetMatrix(model_->GetModelHandle(), mtx);
}

//�v���C���[�����G����
void EnemyBase::SearchForPlayer(const VECTOR& playerPos)
{
	//�G����v���C���[�̒�������
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(playerPos, pos_);
	float innerProduct = 0.0f;

	//���ς��擾����(�Ԃ�l�̓R�T�C��)
	innerProduct = VDot(frontVec_, VNorm(VSub(playerPos, pos_)));

	//��L�̌��ʂ�x���@�ɕς���
	float radian = acos(innerProduct);
	innerProduct = MathUtil::RadianToDegree(radian);

	//����͈͓̔����������v���C���[�����F�ł��鋗������
	//�Z��������v���C���[�����m�������Ƃɂ���
	if (innerProduct < viewing_angle && distanceSize < visible_range)
	{
		isDetection_ = true;
	}
	else {
		isDetection_ = false;
	}

	//��苗�����ɂȂ�����v���C���[�����m�������Ƃɂ���
	if (distanceSize < peasonal_space_range)
	{
		isDetection_ = true;
	}
}

//���[�g�ʂ�Ɉړ�����
void EnemyBase::RoutingUpdate(const VECTOR& playerPos)
{
	//�G�l�~�[�����ɖڎw�����̒��S���W
	VECTOR targetPos = Aster_->GetDestinationCoordinates(playerPos);

	//�ڕW�n�_�ƃ|�W�V�����̋������擾
	VECTOR distance = VSub(targetPos, pos_);

	//�G�l�~�[�Ɩڎw�����̒��S���W�Ƃ̋����̃x�N�g���T�C�Y
	float size = VSize(distance);

	//��]�s��Ɗg�k�s��̍����s��
	MATRIX mtx = CombiningRotAndScallMat(distance);

	//��]�s��Ɗg�k�s����|�����s��ɕ��s�ړ��s���������
	mtx = MMult(mtx, MGetTranslate(pos_));

	//�s������f���ɃZ�b�g����
	MV1SetMatrix(model_->GetModelHandle(), mtx);

	if (size > 1.0f)
	{
		//���K��
		VECTOR norm = VNorm(distance);
		//�ړ��x�N�g�����쐬
		VECTOR moveVec = VScale(norm, move_speed);
		//�ړ�
		pos_ = VAdd(pos_, moveVec);
	}
}

//�G����v���C���[�̒��������ɃI�u�W�F�N�g�����邩
bool EnemyBase::IsThereAnObject(const VECTOR& playerPos)
{
	//�G�l�~�[�ƃv���C���[�̋���
	VECTOR distance = VSub(playerPos, pos_);

	//�G�l�~�[�ƃv���C���[�̋����̃x�N�g���T�C�Y
	int size = static_cast<int>(VSize(distance) / distance_to_divide);

	//�G�l�~�[�ƃv���C���[�̋����̐��K��
	VECTOR norm = VNorm(distance);
	
	//�G����v���C���[�̒�����ɃI�u�W�F�N�g�����邩
	bool noObject = true;

	//�G����v���C���[�̋�����50�Ŋ����Ă���
	//�G����v���C���[�̊ԂɃI�u�W�F�N�g�����邩���f����
	for (int i = 1; i < size; i++)
	{
		//�G����v���C���[�̋�����50�~i�ŕ�������
		VECTOR pointPos = VScale(norm, distance_to_divide * i);

		//�G�̍��W��point���W�𑫂�
		pointPos = VAdd(pos_, pointPos);

		//�I�u�W�F�N�g�����邩�擾����
		noObject = !Aster_->SearchBlockadeMode(pointPos);

		//�I�u�W�F�N�g����������u���C�N����
		if (!noObject)
		{

			//�o�H�T���̌��ʂ��Ȃ�������
			//�o�H�T���p�̔z�������������
			if (Aster_->GetIsRouteEmpty()) 
			{
				Aster_->ArrayInit(playerPos);
			}

			break;
		}
	}

	//���ʂ�Ԃ�
	return noObject;
}

//�e�𔭎˂���
void EnemyBase::Shot(const std::shared_ptr<ShotManager>& shotManager, const VECTOR& playerPos, const  float height)
{
	//�v���C���[�����m���Ă��邩�ǂ���
	//���m���Ă��Ȃ�������return
	if (!isDetection_)
	{
		return;
	}

	//���A�����Ă���t���O��true�ł͂Ȃ�������
	//�A�j���[�V�����𓊂���A�j���[�V�����ɕύX����
	model_->ChangeAnimation(static_cast<int>(PlayerAnimType::Throw), false, false, change_anim_frame);

	//�����Ă���A�j���[�V�����̓���t���[����
	//�e�𔭎˂���
	//�����Ă���t���O��true�ɂ���
	if (model_->GetSpecifiedAnimTime(throw_frame_time))
	{
		VECTOR framePos = model_->GetFrameLocalPosition(hand_framename);
		shotManager->Fire(framePos, playerPos, height);
		isThrow_ = true;
	}
}

//��]�s��Ɗg�k�s�����Z�����s����擾����
MATRIX EnemyBase::CombiningRotAndScallMat(const VECTOR& distance)
{
	//��]�s��̎擾
	MATRIX rotMtx = MGetRotVec2(model_front_vec, distance);

	//���K���������ʃx�N�g�����擾����
	frontVec_ = VTransform(model_front_vec, rotMtx);

	//�g�k�s��̎擾
	MATRIX scaleMtx = MGetScale(scale_);

	//��]�s��Ɗg�k�s��̊|���Z
	MATRIX mtx = MMult(rotMtx, scaleMtx);

	return mtx;
}

//�v���C���[�̗����e�Ɏg�p���钸�_���擾
VECTOR EnemyBase::VertexPosition(const float angle)
{

	VECTOR pos = {};

	//�x���@���ʓx�@�ɕϊ�����
	float radian = MathUtil::DegreeToRadian(angle);

	//�p�x�ɂ����W���擾����
	pos.x = sin(radian);
	pos.z = cos(radian);
	pos.y = 0.0f;

	//�|�W�V������25�{����(�T�C�Y����)
	pos = VScale(pos, shadow_radius);

	//�v���C���[�̃|�W�V�����Ə�L�Ŏ擾�����|�W�V�����𑫂�
	pos = VAdd(pos_, pos);

	//Y���W�͐��ƃ|���S���̓����蔻��Ŏ擾����
	//��ԋ߂��|���S����Y���W��������
	pos.y = pos_.y;

	return pos;
}

//�����e���쐬�A�`��
void EnemyBase::DrawPolygon3D()
{
	//���_�̐����z������
	VERTEX3D vertex[shadow_vertex_num] = {};

	//�O�p�`���쐬���鏇�Ԃ�ۑ�����z��
	WORD index[number_of_orders_to_form_a_triangle] = {};

	//�J���[
	COLOR_U8 difColor = GetColorU8(51, 51, 51, 125);
	COLOR_U8 spcColor = GetColorU8(0, 0, 0, 0);

	//�@���x�N�g��
	VECTOR norm = VGet(0.0f, 1.0f, 0.0f);

	//�p�x�Ɋ���Ē��_�̈ʒu��ύX����
	float angle = 0.0f;

	//�Z�p�`�̒��S���W���擾
	vertex[0].pos = VGet(pos_.x, pos_.y + correction_chadow_height, pos_.z);
	vertex[0].norm = norm;
	vertex[0].dif = difColor;
	vertex[0].spc = GetColorU8(0, 0, 0, 0);
	vertex[0].u = 0.0f;
	vertex[0].v = 0.0f;
	vertex[0].su = 0.0f;
	vertex[0].sv = 0.0f;

	//�p�x���Ƃ̒��_���擾
	for (int i = 1; i < shadow_vertex_num; i++) {
		vertex[i].pos = VertexPosition(angle);
		vertex[i].norm = norm;
		vertex[i].dif = difColor;
		vertex[i].spc = spcColor;
		vertex[i].u = 0.0f;
		vertex[i].v = 0.0f;
		vertex[i].su = 0.0f;
		vertex[i].sv = 0.0f;
		angle += 360.0f / shadow_triangle_num;
	}

	//�O�p�`���쐬���鏇��
	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 0;
	index[4] = 2;
	index[5] = 3;
	index[6] = 0;
	index[7] = 3;
	index[8] = 4;
	index[9] = 0;
	index[10] = 4;
	index[11] = 5;
	index[12] = 0;
	index[13] = 5;
	index[14] = 6;
	index[15] = 0;
	index[16] = 6;
	index[17] = 1;

	DrawPolygonIndexed3D(vertex, shadow_vertex_num, index, shadow_triangle_num, DX_NONE_GRAPH, true);
}