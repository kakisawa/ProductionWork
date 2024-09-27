#include "DxLib.h"
#include "Stage.h"
#include "Player.h"
#include "EnemyBase.h"
#include "Game.h"
#include <cmath>

// �萔
namespace
{
    // �X�e�[�W�T�C�Y
    const VECTOR kStageScale = VGet(15.0f, 15.0f, 15.0f);
    const VECTOR kSkydoomScale = VGet(15.0f, 15.0f, 15.0f);
    const VECTOR kStagePos = VGet(-10835.0f, 0.0f, 0.0f);
    const VECTOR kSkydoomPos = VGet(-2000.0f, 0.0f, 0.0f);

    // �����蔻��
    constexpr float kDefaultSize = 100.0f;	 // ���͂̃|���S�����o�Ɏg�p���鋅�̏����T�C�Y
    constexpr float kHitWidth = 10.0f;	     // �����蔻��J�v�Z���̔��a
    constexpr float kHitHeight = 20.0f;	     // �����蔻��J�v�Z���̍���
    constexpr float kHitBottom = -1.0f;	     // �����蔻��J�v�Z���̈ʒu
    constexpr float kHitBottom2 = -40.0f;	 // �����蔻��J�v�Z���̈ʒu
    constexpr int kHitTryNum = 16;		     // �ǉ����o�������̍ő厎�s��
    constexpr float kHitSlideLength = 1.0f;	 // ��x�̕ǉ����o�������ŃX���C�h�����鋗��
}


