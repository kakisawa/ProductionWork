#include "Player.h"
#include "DxLib.h"
#include "PlayerState.h"
#include "../Model.h"
#include "../GameMap.h"
#include "../../Util/Pad.h"
#include "../../Object/Camera.h"
#include "../../Util/Effect.h"
#include "../Enemy/EnemyRight.h"
#include "../Enemy/EnemyLeft.h"
#include "../../Manager/SoundManager.h"
<<<<<<< HEAD
#include <cmath>
#include <vector>
#include <cassert>
=======
#include "../../Util/Effect.h"
#include <cassert>
#include <cmath>
#include <vector>

bool col;
>>>>>>> 60071db8ef0a37b71e4c4659020bfdc349240f1e

namespace {
	// UI�摜
	const char* const kUI[5]{
		"data/Image/GameScene/UI/Player/HPOurGauge.png",// HPUI(�O��)
		"data/Image/GameScene/UI/Player/HPInGauge.png",	// HPUI(����)
		"data/Image/GameScene/UI/Player/NameBg.png",	// ���O�w�iUI
		"data/Image/GameScene/UI/Player/Fukuoka.png",	// ���OUI
		"data/Image/GameScene/UI/Player/Face.png"		// ��A�C�R��
	};

	const char* const kModelPlayer = "data/model/RogueHooded.mv1";	// ���f���̃t�@�C����

	constexpr int	kMaxHp = 100;				// �̗͍ő�l
	constexpr int	kSordDamage = 20;			// ���̍U����
	constexpr int	kBowDamage = 1;				// �|�̍U����

	constexpr float kInitAngle = -DX_PI_F / 2.0f * 90.0f;	// �v���C���[�̏����p�x*90(�����𔽑΂ɂ���)
	constexpr float kModelSize = 5.0f;			// ���f���̃T�C�Y
	constexpr float kSpeed = 0.7f;				// �v���C���[�ړ����x
	constexpr float kAttackSpeed = 0.5f;		// �v���C���[�U�����̉����x
	constexpr float	kAngleSpeed = 0.2f;			// �p�x�ω����x
	constexpr float	kJumpPower = 1.8f;			// �W�����v��
	constexpr float	kGravity = 0.05f;			// �d��
<<<<<<< HEAD
	constexpr float kEffectHeight = 10.0f;		// �G�t�F�N�g��\�����鍂��
=======
	constexpr int	kMaxHp = 100;				// �̗͍ő�l
	constexpr int	kSordDamage = 20;			// ���̍U����
	constexpr int	kBowDamage = 5;			// �|�̍U����

	constexpr float kEffectHeight = 10.0f;			// �G�t�F�N�g��\�����鍂��
>>>>>>> 60071db8ef0a37b71e4c4659020bfdc349240f1e

	// �A�C�R���ʒu
	constexpr int kFaceUIPosX = 0;
	constexpr int kFaceUIPosY = 30;
	// HP�Q�[�W(�O��)�ʒu
	constexpr int kHpGaugeUIPosX = 130;
	constexpr int kHpGaugeUIPosY = 85;
	// HP�Q�[�W(����)�E���ʒu(Exted�p�E�[���W)
	constexpr int kHpGaugePosX = 852;
	constexpr int kHpGaugePosY = kHpGaugeUIPosY + 42;
	// ���O�w�i�ʒu
	constexpr int kNameBgX = 130;
	constexpr int kNameBgY = 10;
	// ���O�ʒu
	constexpr int kNameX = kNameBgX + 80;
	constexpr int kNameY = kNameBgY + 10;

	const VECTOR kUpPos = VGet(0.0f, 7.0f, 0.0f);		// �v���C���[�̓����蔻���
	const VECTOR kAttackRange = VGet(0.0f, 0.0f,13.0f);	// �v���C���[�̍U�������蔻��
	constexpr float kColRadius = 2.5;					// �v���C���[�̓����蔻�蔼�a
	constexpr float kAttackColRadius = 3.0;				// �v���C���[�̍U�������蔻�蔼�a

	// �������p�l
	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// �x�N�g���̏�����
	constexpr float kInitFloat = 0.0f;				// float�l�̏�����
	constexpr int kInitInt = 0;						// int�l�̏�����

