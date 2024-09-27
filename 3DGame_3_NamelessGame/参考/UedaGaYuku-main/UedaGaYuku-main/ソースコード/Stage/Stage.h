#pragma once

class Player;
class EnemyBase;

/// <summary>
/// �X�e�[�W�̍X�V�ƕ`����s���N���X
/// </summary>
class Stage
{
public:
	Stage();
	virtual ~Stage();
	void Draw();
	// �v���C���[�Ɠ����蔻������āA�␳�����ړ���̃|�W�V������Ԃ�
	VECTOR CheckPlayerCol(Player& player, const VECTOR& moveVec);
	// �G�l�~�[�Ɠ����蔻������āA�␳�����ړ���̃|�W�V������Ԃ�
	VECTOR CheckEnemyCol(EnemyBase& enemy, const VECTOR& moveVec);
	// �X�e�[�W�̔z�u�f�[�^���擾����


	// �X�e�[�W���f�����擾
	int GetStageHandle() const { return m_stageHandle; }

private:
	// ���o���ꂽ�|���S�����ǃ|���S�������|���S�����𔻒f����
	void AnalyzeWallAndFloor(MV1_COLL_RESULT_POLY_DIM hitDim, const VECTOR& checkPosition);
	// �v���C���[�ƕǃ|���S���Ƃ̓�������`�F�b�N����
	VECTOR CheckHitPlayerWithWall(Player& player, const VECTOR& checkPosition);
	// �v���C���[�Ə��|���S���Ƃ̓�������`�F�b�N����
	VECTOR CheckHitPlayerWithFloor(Player& player, const VECTOR& checkPosition);
	// �G�l�~�[�ƕǃ|���S���Ƃ̓�������`�F�b�N����
	VECTOR CheckHitEnemyWithWall(EnemyBase& enemy, const VECTOR& checkPosition);
	// �G�l�~�[�Ə��|���S���Ƃ̓�������`�F�b�N����
	VECTOR CheckHitEnemyWithFloor(EnemyBase& enemy, const VECTOR& checkPosition);

private:
	// �X�e�[�W���
	int m_wallNum;			// �ǃ|���S���̐�
	int	m_floorNum;			// ���|���S���̐�
	int m_stageHandle;		// �X�e�[�W��3D���f��
	int m_skydoomHandle;	// �X�J�C�h�[����3D���f��

	static const int MaxHitColl = 2048;			// ��������R���W�����|���S���̍ő吔
	MV1_COLL_RESULT_POLY* m_wall[MaxHitColl];	// �ǃ|���S���̍\���̂̃A�h���X��ۑ����Ă������߂̃|�C���^�z��
	MV1_COLL_RESULT_POLY* m_floor[MaxHitColl];	// ���|���S���̍\���̂̃A�h���X��ۑ����Ă������߂̃|�C���^�z��
};

