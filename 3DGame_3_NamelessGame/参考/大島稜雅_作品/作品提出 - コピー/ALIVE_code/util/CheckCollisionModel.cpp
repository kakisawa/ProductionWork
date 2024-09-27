#include "CheckCollisionModel.h"

#include "../object/Player.h"
#include "../object/ObjectBase.h"
#include "../object/ObjectManager.h"

#include "Model.h"
#include "Util.h"
#include "SoundManager.h"

#include <algorithm>

namespace {
	//����
	constexpr int half = 2;

	//�O�p�`�̒��_�̐�
	constexpr int triangle_vertex_num = 3;

	//���̔��a
	constexpr float collition_radius = 200.0f;

	//�J�v�Z���̔��a
	constexpr float capsel_radius = 20.0f;
}

//�R���X�g���N�^
CheckCollisionModel::CheckCollisionModel()
{
}

//�f�X�g���N�^
CheckCollisionModel::~CheckCollisionModel()
{
}

// ����������͈͂̃I�u�W�F�N�g�Ƃ̏Փ˔���𓾂�
void CheckCollisionModel::CheckCollisionPersonalArea(const std::shared_ptr<Player>& player, const std::shared_ptr<ObjectManager>& objManager)
{
	//�Z�k��
	auto playerState = player->GetStatus();

	//�X�V�O�̃|�W�V�������擾����
	oldPos = playerState.pos;

	//�X�V��̃|�W�V�������擾����
	nowPos_ = VAdd(playerState.pos, playerState.moveVec);

	//���f���Ƌ��̓����蔻��
	objManager->AddCheckCollModel();
	for (const auto& model : objManager->GetAllCheckCollModel()) {
		if (player->GetStatus().situation.isInTransit) {
			if (player->GetDeadPersonModelPointer()->GetModelPointer() == model) {
				continue;
			}
		}

		MV1RefreshCollInfo(model->GetModelHandle(), model->GetColFrameIndex());
		CollModelState state;
		state.hitDim = MV1CollCheck_Sphere(model->GetModelHandle(), model->GetColFrameIndex(), oldPos, collition_radius + VSize(playerState.moveVec));
		state.model = model;
		hitDim_.push_back(state);
	}
	

	//�x�N�g���̐�Βl���擾���A�������������Ă��Ȃ����̃t���O���Ƃ�
	if (fabs(playerState.moveVec.x) > 0.01f || fabs(playerState.moveVec.z) > 0.01f) {
		moveFlag = true;
	}
	else {
		moveFlag = false;
	}

	CheckWallAndFloor();
}

// �Փ˂����ǂƏ��̃|���S�����𐔂���
void CheckCollisionModel::CheckWallAndFloor()
{
	hitWallNum = 0;
	hitFloorNum = 0;

	//�O�ɂƂ������f���Ƌ��̓����蔻�菈��
	int i = 0;
	for (const auto& result : hitDim_)
	{
		for (i = 0; i < result.hitDim.HitNum; i++) 
		{
			//���f���̖@���x�N�g��
			if (result.hitDim.Dim[i].Normal.y < 0.000001f && result.hitDim.Dim[i].Normal.y > -0.000001f) 
			{
				//�ő�Փː����ǂ̏Փː��𒴂��Ă����烊�^�[��
				if (hitWallNum > max_hit_coll)
				{
					return;
				}

				if (result.hitDim.Dim[i].Position[0].y > oldPos.y + 1.0f ||
					result.hitDim.Dim[i].Position[1].y > oldPos.y + 1.0f ||
					result.hitDim.Dim[i].Position[2].y > oldPos.y + 1.0f)
				{
					//�ǂƂ̏Փˏ��̎擾
					wallHitDim_[hitWallNum].hitDim = &result.hitDim.Dim[i];

					//�Փˉ񐔂̑���
					hitWallNum++;
				}
			}
			else 
			{
				//���|���S�����̎擾�H
				if (hitFloorNum > max_hit_coll)
				{
					return;
				}

				//���Ƃ̏Փˏ��̎擾
				floorHitDim_[hitFloorNum].hitDim = &result.hitDim.Dim[i];

				//�Փˉ񐔂̑���
				hitFloorNum++;

			}
		}
	}
}