	int waitTime = 0;								// �|��A���Ō����̃C���^�[�o��
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Player::Player() :
	m_angle(kInitFloat),
	m_gravity(kGravity),
	m_addDamage(kInitInt),
	m_jumpPower(kInitFloat),
	m_multiAttack(kInitInt),
	m_hp(kMaxHp),
	m_isAttack(false), 
	m_isFirstAttack(false),
<<<<<<< HEAD
	m_isNextAttackFlag(false),
=======
>>>>>>> 60071db8ef0a37b71e4c4659020bfdc349240f1e
	m_isSordAttackDamage(false),
	m_isBowAttackDamage(false),
	m_isWalk(false),
	m_isJump(false),
	m_isDeath(false),
<<<<<<< HEAD
	m_isCol(false),
	m_isBodyCol(false),
	m_isSordAttackToLeftEnemy(false),
	m_isSordAttackToRightEnemy(false),
	m_isBowAttackToLeftEnemy(true),
	m_pos(kInitVec),
	m_move(kInitVec),
	m_UpPos(kInitVec),
	m_targetDir(kInitVec),
	m_vecToEnemy(kInitVec),
	m_attackRange(kInitVec),
	m_attackKind(AttackKind::kNone)
=======
	m_isAttackLeft(false),
	m_isAttackRight(false),
	m_isAttackCancel(false),
	m_isLeftEnemyAttack(true),
	m_jumpPower(0.0f),
	m_multiAttack(0),
	m_pos(kInitVec),
	m_move(kInitVec),
	m_UpPos(kInitVec),
	m_vecToEnemy(kInitVec),
	m_hp(kMaxHp),
	m_targetDir(VGet(0.0f, 0.0f, 0.0f)),
	isCol(false)
>>>>>>> 60071db8ef0a37b71e4c4659020bfdc349240f1e
{
	// UI�摜�̓ǂݍ���
	for (int i = 0; i < m_uiGraph.size(); i++){
		m_uiGraph[i] = LoadGraph(kUI[i]);
		assert(m_uiGraph[i] != -1);
	}

	//���f���C���X�^���X�쐬
	m_pModel = std::make_shared<Model>(kModelPlayer);
	// �A�C�h����Ԃ̃A�j���[�V�������Đ�������
	m_pModel->SetAnim(m_animData.kIdle, false, true);	

	//�X�e�C�g�N���X�̃C���X�^���X����
	m_pState = std::make_shared<PlayerState>();
	m_pEffect = std::make_shared<Effect>();

	// �X�e�C�g�N���X�̃C���X�^���X�ǉ�
	m_pState->AddState([=] { IdleStateUpdate(); }, [=] { IdleStateInit(); }, PlayerState::State::kIdle);					// �ҋ@���
	m_pState->AddState([=] { WalkStateUpdate(); }, [=] { WalkStateInit(); }, PlayerState::State::kWalk);					// �������
	m_pState->AddState([=] { JumpStateUpdate(); }, [=] { JumpStateInit(); }, PlayerState::State::kJump);					// �W�����v���
	m_pState->AddState([=] { AttackSordStateUpdate(); }, [=] { AttackSordStateInit(); }, PlayerState::State::kAttackSord);	// ���U�����
	m_pState->AddState([=] { AttackBowStateUpdate(); }, [=] { AttackBowStateInit(); }, PlayerState::State::kAttackBow);		// �|�U�����

	//�����X�e�C�g�Z�b�g
	m_pState->SetState(PlayerState::State::kIdle);


	// �T�E���h
	m_pSound = new SoundManager;
<<<<<<< HEAD
=======
	m_pSound->Init();
	m_pSound->LoadSE(SoundManager::SE_Type::kSordSE);
	m_pSound->LoadSE(SoundManager::SE_Type::kBowSE);

	col = false;
>>>>>>> 60071db8ef0a37b71e4c4659020bfdc349240f1e
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{
	delete m_pSound;
	m_pSound = nullptr;
}

/// <summary>
/// ������
/// </summary>
void Player::Init(std::shared_ptr<GameMap> pMap)
{
	// �v���C���[�����ݒ�
	m_pModel->SetSize(VGet(kModelSize, kModelSize, kModelSize));
	m_pModel->SetRota(VGet(0.0f, kInitAngle, 0.0f));
	m_pModel->SetPos(m_pos);

	// �}�b�v�̒[
	mp.leftBack = pMap->GetMapLeftBack();
	mp.rightFront = pMap->GetMapRightFront();

<<<<<<< HEAD
	// �T�E���h
	m_pSound->Init();
	// �g�p�T�E���h�̃��[�h
	m_pSound->LoadSE(SoundManager::SE_Type::kSordSE);
	m_pSound->LoadSE(SoundManager::SE_Type::kBowSE);

	// �G�t�F�N�g
=======
>>>>>>> 60071db8ef0a37b71e4c4659020bfdc349240f1e
	m_pEffect->Init();
}


/// <summary>
/// �X�V
/// </summary>
void Player::Update(const Camera& camera, const EnemyRight& enemyR, const EnemyLeft& enemyL)
{
	m_isCol = false;
	m_isBodyCol = false;
	// �p�b�h���͂ɂ���Ĉړ��p�����[�^��ݒ肷��
	VECTOR	upMoveVec;		// �����{�^���u���v����͂������Ƃ��̃v���C���[�̈ړ������x�N�g��
	VECTOR	leftMoveVec;	// �����{�^���u���v����͂������Ƃ��̃v���C���[�̈ړ������x�N�g��

	//���݂̓G�Ƃ̋��������߂�
	NearByEnemy(enemyR, enemyL);
<<<<<<< HEAD

	// �ړ�����
	OldMoveValue(camera, upMoveVec, leftMoveVec);

	// �v���C���[�̈ړ������Ƀ��f���̕������߂Â���
	Angle();
=======

	// �X�e�C�g�̍X�V
	m_pState->Update();

	// �v���C���[�̈ړ������Ƀ��f���̕������߂Â���
	Angle();

	// �v���C���[�̏�ԍX�V
	// �U������
	Attack(enemyR, enemyL);
	// �ړ�����
	OldMoveValue(camera, upMoveVec, leftMoveVec);

	// ���f���̃A�b�v�f�[�g
	m_pModel->Update();

	
>>>>>>> 60071db8ef0a37b71e4c4659020bfdc349240f1e

	// �v���C���[�̏�ԍX�V
	// �U������
	Attack(enemyR, enemyL);

	// ���W�X�V
	Move(m_move, enemyR, enemyL);

	// ���S�m�F����
	Death();

	// �����蔻��̍X�V����
	ColUpdate();

	// �X�e�C�g�̍X�V
	m_pState->Update();

<<<<<<< HEAD
	// ���f���̍X�V
	m_pModel->Update();

	// �G�t�F�N�g�̍X�V
	m_pEffect->Update();
=======
	m_colSphere.UpdateCol(m_pos, m_UpPos, m_attackRange,
		kColRadius, kAttackColRadius);

	m_pEffect->Update();		// �G�t�F�N�g�X�V
>>>>>>> 60071db8ef0a37b71e4c4659020bfdc349240f1e
}

/// <summary>
/// �`��
/// </summary>
void Player::Draw()
{
	// HP�Q�[�W�`��
	DrawExtendGraph(kHpGaugeUIPosX, kHpGaugeUIPosY, kHpGaugeUIPosX + (kHpGaugePosX * (m_hp * 0.01f)), kHpGaugePosY,m_uiGraph[1], true);
	DrawGraph(kHpGaugeUIPosX, kHpGaugeUIPosY, m_uiGraph[0], true);

	// �v���C���[���`��
	DrawGraph(kNameBgX, kNameBgY, m_uiGraph[2], true);		// ���O�q�[
	DrawGraph(kNameX, kNameY, m_uiGraph[3], true);			// ���O
	DrawGraph(kFaceUIPosX, kFaceUIPosY, m_uiGraph[4], true);// �A�C�R��

	// ���f���̕`��
	m_pModel->Draw();

<<<<<<< HEAD
	// �G�t�F�N�g�̕`��
	m_pEffect->Draw();
=======
	m_pEffect->Draw();			 // �G�t�F�N�g�`��
>>>>>>> 60071db8ef0a37b71e4c4659020bfdc349240f1e

#ifdef _DEBUG
	m_colSphere.DrawMain(0xff0000, false);	// �����蔻��`��
	m_colSphere.DrawAttack(0x0000ff, false);	// �����蔻��`��

	DrawFormatString(0, 200, 0xffffff, "Player:m_move.x,y,z=%.2f,=%.2f,=%.2f", m_move.x, m_move.y, m_move.z);

	DrawFormatString(0, 180, 0xffffff, "Player:m_pos.x,y,z=%.2f,=%.2f,=%.2f", m_pos.x, m_pos.y, m_pos.z);
	DrawFormatString(0, 220, 0xffffff, "Player:m_hp=%d", m_hp);

	DrawFormatString(0, 280, 0xffffff, "State=%d", m_pState->GetState());
	DrawFormatString(0, 300, 0xffffff, "m_isWalk=%d", m_isWalk);
	DrawFormatString(0, 320, 0xffffff, "m_angle=%.2f", m_angle);

<<<<<<< HEAD
	DrawFormatString(0, 360, 0xffffff, "m_isLeftEnemyAttack=%d", m_isBowAttackToLeftEnemy);

	DrawFormatString(0, 420, 0xffffff, "m_animSpeed=%.2f", m_pModel->GetAnimSpeed());

	if(m_isBodyCol) {
=======
	DrawFormatString(0, 360, 0xffffff, "m_isLeftEnemyAttack=%d", m_isLeftEnemyAttack);

	DrawFormatString(0, 420, 0xffffff, "m_animSpeed=%.2f", m_pModel->GetAnimSpeed());

	if(col) {
>>>>>>> 60071db8ef0a37b71e4c4659020bfdc349240f1e
		DrawString(0, 500, "�������Ă���", 0xffffff);
	}
	else{
		DrawString(0, 500, "�������Ă��Ȃ�", 0xffffff);
	}

	if (m_isCol){
		DrawString(0, 520, "�������Ă���", 0xffffff);
	}
	else{
		DrawString(0, 520, "�������Ă��Ȃ�", 0xffffff);
	}
#endif // DEBUG
}

/// <summary>
/// �I��
/// </summary>
void Player::End()
{
	// UI�摜�̍폜
	for (int i = 0; i < m_uiGraph.size(); i++){
		DeleteGraph(m_uiGraph[i]);
	}

	// �T�E���h�̉��
	m_pSound->ReleaseSound();
}

<<<<<<< HEAD
/// <summary>
/// �����̋߂��G��T��
/// </summary>
/// <param name="enemyR">�E�̓G</param>
/// <param name="enemyL">���̓G</param>
void Player::NearByEnemy(const EnemyRight& enemyR, const EnemyLeft& enemyL)
{
	// �v���C���[����E�̓G�܂ł̋���
=======
void Player::NearByEnemy(const EnemyRight& enemyR, const EnemyLeft& enemyL)
{
>>>>>>> 60071db8ef0a37b71e4c4659020bfdc349240f1e
	double distance1 = sqrt((enemyR.GetPos().x - m_pos.x) * (enemyR.GetPos().x - m_pos.x) +
		(enemyR.GetPos().y - m_pos.y) * (enemyR.GetPos().y - m_pos.y)+
		(enemyR.GetPos().z - m_pos.z) * (enemyR.GetPos().z - m_pos.z));

<<<<<<< HEAD
	// �v���C���[���獶�̓G�܂ł̋���
=======
>>>>>>> 60071db8ef0a37b71e4c4659020bfdc349240f1e
	double distance2 = sqrt((enemyL.GetPos().x - m_pos.x) * (enemyL.GetPos().x - m_pos.x) +
		(enemyL.GetPos().y - m_pos.y) * (enemyL.GetPos().y - m_pos.y) +
		(enemyL.GetPos().z - m_pos.z) * (enemyL.GetPos().z - m_pos.z));

<<<<<<< HEAD
	// �������������ق��̒l������
	double nearPos = min(distance1, distance2);

	// �E�̓G�̕����߂������ꍇ
	if (nearPos == distance1)
	{
		// �E�̓G���������Ă�����
		if(enemyR.GetAlive())
		{
			// �|�̑_�����E�̓G�ɂ���
			m_isBowAttackToLeftEnemy = false;
		}
		else { // �����Ă��Ȃ�������
			// �|�̑_�������̓G�ɂ���
			m_isBowAttackToLeftEnemy = true;
		}
	}
	else{ // �E�̓G�̕����߂�������
		
		// ���̓G���������Ă�����
		if (enemyL.GetAlive()) {
			// �|�̑_�������̓G�ɂ���
			m_isBowAttackToLeftEnemy = true;
		}
		else { // �����Ă��Ȃ�������
			// �|�̑_�����E�̓G�ɂ���
			m_isBowAttackToLeftEnemy = false;
		}
	}
}

/// <summary>
/// �W�����v�X�e�C�g�̏�����
/// </summary>
=======
	double nearPos = min(distance1, distance2);

	if (nearPos == distance1)
	{
		m_isLeftEnemyAttack = false;
	}
	else {
		m_isLeftEnemyAttack = true;
	}
}

>>>>>>> 60071db8ef0a37b71e4c4659020bfdc349240f1e
void Player::JumpStateInit()
{
	m_isJump = true;
	m_jumpPower = kJumpPower;
}

/// <summary>
/// ���U���̏�����
/// </summary>
void Player::AttackSordStateInit()
{
	m_isAttack = true;
	m_multiAttack = 0;
	m_isNextAttackFlag = false;
	m_isFirstAttack = true;
	m_attackKind = AttackKind::kAttackSord;
}

/// <summary>
/// �|�U���̏�����
/// </summary>
void Player::AttackBowStateInit()
{
	m_isAttack = true;
	m_isNextAttackFlag = false;
	m_isFirstAttack = true;
	m_attackKind = AttackKind::kAttackBow;
}

/// <summary>
/// �ҋ@��Ԃ̍X�V����
/// </summary>
void Player::IdleStateUpdate()
{
	// �A�j���[�V������ҋ@���[�V�����ɕύX
	m_pModel->ChangeAnim(m_animData.kIdle, true, false, 0.5f);
}

/// <summary>
/// ������Ԃ̍X�V����
/// </summary>
void Player::WalkStateUpdate()
{
	// �A�j���[�V������������[�V�����ɕύX
	m_pModel->ChangeAnim(m_animData.kWalk, true, false, 0.5f);
}

/// <summary>
/// �W�����v��Ԃ̍X�V����
/// </summary>
void Player::JumpStateUpdate()
{
	// �A�j���[�V�������W�����v���[�V�����ɕύX
	m_pModel->ChangeAnim(m_animData.kJump, false, false, 1.0f);
}

/// <summary>
/// ���U����Ԃ̍X�V
/// </summary>
void Player::AttackSordStateUpdate()
{
	// �A�j���[�V�����ύX
	switch (m_multiAttack)
	{
	case 0:
		m_pModel->ChangeAnim(m_animData.kAttack1, false, false, 1.0f);
		break;
	case 1:
		m_pModel->ChangeAnim(m_animData.kAttack2, false, false, 1.0f);
		break;
	case 2:
		m_pModel->ChangeAnim(m_animData.kAttack3, false, false, 1.0f);
		break;
	case 3:
		m_pModel->ChangeAnim(m_animData.kAttack4, false, false, 1.0f);
		break;
	default:
		break;
	}

<<<<<<< HEAD
	if (Pad::IsTrigger(PAD_INPUT_X) && !m_isNextAttackFlag){
=======
	if (Pad::IsTrigger(PAD_INPUT_X) && !m_isNextAttackFlag)
	{

>>>>>>> 60071db8ef0a37b71e4c4659020bfdc349240f1e
		if (!m_isFirstAttack)
		{
			m_isNextAttackFlag = true;
		}
		m_isFirstAttack = false;
	}

	if (m_pModel->IsAnimEnd())
	{
		// �A�j���[�V�������I������i�K�Ŏ��̍U���t���O�������Ă��Ȃ�������
		if (!m_isNextAttackFlag)
		{
			m_isAttack = false;
			m_multiAttack = 0;
			m_pState->EndState();
		}

		// �A�j���[�V�������I������i�K�Ŏ��̍U���t���O�������Ă�����
		if (m_isNextAttackFlag)
		{
			m_isNextAttackFlag = false;
			m_multiAttack++;
		}
	}

	if (m_multiAttack >= 4)
	{
		m_multiAttack = 0;
	}
}

/// <summary>
/// �|�U��
/// </summary>
void Player::AttackBowStateUpdate()
{
	bool loop = false;
	if (Pad::IsPress(PAD_INPUT_B))	// B�{�^��
	{
		loop = true;

		waitTime--;
		if (waitTime <= 0)
		{
			m_pSound->PlaySE(SoundManager::SE_Type::kBowSE, DX_PLAYTYPE_BACK);
			m_isBowAttackDamage = true;
			waitTime = 20;
		}
	}

	m_pModel->ChangeAnim(m_animData.kAttackBow, loop, false, 0.5f);

	// �A�j���[�V�������I�������
	if (!loop)
	{
		m_isAttack = false;
		m_pState->EndState();
		waitTime = 0;
	}
}

/// <summary>
/// �ړ��p�����[�^�̐ݒ�
/// </summary>
void Player::OldMoveValue(const Camera& camera, VECTOR& upMoveVec, VECTOR& leftMoveVec)
{
	// �v���C���[�̈ړ������̃x�N�g�����Z�o
	// �����{�^���u���v���������Ƃ��̃v���C���[�̈ړ��x�N�g���̓J�����̎�����������x�����𔲂�������
	upMoveVec = VSub(camera.GetTarget(), camera.GetPosition());
	upMoveVec.y = 0.0f;

	// �����{�^���u���v���������Ƃ��̃v���C���[�̈ړ��x�N�g���͏���������Ƃ��̕����x�N�g���Ƃx���̃v���X�����̃x�N�g���ɐ����ȕ���
	leftMoveVec = VCross(upMoveVec, VGet(0.0f, kSpeed, 0.0f));

	// �ړ��l�������l�ɖ߂�
	m_move = VGet(0.0f, 0.0f, 0.0f);

	// �ړ�������(true:�ړ�����)
	bool isPressMove = false;

	// �ړ�����
	if (!m_isAttack)
	{
		if (Pad::IsPress(PAD_INPUT_RIGHT))						// �E����
		{
			m_move = VAdd(m_move, VScale(leftMoveVec, -1.0f));
			isPressMove = true;
		}
		if (Pad::IsPress(PAD_INPUT_LEFT))						// ������
		{
			m_move = VAdd(m_move, leftMoveVec);
			isPressMove = true;
		}
		if (Pad::IsPress(PAD_INPUT_UP))							// �O����
		{
			m_move = VAdd(m_move, upMoveVec);
			isPressMove = true;
		}
		if (Pad::IsPress(PAD_INPUT_DOWN))						// ������
		{
			m_move = VAdd(m_move, VScale(upMoveVec, -1.0f));
			isPressMove = true;
		}

		// ���K��
		if (VSize(m_move) > 0.0f)
		{
			m_move = VNorm(m_move);
			m_targetDir = m_move;
			m_move = VScale(m_move, kSpeed);
		}
	}

	//�W�����v����
	Jump();
}

/// <summary>
/// �v���C���[�̈ړ�����
/// </summary>
/// <param name="MoveVector">�ړ���</param>
void Player::Move(const VECTOR& MoveVector, const EnemyRight& enemyR, const EnemyLeft& enemyL)
{
	if (fabs(MoveVector.x) > 0.0f || fabs(MoveVector.z) > 0.0f){
		m_isWalk = true;
	}
	else{
		m_isWalk = false;
	}

	Collision enemyRightCol = enemyR.GetColSphere();
	Collision enemyLeftCol = enemyL.GetColSphere();

	if (m_colSphere.IsBodyCollision(enemyRightCol))
	{
		VECTOR colNormal = VNorm(VSub(m_pos, enemyR.GetPos()));
		m_pos = VAdd(m_pos, VScale(colNormal, 0.7f));
		m_isBodyCol = true;
	}
	if (m_colSphere.IsBodyCollision(enemyLeftCol))
	{
		VECTOR colNormal = VNorm(VSub(m_pos, enemyL.GetPos()));
		m_pos = VAdd(m_pos, VScale(colNormal, 0.7f));
		m_isBodyCol = true;
	}

	// �v���C���[�̈ʒu�Ɉړ��ʂ𑫂�
	m_pos = VAdd(m_pos, m_move);

	


	// �v���C���[����ʊO�ɏo�Ȃ��悤���鏈��
	if (m_pos.x < mp.leftBack.x)
	{
		m_pos.x -= m_move.x;		// ��
	}
	if (m_pos.x > mp.rightFront.x)
	{
		m_pos.x -= m_move.x;		// �E
	}
	if (m_pos.z < mp.rightFront.z)
	{
		m_pos.z -= m_move.z;		// �O
	}
	if (m_pos.z > mp.leftBack.z)
	{
		m_pos.z -= m_move.z;		// ��
	}

	// �v���C���[�̈ʒu�Z�b�g
	MV1SetPosition(m_pModel->GetModel(), m_pos);
}

/// <summary>
/// �v���C���[�̊p�x����
/// </summary>
void Player::Angle()
{
	// �v���C���[�̈ړ������Ƀ��f���̕������߂Â���
	float targetAngle;		// �ڕW�p�x
	float difference;		// �ڕW�p�x�ƌ��݂̊p�x�̍�

	// �ڕW�̕����x�N�g������p�x�l���Z�o����
	targetAngle = static_cast<float>(atan2(m_targetDir.x, m_targetDir.z));

	// �ڕW�̊p�x�ƌ��݂̊p�x�Ƃ̍�������o��
	difference = targetAngle - m_angle;

	// ���̊p�x��180�x�ȏ�ɂȂ��Ă�����C������
	if (difference < -DX_PI_F)
	{
		difference += DX_TWO_PI_F;
	}
	else if (difference > DX_PI_F)
	{
		difference -= DX_TWO_PI_F;
	}

	// �p�x�̍���0�ɋ߂Â���
	if (difference > 0.0f)
	{
		// �����v���X�̏ꍇ�͈���
		difference -= kAngleSpeed;
		if (difference < 0.0f)
		{
			difference = 0.0f;
		}
	}
	else
	{
		// �����}�C�i�X�̏ꍇ�͑���
		difference += kAngleSpeed;
		if (difference > 0.0f)
		{
			difference = 0.0f;
		}
	}

	// ���f���̊p�x���X�V
	m_angle = targetAngle - difference;
	MV1SetRotationXYZ(m_pModel->GetModel(), VGet(0.0f, m_angle + DX_PI_F, 0.0f));
}


void Player::Attack(const EnemyRight& enemyR, const EnemyLeft& enemyL)
{
	// ���U��
	Collision enemyRightCol = enemyR.GetColSphere();
	Collision enemyLeftCol = enemyL.GetColSphere();

	m_addDamage = 0;
<<<<<<< HEAD
	m_isSordAttackToLeftEnemy = false;
	m_isSordAttackToRightEnemy = false;
=======
	m_isAttackLeft = false;
	m_isAttackRight = false;
>>>>>>> 60071db8ef0a37b71e4c4659020bfdc349240f1e
	
	if (m_colSphere.IsAttackCollision(enemyLeftCol))
	{
		m_isCol = true;
		m_isSordAttackToLeftEnemy = true;
	}
	if (m_colSphere.IsAttackCollision(enemyRightCol))
	{
		m_isCol = true;
		m_isSordAttackToRightEnemy = true;
	}

<<<<<<< HEAD
	if (m_isAttack && m_isCol) {
=======
	if (m_isAttack && isCol) {
>>>>>>> 60071db8ef0a37b71e4c4659020bfdc349240f1e
		if (m_pModel->GetAnimSpeed() >= 5.0f && m_pModel->GetAnimSpeed() < 6.0f)
		{
			m_pSound->PlaySE(SoundManager::SE_Type::kSordSE, DX_PLAYTYPE_BACK);
			m_isSordAttackDamage = true;
<<<<<<< HEAD
			m_pEffect->PlayDamageEffect(VGet(m_pos.x , m_pos.y + kEffectHeight, m_pos.z));
		}
	}

	if (m_isCol && m_isSordAttackDamage)
=======
			m_pEffect->PlayDamageEffect(VGet(m_pos.x, m_pos.y + kEffectHeight, m_pos.z));
		}
	}

	if (isCol && m_isSordAttackDamage)
>>>>>>> 60071db8ef0a37b71e4c4659020bfdc349240f1e
	{
		m_addDamage = kSordDamage;
		m_isSordAttackDamage = false;
	}	

	if (Pad::IsTrigger(PAD_INPUT_A)) {
		m_isNextAttackFlag = false;
	}

	// RB����������G�̕����������v���O��������
	if (Pad::IsPress(PAD_INPUT_Z))
	{
<<<<<<< HEAD
		if (m_isBowAttackToLeftEnemy) {
			m_vecToEnemy = VSub(m_pos, enemyL.GetPos());
			m_angle = atan2(-m_vecToEnemy.x, -m_vecToEnemy.z);
		}
		else{
=======
		if (m_isLeftEnemyAttack) {
			m_vecToEnemy = VSub(m_pos, enemyL.GetPos());
			m_angle = atan2(-m_vecToEnemy.x, -m_vecToEnemy.z);
		}
		else {
>>>>>>> 60071db8ef0a37b71e4c4659020bfdc349240f1e
			m_vecToEnemy = VSub(m_pos, enemyR.GetPos());
			m_angle = atan2(-m_vecToEnemy.x, -m_vecToEnemy.z);
		}
		MV1SetRotationXYZ(m_pModel->GetModel(), VGet(0.0f, m_angle + DX_PI_F, 0.0f));

		// �|�U��
		if (m_isBowAttackDamage)
		{
<<<<<<< HEAD
			if (!m_isBowAttackToLeftEnemy)	// �E�̓G�Ƀ_���[�W
			{
				m_pEffect->PlayDamageEffect(VGet(enemyR.GetPos().x + enemyR.GetMovePos().x*10, enemyR.GetPos().y + 10, enemyR.GetPos().z + enemyR.GetMovePos().z * 100));
				m_addDamage = kBowDamage;
				m_isBowAttackDamage = false;
				m_isSordAttackToRightEnemy = true;
			}
			else{
				m_pEffect->PlayDamageEffect(VGet(enemyL.GetPos().x + enemyL.GetMovePos().x * 100, enemyL.GetPos().y + 10, enemyL.GetPos().z + enemyL.GetMovePos().z * 100));
				m_addDamage = kBowDamage;
				m_isBowAttackDamage = false;
				m_isSordAttackToLeftEnemy = true;
=======
			if (!m_isLeftEnemyAttack)	// �E�̓G�Ƀ_���[�W
			{
				m_pEffect->PlayDamageEffect(VGet(enemyR.GetPos().x,enemyR.GetPos().y+10, enemyR.GetPos().z));
				m_addDamage = kBowDamage;
				m_isBowAttackDamage = false;
				m_isAttackRight = true;
			}
			else {
				m_pEffect->PlayDamageEffect(VGet(enemyL.GetPos().x, enemyL.GetPos().y + 10, enemyL.GetPos().z));
				m_addDamage = kBowDamage;
				m_isBowAttackDamage = false;
				m_isAttackLeft = true;
>>>>>>> 60071db8ef0a37b71e4c4659020bfdc349240f1e
			}
		}
	}
}

void Player::Jump()
{
	if (!m_isJump)	return;

	if (m_pos.y < 0.0f) {
		m_pState->EndState();
		m_isJump = false;
		m_pos.y = 0.0f;
		m_gravity = kGravity;
	}
	else if (m_pos.y >= 0.0f)
	{
		// �W�����v��ԂȂ�d�͓K�p
		if (m_isJump)
		{
			// �x�������̑��x���d�͕����Z����
			m_jumpPower -= m_gravity;
			m_gravity += 0.0002f;
		}
		// �ړ��x�N�g���̂x�������x�������̑��x�ɂ���
		m_move.y = m_jumpPower;
	}
}

void Player::Death()
{
	if (m_hp <= 0)
	{
		m_isDeath = true;
	}
}

void Player::ColUpdate()
{
	// �v���C���[�����蔻��p�J�v�Z���^�̍��W�X�V
	m_UpPos = VAdd(m_pos, kUpPos);

	MATRIX rotationMatrix = MGetRotY(m_angle);
	m_attackRange = VAdd(m_pos, VTransform(kAttackRange, rotationMatrix));

	m_colSphere.UpdateCol(m_pos, m_UpPos, m_attackRange,
		kColRadius, kAttackColRadius);
}
