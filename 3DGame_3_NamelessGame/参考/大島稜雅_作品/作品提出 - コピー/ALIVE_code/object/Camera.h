#pragma once
#include <DxLib.h>

class Camera
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">�������W</param>
	/// <param name="viewPos">����ꏊ</param>
	Camera(const VECTOR& pos, const VECTOR& viewPos);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Camera();

	/// <summary>
	/// �J�����̏�����
	/// </summary>
	/// <param name="targetPos">�J����������ʒu</param>
	void Init(const VECTOR& targetPos);

	/// <summary>
	/// �J�����̍X�V
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	/// <param name="playerHeight">�v���C���[�̐g��</param>
	void Update(const VECTOR& playerPos, const float playerHeight);

	/// <summary>
	/// �v���C���[��ǐՂ���J�����̍X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	void TrackingCameraUpdate(const VECTOR& playerPos, const float playerHeight);

	/// <summary>
	/// �J�����̒����_����炷
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	/// <param name="playerHeight">�v���C���[�̐g��</param>
	void ChangeOfFocus(const VECTOR& playerPos, const float playerHeight);

	/// <summary>
	/// �ڕW�̃|�W�V�����܂ňړ�����
	/// </summary>
	void EasingMoveCamera();

	////////////////Getter////////////////

	/// <summary>
	/// �J�����̃|�W�V�������擾����
	/// </summary>
	/// <returns>�J�����̍��W</returns>
	const VECTOR& GetPos() const { return pos_; }

	/// <summary>
	/// �J����������ʒu���擾����
	/// </summary>
	/// <returns>�J����������ʒu</returns>
	const VECTOR& GetTarget() const { return cameraViewingPos_; }

	/// <summary>
	/// �J�������ړ������擾����
	/// </summary>
	/// <returns>true : �ړ����@ false : �ړ����ĂȂ�</returns>
	bool GetMoving() const { return isMoving_; }


	////////////////Setter////////////////

	/// <summary>
	/// �J�������ڕW�Ƃ�����W�ƌ���ʒu��ݒ肷��
	/// </summary>
	/// <param name="targetPos">�ڕW�ʒu</param>
	/// <param name="targetViewPos">����ʒu</param>
	/// <param name="upVec">�J�����̏�����x�N�g��</param>
	void SetCameraTargetPosAndView(const VECTOR& targetPos, const VECTOR& targetViewPos, const VECTOR& upVec);

	/// <summary>
	/// �o�ߎ��Ԃ�0�ɖ߂�
	/// </summary>
	void ResetElapsedTime() { elapsedTime_ = 0.0f; }
private:

	/// <summary>
	/// �v���C���[��X���W��ǐ�
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	float TrackingPosX(const VECTOR& playerPos);

	/// <summary>
	/// �v���C���[��Y���W��ǐ�
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	float TrackingPosY(const VECTOR& playerPos, const float playerHeight);

private:

	float moveVecX_ = 0.0f;						//�ړ��x�N�g����X
	float moveVecY_ = 0.0f;						//�ړ��x�N�g����Y

	float elapsedTime_ = 0.0f;					//�o�ߎ���

	bool isMoving_ = false;						//�ړ�����

	DINPUT_JOYSTATE input_ = {};				//�E�X�e�B�b�N�̌��ʂ������

	VECTOR pos_ = {};							//�J�����̃|�W�V����
	VECTOR upVec_ = {0,1,0};					//�J�����̌���ڕW�ʒu
	VECTOR cameraViewingPos_ = {};				//�J����������ʒu

	VECTOR cameraTargetPos_ = {};				//�J�����̖ڕW�ʒu
	VECTOR targetUpVec_ = {};					//�J�����̖ڕW�����
	VECTOR cameraTargetViewPos_ = {};			//�J�����̌���ڕW�ʒu
};