// �Փ˂������f���̃|���S�����ǂ��𔻒f���A�ړ��x�N�g����␳����
void CheckCollisionModel::CheckCollisionWall(const std::shared_ptr<Player>& player)
{
	//�Z�k��
	auto& playerState = player->GetStatus();

	//�ǂ̏���
	if (hitWallNum != 0) 
	{
		hitFlag_ = false;
		//�����Ă�����
		if (moveFlag) 
		{
			int j = 0;
			for (int i = 0; i < hitWallNum; i++)
			{
				auto hitPoly = wallHitDim_[i];
				//�v���C���[�����ɂ����J�v�Z���ƕǃ|���S���̔���@�@�������Ă��Ȃ�������continue
				if (!HitCheck_Capsule_Triangle(nowPos_, VAdd(nowPos_, VGet(0.0f, playerState.height, 0.0f)), capsel_radius, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]))
				{
					continue;
				}

				hitFlag_ = true;

				if (isGoUpStep_) 
				{
					hitFlag_ = false;
					continue;
				}

				VECTOR slideVec = {};

				//�v���C���[�̃x�N�g���ƃ|���S���̖@���x�N�g���̊O�ς��擾
				slideVec = VCross(playerState.moveVec, hitPoly.hitDim->Normal);

				//�v���C���[�̃x�N�g���ƃ|���S���̖@���x�N�g���̊O�ςƃ|���S���̊O�ς̖@���x�N�g���̊O�ς��擾
				slideVec = VCross(hitPoly.hitDim->Normal, slideVec);

				//�X�V�O�̃v���C���[�̃|�W�V�����Ə�L�̊O�ς��擾
				nowPos_ = VAdd(oldPos, slideVec);

				//�ړ�����W�ŕǂƂ̏Փ˔���
				CollisionDetectionWithWallAfterMovement(j, player);

				//�������Ă��Ȃ�������t���O��܂�
				if (j == hitWallNum)
				{
					hitFlag_ = false;
					break;
				}
			}
		}
	}
	else {
		//����ǂƂ�hit��񂪂Ȃ������ꍇ
		for (int i = 0; i < hitWallNum; i++)
		{
			auto hitPoly = wallHitDim_[i];
			if (HitCheck_Capsule_Triangle(nowPos_, VAdd(nowPos_, VGet(0.0f, playerState.height, 0.0f)), capsel_radius, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]))
			{
				hitFlag_ = false;
				break;
			}
		}
	}


	// �ǂɓ������Ă�����ǂ��牟���o���������s��
	//���������|���S���̖@���x�N�g���̂T�{���v���C���[�̃|�W�V�����ɑ����Ă���
	if (hitFlag_ && !isGoUpStep_)
	{
		int i, j, k;
		for (k = 0; k < 16; k++)
		{
			for (i = 0; i < hitWallNum; i++)
			{
				auto& hitPoly = wallHitDim_[i];
				if (!HitCheck_Capsule_Triangle(nowPos_, VAdd(nowPos_, VGet(0.0f, playerState.height, 0.0f)), capsel_radius, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]))
				{
					continue;
				}

				//�x�N�g���𑫂�����̃v���C���[�̍��W
				nowPos_ = VAdd(nowPos_, VScale(hitPoly.hitDim->Normal, 5.0f));

				//�ړ�����W�ŕǂƂ̏Փ˔���
				CollisionDetectionWithWallAfterMovement(j, player);

				if (j == hitWallNum)break;
			}
			if (i != hitWallNum)break;
		}
	}
}

