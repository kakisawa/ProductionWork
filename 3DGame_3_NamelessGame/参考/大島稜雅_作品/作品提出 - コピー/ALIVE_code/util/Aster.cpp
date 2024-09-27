#include "Aster.h"
#include "../object/ObjectManager.h"
#include "Util.h"
#include <algorithm>

namespace {
	//�񎟌��z��̍ő吔 X
	constexpr int max_X = 11;

	//�񎟌��z��̍ő吔 Z
	constexpr int max_Z = 11;

	//�񎟌��z��̍ő�v�f��
	constexpr int max_Index = max_X * max_Z;

	//����
	constexpr int half = 2;

	//���̕�����
	constexpr int division_num = 32;

	//1�}�X�̏c�̃T�C�Y
	constexpr float height_size = 100.0f;

	//1�}�X�̉��̃T�C�Y
	constexpr float width_size = 100.0f;

	//���̔��a
	constexpr float sphere_radius = 16.0f;
}

//�R���X�g���N�^
Aster::Aster()
{
}

//�f�X�g���N�^
Aster::~Aster()
{
}

// �o�H�T�����s���z��̏�����
void Aster::ArrayInit(const VECTOR& pos)
{
	//�c���̔����̃T�C�Y���擾
	int halfX = max_X / half;
	int halfZ = max_Z / half;

	//���S���W
	VECTOR centerPos = {};

	//�v���C���[�𒆐S�Ƃ���max_X �~ max_Z�̃}�X�𐶐�����
	for (int i = 0; i < max_Index; i++)
	{
		//���S���W�̎擾
		centerPos.x = (i % max_X - halfX) * width_size + pos.x;
		centerPos.y = pos.y;
		centerPos.z = (i / max_Z - halfZ) * height_size + pos.z;

		//�}�X�z��̐ݒ�
		masu_[i].centerPos = centerPos;
		masu_[i].masuMode = MasuMode::normalMode;
		masu_[i].x = i % max_X;
		masu_[i].z = i / max_Z;
	}
}

//������
void Aster::Init()
{
   	route_.clear();
	scoreTable_.clear();

	for (auto& masu : masu_) 
	{
		if (masu.second.masuMode == MasuMode::doneMode) 
		{
			masu.second.masuMode = MasuMode::normalMode;
		}
	}
}

//�X�V
void Aster::Update()
{
	//�v���C���[�ƓG�̃C���f�b�N�X�������������ꍇ
	//���̃}�X�̒��S���W��Ԃ�
	if (currentPlayerIndex_ == currentEnemyIndex_) 
	{

		//�ŒZ���[�g���폜����
		route_.clear();

		//�ŒZ���[�g��T���ۂɃ}�X�̃��[�h��doneMode�ɂ�������
		//doneMode����normalMode�ɖ߂�
		for (auto& mode : masu_) {
			if (mode.second.masuMode != MasuMode::blockadeMode) 
			{
				mode.second.masuMode = MasuMode::normalMode;
			}
		}
	}
}

//�f�o�b�O�`��
void Aster::DebugDraw()
{
	//�f�o�b�O�p�`��
	for (int i = 0; i < max_Index; i++) 
	{
		if (masu_[i].masuMode == MasuMode::normalMode) 
		{
			DrawSphere3D(masu_[i].centerPos, sphere_radius, division_num, 0x00ff00, 0x00ff00, true);
		}
		else if(masu_[i].masuMode == MasuMode::blockadeMode) 
		{
			DrawSphere3D(masu_[i].centerPos, sphere_radius, division_num, 0xff0000, 0xff0000, true);
		}
		else 
		{
			DrawSphere3D(masu_[i].centerPos, sphere_radius, division_num, 0xffff00, 0xffff00, true);
		}
		
		VECTOR pos = ConvWorldPosToScreenPos(masu_[i].centerPos);
		DrawFormatStringF(pos.x, pos.y, 0x448844, "%d", i);
	}

	for (auto& result : debugScoreTable)
	{
		VECTOR pos = ConvWorldPosToScreenPos(masu_[result.second.destinationIndex].centerPos);
		DrawFormatStringF(pos.x, pos.y, 0xff0000, "S:%d", result.second.score);
	}
}

//���݂̃C���f�b�N�X�����X�V����
void Aster::CurrentIndexUpdate(const VECTOR& playerPos, const VECTOR& enemyPos)
{
	//�G�̃C���f�b�N�X���擾
	currentEnemyIndex_ = SearchCurrentIndex(enemyPos);

	//�v���C���[�̃C���f�b�N�X���擾
	currentPlayerIndex_ = SearchCurrentIndex(playerPos);
}

