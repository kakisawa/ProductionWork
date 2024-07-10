#pragma once

class Player;

/// <summary>
/// �X�e�[�W�̍X�V�ƕ`����s���N���X
/// </summary>
class Stage
{
public:
	Stage();
	virtual ~Stage();
	void Draw();
	// �����蔻������āA�␳�����ړ���̃|�W�V������Ԃ�
	VECTOR CheckCollision(Player& player, const VECTOR& moveVector);

	// �X�e�[�W���f�����擾
	int GetStageHandle() const { return m_stageHandle; }

private:
	// ���o���ꂽ�|���S�����ǃ|���S�������|���S�����𔻒f����
	void AnalyzeWallAndFloor(MV1_COLL_RESULT_POLY_DIM hitDim, const VECTOR& checkPosition);
	// �ǃ|���S���Ƃ̓�������`�F�b�N����
	VECTOR CheckHitWithWall(Player& player, const VECTOR& checkPosition);
	// ���|���S���Ƃ̓�������`�F�b�N����
	VECTOR CheckHitWithFloor(Player& player, const VECTOR& checkPosition);

private:
	// �X�e�[�W���
	int m_wallNum;		// �ǃ|���S���̐�
	int	m_floorNum;		// ���|���S���̐�
	int m_stageHandle;	// �X�e�[�W��3D���f��

	static const int MaxHitColl = 2048;			// ��������R���W�����|���S���̍ő吔
	MV1_COLL_RESULT_POLY* m_wall[MaxHitColl];	// �ǃ|���S���̍\���̂̃A�h���X��ۑ����Ă������߂̃|�C���^�z��
	MV1_COLL_RESULT_POLY* m_floor[MaxHitColl];	// ���|���S���̍\���̂̃A�h���X��ۑ����Ă������߂̃|�C���^�z��
};

