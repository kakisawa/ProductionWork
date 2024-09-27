#pragma once
#include "../object/ObjectData.h"
#include <string>
#include <DxLib.h>

class Model
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="fileName">���f���������Ă���t�@�C���p�X</param>
	Model(const std::string& fileName,const Material materialType);

	/// <summary>
	/// duplicate�p�R���X�g���N�^
	/// </summary>
	/// <param name="orgModel"></param>
	Model(const int orgModel,const Material materialType);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Model();

	/// <summary>
	/// collision�t���[����collision�Ƃ��Ďg��
	/// </summary>
	/// <param name="isUse">�g�p���邩</param>
	/// <param name="isNeedUpdate">�X�V���K�v��</param>
	/// <param name="collFrameName">����̃t���[�����Փ˔���p�t���[���ɂ���ꍇ�A�t���[�����������</param>
	void SetUseCollision(const bool isUse, const  bool isNeedUpdate, const std::string& collFrameName = "");

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �A�j���[�V������ύX����
	/// </summary>
	/// <param name="animNo">�A�j���[�V�����ԍ�</param>
	/// <param name="isLoop">���[�v�Đ����邩</param>
	/// <param name="isForceChange">�����I�ɕύX���邩</param>
	/// <param name="changeFrame">�A�j���[�V������ύX����ۂɉ��b�����ĕω�����̂�</param>
	void ChangeAnimation(const int animNo, const  bool isLoop, const  bool isForceChange, const int changeFrame);

	/// <summary>
	/// �A�j���[�V�����̏I�����擾����
	/// </summary>
	/// <returns>�A�j���[�V�������I�������</returns>
	bool IsAnimEnd();

	////////////////Getter////////////////

	/// <summary>
	/// �w�肵���t���[���̍��W���擾����
	/// </summary>
	/// <param name="animNo">�A�j���[�V�����ԍ�</param>
	/// <param name="frameName">�擾�������A�j���[�V������</param>
	/// <returns>���W</returns>
	VECTOR GetFrameLocalPosition(const std::string& frameName)const;

	/// <summary>
	/// �A�j���[�V�����̓���t���[���ƌ��݂̃A�j���[�V�����t���[���������������擾����
	/// </summary>
	/// <param name="specifiedTime">����̃t���[������</param>
	/// <returns>true : ����  false : �Ⴄ</returns>
	bool GetSpecifiedAnimTime(const int specifiedTime)const;

	/// <summary>
	/// ���f�����擾����
	/// </summary>
	/// <returns>���f���n���h����Ԃ�</returns>
	int GetModelHandle() const { return modelHandle_; }

	/// <summary>
	/// collision�t���[�����擾����
	/// </summary>
	/// <returns>collision�t���[����Ԃ�</returns>
	int GetColFrameIndex() const { return colFrameIndex_; }

	/// <summary>
	/// �|�W�V�������擾����
	/// </summary>
	/// <returns>�|�W�V������Ԃ�</returns>
	VECTOR GetPos() const { return pos_; }

	/// <summary>
	/// ��]�l���擾����
	/// </summary>
	/// <returns>��]�l</returns>
	VECTOR GetRot()const { return rot_; }

	/// <summary>
	/// ���݂̃A�j���[�V�����ԍ����擾����
	/// </summary>
	/// <returns>�A�j���[�V�����ԍ�</returns>
	int GetCurrentAnimNo() const { return animNext_.animNo; }

	/// <summary>
	/// �A�j���[�V�����̍Đ����Ԃ��擾����
	/// </summary>
	/// <returns>��A�j���[�V�����̍Đ�����</returns>
	float GetAnimTotalTime()const { return animNext_.totalTime; }

	/// <summary>
	/// �I�u�W�F�N�g�̃}�e���A���^�C�v���擾
	/// </summary>
	/// <returns>�^�C�v</returns>
	Material GetMaterialType() const { return materialType_; }

	////////////////Setter////////////////

	/// <summary>
	/// �|�W�V������ݒ肷��
	/// </summary>
	/// <param name="pos">�|�W�V����</param>
	void SetPos(const VECTOR& pos);

	/// <summary>
	/// �p�x��ݒ肷��
	/// </summary>
	/// <param name="rot">�p�x</param>
	void SetRot(const VECTOR& rot);

	/// <summary>
	/// �T�C�Y��ݒ肷��
	/// </summary>
	/// <param name="scale">�g�k��</param>
	void SetScale(const VECTOR& scale);

	/// <summary>
	/// collision�Ƃ��Ďg�������t���[�����w�肷��
	/// </summary>
	/// <param name="collFrameName">collision�Ƃ��Ďg�������t���[�����B�Ȃ���΋�</param>
	void SetCollFrame(const std::string& collFrameName = "");

	/// <summary>
	/// �A�j���[�V������ݒ肷��
	/// </summary>
	/// <param name="animNo">�A�j���[�V�����ԍ�</param>
	/// <param name="isLoop">���[�v�Đ����邩</param>
	/// <param name="IsForceChange">�����I�ɕύX���邩</param>
	void SetAnimation(const int animNo, const bool isLoop, const bool IsForceChange);

	/// <summary>
	/// �A�j���[�V�����ԍ��̍Ō�̃t���[����ݒ肷��
	/// </summary>
	/// <param name="animNo">�A�j���[�V�����ԍ�</param>
	void SetAnimEndFrame(const int animNo);

	/// <summary>
	/// �A�j���[�V�����̃t���[����ݒ肷��
	/// </summary>
	/// <param name="value">�t���[����</param>
	void SetAnimationFrame(const float value);

private:
	//�v�@14byte
	struct AnimData {
		int animNo;			//4byte
		int attachNo;		//4byte
		float totalTime;	//4byte
		bool isLoop;		//2byte
	};
private:
	/// <summary>
	/// �A�j���[�V�����f�[�^������������
	/// </summary>
	/// <param name="anim"></param>
	void ClearAnimData(AnimData& anim);

	/// <summary>
	/// �A�j���[�V�����̍X�V
	/// </summary>
	/// <param name="anim"></param>
	/// <param name="dt"></param>
	void UpdateAnim(const AnimData& anim, const  float dt = 1.0f);

	/// <summary>
	/// �A�j���[�V�����̃u�����h
	/// </summary>
	void UpdateAnimBlendRate();
private:
	int modelHandle_ = -1;				//���f���n���h��
	int colFrameIndex_ = 0;				//�Փ˔���p�t���[��
	int animChangeFrame_ = 0;			//�A�j���[�V�����̕ύX�t���[��
	int animChangeFrameTotal_ = 0;		//�A�j���[�V������ύX����̂ɂ����鑍����
										
	bool isUseCollision_ = false;		//�Փ˔�����s����
	bool isUpdateCollsion_ = false;		//�Փ˔���p�t���[���̍X�V���s����
										
	VECTOR pos_ = {};					//�|�W�V����
	VECTOR rot_ = {};					//��]

	AnimData animPrev_ = {};			//�O�̃A�j���[�V�������
	AnimData animNext_ = {};			//���̃A�j���[�V�������
										
	Material materialType_ = {};		//�}�e���A���̑f��
};