//�ړ�����W�ŕǂƂ̏Փ˔���
void CheckCollisionModel::CollisionDetectionWithWallAfterMovement(int& j, const std::shared_ptr<Player>& player)
{
	//�Z�k��
	auto& playerState = player->GetStatus();

	//�܂������蔻��H
	for (j = 0; j < hitWallNum; j++)
	{
		auto& hitPoly = wallHitDim_[j];
		if (HitCheck_Capsule_Triangle(nowPos_, VAdd(nowPos_, VGet(0.0f, playerState.height, 0.0f)), capsel_radius, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]))
		{
			player->SetMoveVec(VGet(0, 0, 0));
			break;
		}
	}
}

// �Փ˂������f���̃|���S���������𔻒f����
void CheckCollisionModel::CheckCollisionFloor(const std::shared_ptr<Player>& player)
{
	//�Z�k��
	auto playerState = player->GetStatus();

	float jumpVec = playerState.jump.jumpVec;
	bool isJump = playerState.jump.isJump;

	//���Ƃ̓����蔻��
	if (hitFloorNum != 0)
	{
		//�W�����v�����x�N�g�����}�C�i�X�łȂ����
		if (isJump && jumpVec > 0.0f)
		{
			float minY = 0.0f;
			hitFlag_ = false;
			for (int i = 0; i < hitFloorNum; i++)
			{
				//�f�[�^�̃R�s�[
				auto& hitPoly = floorHitDim_[i];

				//�Փ˔��茋��
				hitLineResult_.push_back(HitCheck_Line_Triangle(nowPos_, VAdd(nowPos_, VGet(0.0f, playerState.height, 0.0f)), hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]));

				//�Փ˔��茋�ʂŏՓ˂��ĂȂ�������ȍ~�̏������s��Ȃ�
				if (hitLineResult_.back().HitFlag == false)
				{
					continue;
				}

				//�Փ˔��茋�ʂŏՓ˂��ĂȂ�������ȍ~�̏������s��Ȃ�
				if (hitFlag_ == true && minY < hitLineResult_.back().Position.y)
				{
					continue;
				}
				hitFlag_ = true;
				minY = hitLineResult_.back().Position.y;

				hitLineResult_.clear();

			}
			if (hitFlag_) {
				nowPos_.y = minY - playerState.height;
				if (jumpVec != 36.0f) 
				{
					jumpVec = -jumpVec;
				}
			}
		}
		else {
			float maxY = nowPos_.y + playerState.height;
			float correction = capsel_radius;
			hitFlag_ = false;
			for (int i = 0; i < hitFloorNum; i++)
			{
				auto& hitPoly = floorHitDim_[i];
				
				hitLineResult_.push_back(HitCheck_Line_Triangle(VAdd(nowPos_, VGet(0, playerState.height, 0)), nowPos_, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]));
				hitLineResult_.push_back(HitCheck_Line_Triangle(VAdd(nowPos_, VGet(correction, playerState.height,  0))	, VAdd(nowPos_, VGet(correction, 0, 0))	, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]));
				hitLineResult_.push_back(HitCheck_Line_Triangle(VAdd(nowPos_, VGet(-correction, playerState.height, 0))	, VAdd(nowPos_, VGet(-correction, 0, 0)), hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]));
				hitLineResult_.push_back(HitCheck_Line_Triangle(VAdd(nowPos_, VGet(0, playerState.height,  correction))	, VAdd(nowPos_, VGet(0, 0, correction))	, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]));
				hitLineResult_.push_back(HitCheck_Line_Triangle(VAdd(nowPos_, VGet(0, playerState.height, -correction))	, VAdd(nowPos_, VGet(0, 0, -correction)), hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]));

				int hitCount = 0;
				HITRESULT_LINE hitLine = {};

				for (auto& result : hitLineResult_) 
				{
					if (result.HitFlag == 1) 
					{
						hitCount++;
						hitLine = result;
					}
				}
				
				if (hitCount < 1) 
				{
					hitLineResult_.clear();
					continue;
				}

				hitFlag_ = true;
				maxY = hitLine.Position.y;
				hitLineResult_.clear();
			}
			if (hitFlag_)
			{
				nowPos_.y = maxY;
				jumpVec = 0.0f;
				if (isJump) {
  					isJump = false;
				}
			}
			else
			{
 				isJump = true;
			}
		}
	}

	player->SetJumpInfo(isJump, jumpVec);

}