// �|�W�V�����������ɔz��̉��Ԗڂɑ��݂��邩�擾����
bool Aster::LocationInformation(const VECTOR& playerPos, const VECTOR& enemyPos)
{
	//enemy��player�������C���f�b�N�X���W
	int oldEnemyIndex = currentEnemyIndex_;
	int oldPlayerIndex = currentPlayerIndex_;

	//���݂̃C���f�b�N�X�����X�V����
	CurrentIndexUpdate(playerPos, enemyPos);

	//�v���C���[�̃C���f�b�N�X���W���O�̃t���[���Ɣ�ׂ��Ƃ�
	//�������A�ēx�o�H�T�����s��
	if (oldPlayerIndex != currentPlayerIndex_ && masu_[currentPlayerIndex_].masuMode != MasuMode::blockadeMode)
	{
		Init();
		return true;
	}

	return false;
}

// �ړI�n�̍��W���擾����
VECTOR Aster::GetDestinationCoordinates(const VECTOR& playerPos)
{
	//�ǐՃ��[�g�����݂��Ȃ��ꍇ�A���݂̍��W��Ԃ�
	if (route_.empty())
	{
		return masu_[currentEnemyIndex_].centerPos;
	}

	//�v���C���[���ǐՕs�\�ȃI�u�W�F�N�g�̏�ɗ����Ă��鎞
	//�G�l�~�[�������Ă���C���f�b�N�X�̒��S�|�W�V������Ԃ�
	if (masu_[currentPlayerIndex_].masuMode == MasuMode::blockadeMode) 
	{
		return masu_[currentEnemyIndex_].centerPos;
	}

	//�ڕW�C���f�b�N�X���W�̒��S�|�W�V����
	VECTOR targetPos = masu_[route_.front()].centerPos;

	//�v���C���[��ǐՂ��郋�[�g�Œʂ�������ǐՃ��[�g�z�񂩂�폜����
	if (currentEnemyIndex_ == route_.front()) 
	{
		route_.pop_front();
	}

	//�ڕW�|�W�V������Ԃ�
	return targetPos;
}

//�o�H�T��
void Aster::RouteSearch()
{
	//���݂̃G�l�~�[������C���f�b�N�X���W���擾
	int currentIndex = currentEnemyIndex_;
	routeSearchEnemyIndex_ = currentEnemyIndex_;

	//�G�l�~�[�ƃv���C���[�̃C���f�b�N�X���W����v���Ȃ���
	while (routeSearchEnemyIndex_ != currentPlayerIndex_)
	{
		if (masu_[routeSearchEnemyIndex_].masuMode != MasuMode::blockadeMode) {
			masu_[routeSearchEnemyIndex_].masuMode = MasuMode::doneMode;
		}

		SearchAroundSquares();

		//���ɃX�R�A���o���}�X�����ɃX�R�A���o���Ă������ꍇ
		//����ڂɃX�R�A�v�Z���s��ꂽ�����擾����
		for (auto& list : ShortestCandidateIndex_) {
			for (auto& index : list.second) {
				if (index == routeSearchEnemyIndex_) {
					moveCount_ = list.first;
				}
			}
		}
		moveCount_++;
	}

	moveCount_ = 0;

	int targetIndex = currentPlayerIndex_;

	//�ŒZ���[�g��scoreTable���璊�o����
	while (currentIndex != targetIndex)
	{
		targetIndex = scoreTable_[targetIndex].currentIndex;
		route_.push_front(targetIndex);
	}
	//�Ō�Ƀv���C���[������C���f�b�N�X���W��ǉ�����
	route_.push_back(currentPlayerIndex_);

	//���Z�b�g
	scoreTable_.clear();
	debugScoreTable.clear();
	ShortestCandidateIndex_.clear();
}

/// ���͂̏������݂��邩�T��
void Aster::SearchAroundSquares()
{
	//���[�A�㉺�̒[�̃}�X���擾
	int leftEnd = routeSearchEnemyIndex_ % max_X;
	int rightEnd = routeSearchEnemyIndex_ % max_X;
	int bottom = routeSearchEnemyIndex_ / max_X;
	int top = routeSearchEnemyIndex_ / max_X;

	//�}�X���Ȃ��Ƃ���͌��Ȃ��悤�ɂ���
	bool isCheckLeft = false;
	bool isCheckBottom = false;
	bool isCheckRight = false;
	bool isCheckTop = false;

	if (leftEnd == 0) {
		isCheckLeft = true;
	}
	if (bottom == 0) {
		isCheckBottom = true;
	}
	if (rightEnd == max_X - 1) {
		isCheckRight = true;
	}
	if (top == max_Z) {
		isCheckTop = true;
	}

	SearchSurroundingSquares(isCheckLeft, isCheckRight, isCheckTop, isCheckBottom);
}

