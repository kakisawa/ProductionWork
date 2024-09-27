#pragma once
#include <list>
#include <DxLib.h>
#include <unordered_map>

class Aster
{
private:
	//�������ɕۑ�����Ȃ�(�R���p�C�����ɓW�J)
	enum class MasuMode {
		normalMode,				//�ʏ탂�[�h
		doneMode,				//�ς݃��[�h
		blockadeMode,			//�������[�h
	};

	//�������ɕۑ�����Ȃ�(�R���p�C�����ɓW�J)
	enum class Direction {
		left,					//��
		topLeft,				//����
		top,					//��
		topRight,				//�E��
		right,					//�E
		bottomRight,			//�E��
		bottom,					//��
		bottomLeft,				//����
	};

	//�v�@20byte
	struct MasuState {
		VECTOR centerPos;		//���S���W			//12byte
		MasuMode masuMode;		//���̃��[�h		//�������ɕۑ�����Ȃ�(�R���p�C�����ɓW�J)
		int x;					//2�����z���X		//4byte
		int z;					//2�����z���Y		//4byte
	};

	//�v�@20byte
	struct Score {
		int moveCost;			//�ړ��R�X�g								//4byte
		int estimationCost;		//����R�X�g								//4byte
		int score;				//�v���C���[��ǐՂ��邤���œ��_��t����	//4byte
		int currentIndex;		//�ړ���̃C���f�b�N�X						//4byte
		int destinationIndex;	//�ړ���̃C���f�b�N�X						//4byte
		Direction dir;			//���p										//�������ɕۑ�����Ȃ�(�R���p�C�����ɓW�J)
	};
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Aster();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Aster();

	/// <summary>
	/// �o�H�T�����s���z��̏�����
	/// </summary>
	/// <param name="pos">�|�W�V����</param>
	void ArrayInit(const VECTOR& pos);

	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void DebugDraw();

	/// <summary>
	/// ���݂̃C���f�b�N�X�����X�V����
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	/// <param name="enemyPos">�G�̃|�W�V����</param>
	void CurrentIndexUpdate(const VECTOR& playerPos, const VECTOR& enemyPos);

	/// <summary>
	/// �|�W�V�����������ɔz��̉��Ԗڂɑ��݂��邩�擾����
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	/// <param name="enemyPos">�G�̃|�W�V����</param>
	/// <returns>�v���C���[�͈ړ������� true:�����@false�F���Ă��Ȃ�</returns>
	bool LocationInformation(const VECTOR& playerPos, const VECTOR& enemyPos);

	/// <summary>
	/// �ړI�n�̍��W���擾����
	/// </summary>
	/// /// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	/// <returns>�ړI�n�̃|�W�V����</returns>
	VECTOR GetDestinationCoordinates(const VECTOR& playerPos);

	/// <summary>
	/// �o�H�T��
	/// </summary>
	void RouteSearch();

	/// <summary>
	/// ���͂̏������݂��邩�T��
	/// </summary>
	void SearchAroundSquares();

	/// <summary>
	/// ���͂̏�����������
	/// </summary>
	void SearchSurroundingSquares(const bool skipCheckLeft, const bool skipCheckRight, const  bool skipCheckTop, const  bool skipCheckBottom);

	/// <summary>
	/// ���̃X�R�A���擾����
	/// </summary>
	/// <param name="direction">����</param>
	/// <param name="index">���̔ԍ�</param>
	void ScoreCaluculation(const Direction direction, const int index);

	/// <summary>
	/// ���W���珡�̃C���f�b�N�X���擾����
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	int SearchCurrentIndex(const VECTOR& pos);

	/// <summary>
	/// �G����v���C���[�Ɍ��������[�g���Ȃ����擾����
	/// </summary>
	/// <returns>true:�Ȃ��@false:����</returns>
	bool GetIsRouteEmpty();

	/// <summary>
	/// �����̃|�W�V�����̃}�X��blockmode�����f����
	/// </summary>
	/// <param name="pos">�|�W�V����</param>
	/// <returns>true�Fblockmode�@false�Fblockmode�ł͂Ȃ�</returns>
	bool SearchBlockadeMode(const VECTOR& pos);
private:

	int routeSearchEnemyIndex_ = 0;										//�ŒZ�����𒲂ׂ邽�߂ɓG�̃C���f�b�N�X���X�V����
	int currentEnemyIndex_ = 0;											//���݂̓G�̃C���f�b�N�X
	int currentPlayerIndex_ = 0;										//�v���C���[�̃C���f�b�N�X
	int moveCount_ = 0;													//�ړ���

	std::list<int> route_;												//�ŒZ���[�g��ۑ�����

	std::unordered_map<int, MasuState> masu_;							//�ړ��͈�
	std::unordered_map<int, Score> scoreTable_;							//�ړ��͈͂̏��ɃX�R�A������
	std::unordered_map<int, Score> debugScoreTable;						//�f�o�b�O�p�̃X�R�A�e�[�u��(�폜���Ă悢)
	std::unordered_map<int, std::list<int>> ShortestCandidateIndex_;	//�ŒZ���[�g�̌��Ƃ��Ăł��C���f�b�N�X�Ɖ���ڂɍŒZ���Ƃ��ďo���ꂽ���ۑ�����
};

