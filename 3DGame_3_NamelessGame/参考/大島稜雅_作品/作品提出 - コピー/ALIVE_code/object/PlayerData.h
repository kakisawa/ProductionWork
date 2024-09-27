#pragma once

enum class PlayerAnimType {
	Idle,			//�ҋ@���
	Walk,			//����
	Run,			//����
	Jump,			//�W�����v
	Crank,			//�N�����N����
	Death,			//����
	IdleToSitup,	//idle��Ԃ������
	SitupToIdle,	//�����Ă����Ԃ���idle
	LeverOn,		//���o�[���N������
	WakeUp,			//�N���オ��
	Throw,			//������
	StopTimer,		//�^�C�}�[���Ƃ߂�
	StopTimerCancel,//�^�C�}�[���~�߂�s�����Ƃ߂�
	Put,			//����u��
	Pull,			//��������
	max
};

//�v�@20byte
struct PlayerInfo {
	float jumpPower;			//4byte
	float runningJumpPower;		//4byte
	float rotSpeed;				//4byte
	float walkSpeed;			//4byte
	float runningSpeed;			//4byte
};