//���͂̏��̃X�R�A���擾����
void Aster::SearchSurroundingSquares(const bool skipCheckLeft, const  bool skipCheckRight, const  bool skipCheckTop, const  bool skipCheckBottom)
{
	//���͂̏��̃C���f�b�N�X
	int left = routeSearchEnemyIndex_ - 1;
	int right = routeSearchEnemyIndex_ + 1;
	int top = routeSearchEnemyIndex_ + max_X;
	int bottom = routeSearchEnemyIndex_ - max_X;

	if (!skipCheckTop) {
		//��̏��̃X�R�A���擾����
		if (masu_[top].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::top, top);
		}
	}

	if (!skipCheckBottom) {
		//���̏��̃X�R�A���擾����
		if (masu_[bottom].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::bottom, bottom);
		}
	}

	if (!skipCheckLeft) {
		//���̏��̃X�R�A���擾����
		if (masu_[left].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::left, left);
		}
	}

	if (!skipCheckRight) {
		//�E�̏��̃X�R�A���擾����
		if (masu_[right].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::right, right);
		}
	}

	//������
	int score = 1000;

	//�ŒZ���[�g�����߂�ۂɈ�ԒႢ�X�R�A�̃C���f�b�N�X�}�X�����̒T���}�X�ɂ���
	for (auto& result : scoreTable_) {
		if (masu_[result.second.destinationIndex].masuMode == MasuMode::normalMode) {
			if (score > result.second.score) {
				score = result.second.score;
				routeSearchEnemyIndex_ = result.second.destinationIndex;
			}
		}
	}

	//�f�o�b�O�p
	debugScoreTable = scoreTable_;
}

//����̏��̃X�R�A���擾����
void Aster::ScoreCaluculation(const Direction direction, const  int index)
{
	//����R�X�g
	int estimationCostX = 0;
	int estimationCostZ = 0;

	//X��Z�ǂ��Ɉړ�������
	estimationCostX = masu_[currentPlayerIndex_].x - masu_[index].x;
	estimationCostZ = masu_[currentPlayerIndex_].z - masu_[index].z;

	//����R�X�g��direction�̔z��ɓ����
	if (estimationCostX < 0 || estimationCostZ < 0)
	{
		if (estimationCostX < 0) {
			estimationCostX = -(std::min)(estimationCostX, 0);
		}
		if (estimationCostZ < 0) {
			estimationCostZ = -(std::min)(estimationCostZ, 0);
		}
		
		//�l�����̎�
		scoreTable_[index].estimationCost = estimationCostX + estimationCostZ;
	}
	else 
	{
		//�l�����̎�
		scoreTable_[index].estimationCost = estimationCostX + estimationCostZ;
	}

	//�ړ��ʂ��J�E���g����
	if (masu_[index].masuMode == MasuMode::normalMode) 
	{
		scoreTable_[index].moveCost = moveCount_;
		scoreTable_[index].score = scoreTable_[index].moveCost + scoreTable_[index].estimationCost;
		scoreTable_[index].destinationIndex = index;
		scoreTable_[index].currentIndex = routeSearchEnemyIndex_;
		scoreTable_[index].dir = direction;
	}

	ShortestCandidateIndex_[moveCount_].push_back(index);
}

//���W���珡�̃C���f�b�N�X���擾����
int Aster::SearchCurrentIndex(const VECTOR& pos)
{
	int index = 0;
	float min = 1000.0f;
	float distanceSize = 0.0f;

	//�����̍��W�����ԋ߂��}�X�̒��S������index�ԍ����擾����
	for (int i = 0; i < max_Index; i++)
	{
		//���S����G�̃|�W�V�����̋������Ƃ�
		distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(masu_[i].centerPos, pos);

		//�ߋ��ŒZ�ɋ߂����ʂƔ�ׂ�
		if (min > distanceSize)
		{
			//�ߋ��ŒZ���X�V
			min = distanceSize;

			//�C���f�b�N�X���擾
			index = i;
		}
	}

	//�C���f�b�N�X���擾
	return index;
}

//�G����v���C���[�Ɍ��������[�g���Ȃ����擾����
bool Aster::GetIsRouteEmpty()
{
	//���[�g���������Atrue��Ԃ�
	if (route_.empty())
	{
		return true;
	}

	return false;
}

//�����̃|�W�V�����̃}�X��blockmode�����f����
bool Aster::SearchBlockadeMode(const VECTOR& pos)
{
	//�����̍��W�̃C���f�b�N�X���W���擾
	int pointIndex = SearchCurrentIndex(pos);

	//�擾�����C���f�b�N�X���W�̏���blockadeMode��������true��Ԃ�
	if (masu_[pointIndex].masuMode == MasuMode::blockadeMode)
	{
		return true;
	}

	return false;
}