#pragma once
#include <DxLib.h>
#include <memory>
#include <vector>
#include <list>
#include <deque>

class Player;
class Model;
class ObjectManager;

namespace {
	//�ő�Փː�
	constexpr int max_hit_coll = 2048;
}

class CheckCollisionModel
{
private:

	struct CollModelState {
		MV1_COLL_RESULT_POLY_DIM hitDim = {};		//108byte
		std::shared_ptr<Model> model = {};			//�킩���
	};

	struct CollResultPoly {
		MV1_COLL_RESULT_POLY* hitDim = {};			//104byte
		std::shared_ptr<Model> model = {};			//�킩���
	};

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CheckCollisionModel();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CheckCollisionModel();

	/// <summary>
	/// ����������͈͂̃I�u�W�F�N�g�Ƃ̏Փ˔���𓾂�
	/// </summary>
	/// <param name="player">�v���C���[���Q��</param>
	/// <param name="moveVec">�v���C���[�̈ړ���</param>
	/// <param name="models">�v���C���[�ƏՓ˔�����s�����f��</param>
	void CheckCollisionPersonalArea(const std::shared_ptr<Player>& player, const std::shared_ptr<ObjectManager>& objManager);

	/// <summary>
	/// �Փ˂����ǂƏ��̃|���S�����𐔂���
	/// </summary>
	void CheckWallAndFloor();

	/// <summary>
	/// �Փ˂������f���̃|���S�����ǂ��𔻒f���A�ړ��x�N�g����␳����
	/// </summary>
	/// <param name="player">�v���C���[���Q��</param>
	void CheckCollisionWall(const std::shared_ptr<Player>& player);

	/// <summary>
	/// �ړ�����W�ŕǂƂ̏Փ˔���
	/// </summary>
	/// <param name="j">��</param>
	/// <param name="player">�v���C���[���Q��</param>
	void CollisionDetectionWithWallAfterMovement(int& j, const std::shared_ptr<Player>& player);

	/// <summary>
	/// �Փ˂������f���̃|���S���������𔻒f����
	/// </summary>
	/// <param name="player">�v���C���[���Q��</param>
	/// <param name="moveVec">�v���C���[�̈ړ���</param>
	/// <param name="jumpFlag">�v���C���[�̃W�����v�t���O</param>
	/// <param name="playerHeight">�v���C���[�̍���</param>
	void CheckCollisionFloor(const std::shared_ptr<Player>& player);

	/// <summary>
	/// checkCollisionPersonalArea�AcheckCollisionWall�AcheckCollisionFloor���Ăяo���B
	/// �Ō�A��L�̊֐��Ŏ擾�����l����������
	/// </summary>
	/// <param name="player">�v���C���[���Q�Ƃ���</param>
	void CheckCollision(const std::shared_ptr<Player>& player, const std::shared_ptr<ObjectManager>& objManager);

	/// <summary>
	/// �Փ˂����I�u�W�F�N�g�����z�����邩���f����
	/// </summary>
	void CheckStepDifference(const std::shared_ptr<Player>& player);

	/// <summary>
	/// �v���C���[�̉��ɉe���ǂ���`�悵�������߂�
	/// �v���C���[�̐^���̈�ԋ߂��|���S���̍������擾����
	/// </summary>
	/// <param name="player">�v���C���[�̃|�C���^</param>
	/// <param name="objManager">�I�u�W�F�N�g�}�l�[�W���[�̃|�C���^</param>
	void FindThePolygonBelowThePlayer(const std::shared_ptr<Player>& player, const std::shared_ptr<ObjectManager>& objManager);

	/// <summary>
	/// ���̂Ƃ̏Փ˔���
	/// </summary>
	/// <param name="player">�v���C���[�̃|�C���^</param>
	/// <param name="objManager">�I�u�W�F�N�g�}�l�[�W���[�̃|�C���^</param>
	void CheckCollCorpseModel(const std::shared_ptr<Player>& player, const std::shared_ptr<ObjectManager>& objManager);

private:

	int hitWallNum = 0;									//�ǂƏՓ˂����|���S���̐�
	int hitFloorNum = 0;								//���ƏՓ˂����|���S���̐�

	float objectHeightY_ = 0;							//�Փ˂����I�u�W�F�N�g�̍�����ۊǂ���

	bool isGoUpStep_ = false;							//�i������邱�Ƃ��o����
	bool moveFlag = false;								//���݈ړ����Ă��邩�̃t���O
	bool hitFlag_ = false;								//�Փ˂������̃t���O

	VECTOR oldPos = {};									//���݂̃v���C���[�̍��W
	VECTOR nowPos_ = {};									//�v���C���[�̈ړ��ʂƌ��݂̍��W�𑫂��Č���

	//���f���Ƃ̓����蔻��p���\�b�h
	std::list<CollModelState> hitDim_;					//���ƃ��f���̏Փ˔���p
	std::list<HITRESULT_LINE> hitLineResult_ = {};		//�|�W�S���Ɛ��̏Փ˔���p

	CollResultPoly wallHitDim_[max_hit_coll] = {};		//�ǂ̏Փˌ��ʗp�z��
	CollResultPoly floorHitDim_[max_hit_coll] = {};		//���̏Փˌ��ʗp�z��
};