// checkCollisionPersonalArea�AcheckCollisionWall�AcheckCollisionFloor���Ăяo���B
// �Ō�A��L�̊֐��Ŏ擾�����l����������
void CheckCollisionModel::CheckCollision(const std::shared_ptr<Player>& player, const std::shared_ptr<ObjectManager>& objManager)
{

	//�v���C���[������͈͂̏Փ˔�����Ƃ�
	CheckCollisionPersonalArea(player, objManager);

	CheckCollCorpseModel(player, objManager);
	//�Փ˂����I�u�W�F�N�g�����z���邱�Ƃ��o����I�u�W�F�N�g�����f����
	CheckStepDifference(player);
	//�擾�����Փˌ��ʂ���ǂɓ��������ꍇ�̏���
	CheckCollisionWall(player);
	//�擾�����Փˌ��ʂ��珰�ɓ��������ꍇ�̏���
	CheckCollisionFloor(player);

	FindThePolygonBelowThePlayer(player, objManager);

	//�|�W�V�����̃Z�b�g
	player->SetPos(nowPos_);

	//�Փ˔���̏���
	for (auto& hit : hitDim_)
	{
		MV1CollResultPolyDimTerminate(hit.hitDim);
	}

	hitDim_.clear();
}

// �Փ˂����I�u�W�F�N�g�����z�����邩���f����
void CheckCollisionModel::CheckStepDifference(const std::shared_ptr<Player>& player)
{
	//�Z�k��
	auto playerState = player->GetStatus();

	objectHeightY_ = 0;
	isGoUpStep_ = false;

	//���z������i�������f���邽��
	bool overHeight = false;

	//�v���C���[����ɂ����J�v�Z���Ɠ��������|���S���̓����蔻����s��
	//�v���C���[�̍��������|���S���̍������Ⴉ������i������鏈�������
	if (hitWallNum != 0)
	{
		for (int i = 0; i < hitWallNum; i++)
		{
			auto hitPoly = wallHitDim_[i];
			if (!HitCheck_Capsule_Triangle(VGet(nowPos_.x, nowPos_.y, nowPos_.z), VAdd(nowPos_, VGet(0.0f, playerState.height, 0.0f)), capsel_radius, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]))
			{
				continue;
			}

			//�Փ˂����|���S���̈��Y���̍������_�������o��
			//���z������i���ȏゾ������overHeight��true�ɂ���
			for (int i = 0; i < triangle_vertex_num; i++)
			{
				if (nowPos_.y + playerState.height / half < hitPoly.hitDim->Position[i].y)
				{
					overHeight = true;
				}
			}

			//���z���邱�Ƃ��ł��鍂���ň�ԍ���Y���̒l��������
			if (!overHeight) {
				for (int i = 0; i < triangle_vertex_num; i++)
				{
					if (objectHeightY_ < hitPoly.hitDim->Position[i].y)
					{
						objectHeightY_ = hitPoly.hitDim->Position[i].y;
						isGoUpStep_ = true;
					}
				}
			}
		}
	}

	//���
	if (objectHeightY_ > 0.0f)
	{
		nowPos_.y = objectHeightY_;
	}
}