/// <summary>
/// �R���X�g���N�^
/// </summary>
Stage::Stage():
    m_wallNum(0),
    m_floorNum(0),
    m_floor(),
    m_wall()
{
	m_stageHandle = MV1LoadModel("data/Model/Stage/city.mv1");
    m_skydoomHandle = MV1LoadModel("data/Model/Stage/skydoom.mv1");

    MV1SetScale(m_stageHandle, kStageScale);
    MV1SetPosition(m_stageHandle, kStagePos);
    MV1SetScale(m_skydoomHandle, kSkydoomScale);
    MV1SetPosition(m_skydoomHandle, kSkydoomPos);

    // ���f���S�̂̃R���W�������̃Z�b�g�A�b�v
    MV1SetupCollInfo(m_stageHandle, -1);
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
Stage::~Stage()
{
    MV1DeleteModel(m_stageHandle);
    MV1DeleteModel(m_skydoomHandle);
}


/// <summary>
/// �`��
/// </summary>
void Stage::Draw()
{
    // �X�e�[�W�`��
    MV1DrawModel(m_stageHandle);
    MV1DrawModel(m_skydoomHandle);
}


/// <summary>
/// �v���C���[�Ɠ����蔻������āA�␳�����ړ���̃|�W�V������Ԃ�
/// </summary>
/// <param name="player">�v���C���[�Q��</param>
/// <param name="moveVec">�v���C���[�̈ړ���</param>
/// <returns>�ړ���̍��W</returns>
VECTOR Stage::CheckPlayerCol(Player& player, const VECTOR& moveVec)
{
    VECTOR oldPos = player.GetPos();		// �ړ��O�̍��W	
    VECTOR nextPos = VAdd(oldPos, moveVec);	// �ړ���̍��W

    // �v���C���[�̎��͂ɂ���X�e�[�W�|���S�����擾����
    auto hitDim = MV1CollCheck_Sphere(m_stageHandle, -1, oldPos, kDefaultSize + VSize(moveVec));

    // ���o���ꂽ�|���S�����ǃ|���S�������|���S�������ʂ��A�ۑ�����
    // MEMO:�ǃ|���S��(XZ���ʂɐ����ȃ|���S��)�A���|���S��(XZ���ʂɐ����łȂ��|���S��)
    AnalyzeWallAndFloor(hitDim, oldPos);

    // �ǃ|���S���Ƃ̓����蔻����`�F�b�N���A�v���C���[�̈ړ��x�N�g����␳����
    nextPos = CheckHitPlayerWithWall(player, nextPos);
    // ���|���S���Ƃ̓����蔻����`�F�b�N���A�v���C���[�̈ړ��x�N�g����␳����
    nextPos = CheckHitPlayerWithFloor(player, nextPos);

    // ���o�����v���C���[�̎��͂̃|���S�����̌�n��������
    MV1CollResultPolyDimTerminate(hitDim);

    return nextPos;
}


/// <summary>
/// �G�l�~�[�Ɠ����蔻������āA�␳�����ړ���̃|�W�V������Ԃ�
/// </summary>
/// <param name="enemy">�G�l�~�[�Q��</param>
/// <param name="moveVec">�G�l�~�[�̈ړ���</param>
/// <returns>�ړ���̍��W</returns>
VECTOR Stage::CheckEnemyCol(EnemyBase& enemy, const VECTOR& moveVec)
{
    VECTOR oldPos = enemy.GetPos();		    // �ړ��O�̍��W	
    VECTOR nextPos = VAdd(oldPos, moveVec);	// �ړ���̍��W

    // �G�l�~�[�̎��͂ɂ���X�e�[�W�|���S�����擾����
    auto hitDim = MV1CollCheck_Sphere(m_stageHandle, -1, oldPos, kDefaultSize + VSize(moveVec));

    // ���o���ꂽ�|���S�����ǃ|���S�������|���S�������ʂ��A�ۑ�����
    AnalyzeWallAndFloor(hitDim, oldPos);

    // �ǃ|���S���Ƃ̓����蔻����`�F�b�N���A�G�l�~�[�̈ړ��x�N�g����␳����
    nextPos = CheckHitEnemyWithWall(enemy, nextPos);
    // ���|���S���Ƃ̓����蔻����`�F�b�N���A�G�l�~�[�̈ړ��x�N�g����␳����
    nextPos = CheckHitEnemyWithFloor(enemy, nextPos);

    // ���o�����v���C���[�̎��͂̃|���S�����̌�n��������
    MV1CollResultPolyDimTerminate(hitDim);

    return nextPos;
}


/// <summary>
/// ���o���ꂽ�|���S�����ǃ|���S�������|���S�����𔻒f���A�ۑ�����
/// </summary>
/// <param name="hitDim">���ƃ��f���̓����蔻��</param>
/// <param name="checkPosition">�ړ���̍��W</param>
void Stage::AnalyzeWallAndFloor(MV1_COLL_RESULT_POLY_DIM hitDim, const VECTOR& checkPosition)
{
    // �ǃ|���S���Ə��|���S���̐�������������
    m_wallNum = 0;
    m_floorNum = 0;

    // ���o���ꂽ�|���S���̐������J��Ԃ�
    for (int i = 0; i < hitDim.HitNum; i++)
    {
        // XZ���ʂɐ������ǂ���
        // MEMO:�������ǂ����̓|���S���̖@����Y������0�Ɍ���Ȃ��߂����ǂ����Ŕ��f����
        bool isVertical = hitDim.Dim[i].Normal.y < 0.000001f && hitDim.Dim[i].Normal.y > -0.000001f;
        
        // XZ���ʂɐ����ȏꍇ(�ǃ|���S���Ɣ��f���ꂽ�ꍇ)
        if (isVertical)
        {
            // �ǃ|���S���Ɣ��f���ꂽ�ꍇ�ł��A�v���C���[��Y���W+1.0f��荂���|���S���̂ݓ����蔻����s��
            if (hitDim.Dim[i].Position[0].y > checkPosition.y + 1.0f ||
                hitDim.Dim[i].Position[1].y > checkPosition.y + 1.0f ||
                hitDim.Dim[i].Position[2].y > checkPosition.y + 1.0f)
            {
                // �|���S���̐����񋓂ł�����E���ɒB���Ă��Ȃ��ꍇ�A�|���S����z��ɒǉ�����
                if (m_wallNum < MaxHitColl)
                {
                    // �|���S���̍\���̂̃A�h���X��ǃ|���S���|�C���^�z��ɕۑ�����
                    m_wall[m_wallNum] = &hitDim.Dim[i];
                    m_wallNum++;
                }
            }
        }
        // XZ���ʂɐ����łȂ��ꍇ
        else
        {
            // �|���S���̐����񋓂ł�����E���ɒB���Ă��Ȃ��ꍇ�A�|���S����z��ɒǉ�
            if (m_floorNum < MaxHitColl)
            {
                // �|���S���̍\���̂̃A�h���X�����|���S���|�C���^�z��ɕۑ�����
                m_floor[m_floorNum] = &hitDim.Dim[i];
                m_floorNum++;
            }
        }
    }
}


/// <summary>
/// �v���C���[�ƕǃ|���S���Ƃ̓�������`�F�b�N����
/// </summary>
/// <param name="player">�v���C���[�Q��</param>
/// <param name="checkPosition">�ړ���̍��W</param>
/// <returns>�␳���ׂ��x�N�g��</returns>
VECTOR Stage::CheckHitPlayerWithWall(Player& player, const VECTOR& checkPosition)
{
    // �␳��̈ʒu
    VECTOR fixedPos = checkPosition;

    // �ǂ̐����Ȃ������牽�����Ȃ�
    if (m_wallNum == 0) return fixedPos;

    // �ǂ���̉����o���������s��
    for (int i = 0; i < kHitTryNum; i++)
    {
        // ������\���̂���ǃ|���S����S�Č���
        bool isHitWall = false;
        for (int i = 0; i < m_wallNum; i++)
        {
            // i�Ԗڂ̕ǃ|���S���̃A�h���X��ǃ|���S���|�C���^�z�񂩂�擾
            auto poly = m_wall[i];

            // �v���C���[�̏I�_
            VECTOR fixedEndPos = VAdd(fixedPos, VGet(0.0f, kHitHeight, 0.0f));
            // �v���C���[���ǂɓ������Ă��邩
            bool isHit = HitCheck_Capsule_Triangle(fixedPos, fixedEndPos, kHitWidth, poly->Position[0], poly->Position[1], poly->Position[2]);

            // �v���C���[�Ɠ������Ă���ꍇ
            if (isHit)
            {
                // �v���C���[��ǂ̖@�������Ɉړ�������
                fixedPos = VAdd(fixedPos, VScale(poly->Normal, kHitSlideLength));

                // �ړ������ǃ|���S���ƐڐG���Ă��邩�ǂ������肷��
                for (int j = 0; j < m_wallNum; j++)
                {
                    // �������Ă����烋�[�v�𔲂���
                    poly = m_wall[j];
                    if (isHit)
                    {
                        isHitWall = true;
                        break;
                    }
                }

                // �S�Ẵ|���S���Ɠ������Ă��Ȃ������烋�[�v�I��
                if (!isHitWall) break;
            }
        }

        // �S�Ă̕ǃ|���S���ƐڐG���Ȃ��Ȃ����烋�[�v���甲����
        if (!isHitWall) break;
    }

    return fixedPos;
}


 ///<summary>
 /// �v���C���[�Ə��|���S���Ƃ̓�������`�F�b�N����
 ///</summary>
 ///<param name="player">�v���C���[�Q��</param>
 ///<param name="checkPosition">�ړ���̍��W</param>
 ///<returns>�␳���ׂ��x�N�g��</returns>
VECTOR Stage::CheckHitPlayerWithFloor(Player& player, const VECTOR& checkPosition)
{
    VECTOR fixedPos = checkPosition;

    // ���̐����Ȃ������牽�����Ȃ�
    if (m_floorNum == 0) return fixedPos;

    bool isHitFloor = false;
    float maxY = 0.0f;

    for (int i = 0; i < m_floorNum; i++)
    {
        auto poly = m_floor[i]; // i�Ԗڂ̏��|���S���̃A�h���X

        HITRESULT_LINE lineResult{};  // �����ƃ|���S���Ƃ̓����蔻��̌��ʂ�������\����
        VECTOR topPos = VAdd(fixedPos, VGet(0.0f, kHitHeight, 0.0f));       // �v���C���[�̓��̐�̈ʒu
        VECTOR bottomPos = VAdd(fixedPos, VGet(0.0f, kHitBottom, 0.0f));    // �v���C���[�̑�����菭���Ⴂ�ʒu
        VECTOR bottomPos2 = VAdd(fixedPos, VGet(0.0f, kHitBottom2, 0.0f));  // �v���C���[�̑�����肳��ɒႢ�ʒu

        // ���̐悩�炻�������Ⴂ�ʒu�̊Ԃœ������Ă��邩�𔻒肷��
        lineResult = HitCheck_Line_Triangle(topPos, bottomPos2, poly->Position[0], poly->Position[1], poly->Position[2]);

        // ���ɓ��������|���S��������A���o�������|���S�����Ⴂ�ꍇ�������Ȃ�
        if (lineResult.HitFlag)
        {
            if (!(isHitFloor && maxY > lineResult.Position.y))
            {
                // �ڐG����Y���W��ۑ�����
                isHitFloor = true;
                maxY = lineResult.Position.y;
            }
        }
    }

    // ���|���S���ɓ��������ꍇ
    if (isHitFloor)
    {
        // �v���C���[�����ɐڐG������
        fixedPos.y = maxY;
    }

    return fixedPos;
}


/// <summary>
/// �G�l�~�[�ƕǃ|���S���Ƃ̓�������`�F�b�N����
/// </summary>
/// <param name="enemy">�G�l�~�[�Q��</param>
/// <param name="checkPosition">�ړ���̍��W</param>
/// <returns>�␳���ׂ��x�N�g��</returns>
VECTOR Stage::CheckHitEnemyWithWall(EnemyBase& enemy, const VECTOR& checkPosition)
{
    // �␳��̈ʒu
    VECTOR fixedPos = checkPosition;

    // �ǂ̐����Ȃ������牽�����Ȃ�
    if (m_wallNum == 0) return fixedPos;

    // �ǂ���̉����o���������s��
    for (int i = 0; i < kHitTryNum; i++)
    {
        // ������\���̂���ǃ|���S����S�Č���
        bool isHitWall = false;
        for (int i = 0; i < m_wallNum; i++)
        {
            // i�Ԗڂ̕ǃ|���S���̃A�h���X��ǃ|���S���|�C���^�z�񂩂�擾
            auto poly = m_wall[i];

            // �G�l�~�[�̏I�_
            VECTOR fixedEndPos = VAdd(fixedPos, VGet(0.0f, kHitHeight, 0.0f));
            // �G�l�~�[���ǂɓ������Ă��邩
            bool isHit = HitCheck_Capsule_Triangle(fixedPos, fixedEndPos, kHitWidth, poly->Position[0], poly->Position[1], poly->Position[2]);

            // �G�l�~�[�Ɠ������Ă���ꍇ
            if (isHit)
            {
                // �G�l�~�[��ǂ̖@�������Ɉړ�������
                fixedPos = VAdd(fixedPos, VScale(poly->Normal, kHitSlideLength));

                // �ړ������ǃ|���S���ƐڐG���Ă��邩�ǂ������肷��
                for (int j = 0; j < m_wallNum; j++)
                {
                    // �������Ă����烋�[�v�𔲂���
                    poly = m_wall[j];
                    if (isHit)
                    {
                        isHitWall = true;
                        break;
                    }
                }

                // �S�Ẵ|���S���Ɠ������Ă��Ȃ������烋�[�v�I��
                if (!isHitWall) break;
            }
        }

        // �S�Ă̕ǃ|���S���ƐڐG���Ȃ��Ȃ����烋�[�v���甲����
        if (!isHitWall) break;
    }

    return fixedPos;
}


/// <summary>
/// �G�l�~�[�Ə��|���S���Ƃ̓�������`�F�b�N����
/// </summary>
/// <param name="enemy">�G�l�~�[�Q��</param>
/// <param name="checkPosition">�ړ���̍��W</param>
/// <returns>�␳���ׂ��x�N�g��</returns>
VECTOR Stage::CheckHitEnemyWithFloor(EnemyBase& enemy, const VECTOR& checkPosition)
{
    VECTOR fixedPos = checkPosition;

    // ���̐����Ȃ������牽�����Ȃ�
    if (m_floorNum == 0) return fixedPos;

    bool isHitFloor = false;
    float maxY = 0.0f;

    for (int i = 0; i < m_floorNum; i++)
    {
        auto poly = m_floor[i]; // i�Ԗڂ̏��|���S���̃A�h���X

        HITRESULT_LINE lineResult{};  // �����ƃ|���S���Ƃ̓����蔻��̌��ʂ�������\����
        VECTOR topPos = VAdd(fixedPos, VGet(0.0f, kHitHeight, 0.0f));       // �G�l�~�[�̓��̐�̈ʒu
        VECTOR bottomPos = VAdd(fixedPos, VGet(0.0f, kHitBottom, 0.0f));    // �G�l�~�[�̑�����菭���Ⴂ�ʒu
        VECTOR bottomPos2 = VAdd(fixedPos, VGet(0.0f, kHitBottom2, 0.0f));  // �G�l�~�[�̑�����肳��ɒႢ�ʒu

        // ���̐悩�炻�������Ⴂ�ʒu�̊Ԃœ������Ă��邩�𔻒肷��
        lineResult = HitCheck_Line_Triangle(topPos, bottomPos2, poly->Position[0], poly->Position[1], poly->Position[2]);

        // ���ɓ��������|���S��������A���o�������|���S�����Ⴂ�ꍇ�������Ȃ�
        if (lineResult.HitFlag)
        {
            if (!(isHitFloor && maxY > lineResult.Position.y))
            {
                // �ڐG����Y���W��ۑ�����
                isHitFloor = true;
                maxY = lineResult.Position.y;
            }
        }
    }

    // ���|���S���ɓ��������ꍇ
    if (isHitFloor)
    {
        // �G�l�~�[�����ɐڐG������
        fixedPos.y = maxY;
    }

    return fixedPos;
}
