#include "Player.h"
#include "../Camera.h"
#include "../Pad.h"
#include "../LoadCsv.h"
#include <cmath>

namespace {
	const VECTOR InitVec = VGet(0.0f, 0.0f, 0.0f);	// Vector�l�������l
	constexpr float InitFloat = 0.0f;				// float�l������

	const char* kModelFilePath = "Data/Model/PlayerModel.mv1";
}

Player::Player() :
	model(-1),
	m_angle(InitFloat),
	m_pos(InitVec),
	m_move(InitVec),
	m_targetDir(InitVec)
{
	// �v���C���[�O���f�[�^�ǂݍ���
	LoadCsv::GetInstance().LoadData(m_chara);
	// ���f���̓ǂݍ���
	model = MV1LoadModel(kModelFilePath);

	// ���W�����l
	m_pos = VGet(m_chara.initPosX, m_chara.initPosY, m_chara.initPosZ);
}

Player::~Player()
{
	MV1DeleteModel(model);
}

void Player::Init()
{
	MV1SetPosition(model, m_pos);
	MV1SetScale(model, VGet(m_chara.modelSize, m_chara.modelSize, m_chara.modelSize));
}

void Player::Update(const Camera& camera)
{
	Move(camera);
	Angle();

	m_pos = VAdd(m_pos, m_move);
	MV1SetPosition(model, m_pos);
}

void Player::Draw()
{
	MV1DrawModel(model);

	DrawFormatString(0, 300, 0xffffff, "m_move.x/y/z=%.2f/%.2f/%.2f", m_move.x, m_move.y, m_move.z);

	DrawFormatString(0, 340, 0xffffff, "m_targetDir=%.2f", m_targetDir);
	DrawFormatString(0, 360, 0xffffff, "inputX=%d", inputX);
	DrawFormatString(0, 380, 0xffffff, "inputY=%d", inputY);
}

void Player::Move(const Camera& camera)
{
	//{
	//	VECTOR upMoveVec = VSub(camera.GetTarget(), camera.GetPosition());
	//	upMoveVec.y = InitFloat;
	//	VECTOR leftMoveVec = VCross(upMoveVec, VGet(0.0f, m_chara.walkSpeed, 0.0f));
	//	// �ړ��ʂ̏�����
	//	m_move = InitVec;
	//	inputX, inputY = 0;
	//	GetJoypadAnalogInput(&inputX, &inputY, DX_INPUT_KEY_PAD1);
	//	m_move = VGet(inputX, 0.0f, -inputY);
	//	if (inputX > 0.0f)					// �E����
	//	{
	//		m_move = VAdd(m_move, VScale(leftMoveVec, -1.0f));
	//	}
	//	if (inputX < 0.0f)						// ������
	//	{
	//		m_move = VAdd(m_move, leftMoveVec);
	//	}
	//	if (inputY < 0.0f)							// �O����
	//	{
	//		m_move = VAdd(m_move, upMoveVec);
	//	}
	//	if (inputY > 0.0f)						// ������
	//	{
	//		m_move = VAdd(m_move, VScale(upMoveVec, -1.0f));
	//	}
	//	float len = VSize(m_move);
	//	// �x�N�g���̒�����0.0�`1.0�̊����ɕϊ�����
	//	float rate = len / 1000.0f;
	//	// ���K��
	//	if (VSize(m_move) > 0.0f) {
	//		m_move = VNorm(m_move);
	//		m_targetDir = m_move;
	//		m_move = VScale(m_move, m_chara.walkSpeed);
	//	}
	//}

	//if (Pad::IsPress(PAD_INPUT_RIGHT))						// �E����
	//{
	//	m_move = VAdd(m_move, VScale(leftMoveVec, -1.0f));
	//}
	//if (Pad::IsPress(PAD_INPUT_LEFT))						// ������
	//{
	//	m_move = VAdd(m_move, leftMoveVec);
	//}
	//if (Pad::IsPress(PAD_INPUT_UP))							// �O����
	//{
	//	m_move = VAdd(m_move, upMoveVec);
	//}
	//if (Pad::IsPress(PAD_INPUT_DOWN))						// ������
	//{
	//	m_move = VAdd(m_move, VScale(upMoveVec, -1.0f));
	//}
	//// ���K��
	//if (VSize(m_move) > 0.0f) {
	//	m_move = VNorm(m_move);
	//	m_targetDir = m_move;
	//	m_move = VScale(m_move, m_chara.walkSpeed);
	//}


	// �J�����̌����x�N�g�����擾
	VECTOR cameraForwardVec = VSub(camera.GetTarget(), camera.GetPosition());
	cameraForwardVec.y = 0.0f; // ���������̂ݍl������
	cameraForwardVec = VNorm(cameraForwardVec); // ���K��

	// �J�����̉E�����x�N�g�����擾�iforward�x�N�g����Y����x�N�g���̊O�ςŎZ�o�j
	VECTOR cameraRightVec = VCross(cameraForwardVec, VGet(0.0f, 1.0f, 0.0f));

	// ���͂̏�����
	m_move = InitVec;
	inputX = inputY = 0;

	// ���͎擾
	GetJoypadAnalogInput(&inputX, &inputY, DX_INPUT_KEY_PAD1);

	// �J������Ńv���C���[�̈ړ��x�N�g����ݒ�
	m_move = VScale(cameraForwardVec, -inputY);  // �O��ړ�
	m_move = VAdd(m_move, VScale(cameraRightVec, -inputX));  // ���E�ړ�

	// ���K���ƈړ����x�̓K�p
	if (VSize(m_move) > 0.0f) {
		m_move = VNorm(m_move); // ���K��
		m_targetDir = m_move;  // �ڕW������ۑ�
		m_move = VScale(m_move, m_chara.walkSpeed); // �ړ����x��K�p
	}
}

void Player::leftStick(VECTOR moveUp, VECTOR moveLeft)
{


}

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
		difference -= m_chara.rotaSpeed;
		if (difference < 0.0f)
		{
			difference = 0.0f;
		}
	}
	else
	{
		// �����}�C�i�X�̏ꍇ�͑���
		difference += m_chara.rotaSpeed;
		if (difference > 0.0f)
		{
			difference = 0.0f;
		}
	}

	// ���f���̊p�x���X�V
	m_angle = targetAngle - difference;
	MV1SetRotationXYZ(model, VGet(0.0f, m_angle + DX_PI_F, 0.0f));
}