// �v���C���[�̉��ɉe���ǂ���`�悵�������߂�
// �v���C���[�̐^���̈�ԋ߂��|���S���̍������擾����
void CheckCollisionModel::FindThePolygonBelowThePlayer(const std::shared_ptr<Player>& player, const std::shared_ptr<ObjectManager>& objManager)
{
	std::unordered_map<Material, std::list<MV1_COLL_RESULT_POLY>> hitLine;

	//�I�u�W�F�N�g�̃|���S�����擾����
	for (const auto& model : objManager->GetAllCheckCollModel())
	{
		//�v���C���[�������������^��ł���ꍇ
		//�����^��ł���I�u�W�F�N�g�̃|���S�����擾���Ȃ��悤�ɂ��Ă���
		if (player->GetStatus().situation.isInTransit)
		{
			if (player->GetDeadPersonModelPointer()->GetModelPointer() == model)
			{
				continue;
			}
		}

		//�I�u�W�F�N�g�ƃv���C���[�̍����̍����擾����
		float distance = nowPos_.y - model->GetPos().y;
		distance = (std::max)(distance, -distance);

		//���f���Ɛ��̓����蔻������
		MV1RefreshCollInfo(model->GetModelHandle(), model->GetColFrameIndex());
		VECTOR playerHeadPos = VGet(nowPos_.x, nowPos_.y + player->GetStatus().height, nowPos_.z);
		VECTOR lowPos = VGet(nowPos_.x, nowPos_.y - distance, nowPos_.z);
		hitLine[model->GetMaterialType()].push_back(MV1CollCheck_Line(model->GetModelHandle(), model->GetColFrameIndex(), playerHeadPos, lowPos));
		if (hitLine[model->GetMaterialType()].back().HitFlag == 0) 
		{
			hitLine[model->GetMaterialType()].pop_back();
		}
		else {
			int a = 0;
		}
	}

	float nearPosY = 5000.0f;
	float resultY = 0.0f;
	float distanceY = 0.0f;
	Material materialType = Material::max;

	//�����蔻��̌��ʂ����ԋ߂��|���S����Y���W���擾����
	for (const auto& list : hitLine)
	{
		for (const auto& result : list.second)
		{
			
			if (result.HitFlag == 0) 
			{
				continue;
			}

			distanceY = nowPos_.y - result.HitPosition.y;
			if (nearPosY > distanceY)
			{
				nearPosY = distanceY;
				resultY = result.HitPosition.y;
				materialType = list.first;
			}
		}
	}

	//��ԋ߂�Y���W���ۉe��Y���W�Ƃ��Ďg��
	//�����蔻��̌��ʉ��Ƃ��������Ă��Ȃ������ꍇ
	//Y���W��0�ɂ���
	player->SetRoundShadowHeightAndMaterial(resultY + 1,materialType);

	hitLine.clear();
}

//���̂Ƃ̏Փ˔���
void CheckCollisionModel::CheckCollCorpseModel(const std::shared_ptr<Player>& player, const std::shared_ptr<ObjectManager>& objManager)
{

	//�Z�k��
	auto playerState = player->GetStatus();

	//�v���C���[������ʂ̎��̂������^��ł�����擾���Ȃ�
	if (player->GetStatus().situation.isInTransit)
	{
		return;
	}

	//�����^�Ԏ��̂��擾����
	for (const auto& obj : objManager->GetSpecificObject(ObjectType::Corpse))
	{
		for (const auto& hit : hitDim_)
		{
			//�Փˌ��ʂ����̈ȊO��������continue
			if (hit.model != obj->GetModelPointer())
			{
				continue;
			}

			//���̂̃��f���ƃv���C���[�̍��W����ɍ쐬���ꂽ�J�v�Z���Ƃ̏Փ˔���
			auto result = MV1CollCheck_Capsule(hit.model->GetModelHandle(), hit.model->GetColFrameIndex(), playerState.pos, VAdd(playerState.pos, VGet(0, playerState.height, 0)), capsel_radius);

			//��L�̏Փ˔���̌��ʂ�1�ł�����΃v���C���[��
			//���̎��̂̃|�C���^�[�Ǝ����^�ׂ�t���O�𑗂�
			if (result.HitNum > 0)
			{
				player->SetCarryInfo(true,obj);
			}

			MV1CollResultPolyDimTerminate(result);

		}
	}
}