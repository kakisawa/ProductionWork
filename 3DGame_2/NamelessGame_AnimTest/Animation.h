#pragma once
class Animation
{
public:
	Animation();
	~Animation();

	virtual void Init();
	virtual void Update(int model);
	virtual void Draw();

public:
	// �A�j���[�V�����̐���

	/// <summary>
	/// �A�j���[�V������ݒ肷��(�����ɐ؂�ւ���)
	/// </summary>
	/// <param name="animNo">�ύX��A�j���[�V�����ԍ�</param>
	/// <param name="isLoop">�A�j���[�V���������[�v�����邩</param>
	/// <param name="isForceChange">���Ɏw�肳�ꂽ�A�j���[�V�������Đ�����Ă���ꍇ���ύX���邩</param>
	void SetAnim(int model,int animNo, bool isLoop, bool isForceChange);


	/// <summary>
	/// �A�j���[�V�����ύX������
	/// </summary>
	/// <param name="animNo">�ύX��A�j���[�V�����ԍ�</param>
	/// <param name="isLoop">���[�v�Đ������邩</param>
	/// <param name="isForceChange">���Ɏw�肳�ꂽ�A�j���[�V�������Đ�����Ă���ꍇ���Đ����邩</param>
	/// <param name="isChangeFrame">���t���[�������Đ؂�ւ���</param>
	void ChangeAnim(int model,int animNo, bool isLoop, bool isForceChange, bool isChangeFrame);

	// ���݂̃A�j���[�V�������I�����Ă��邩�ǂ������擾����(Loop�A�j���̏ꍇ�͎擾�ł��Ȃ� = false��Ԃ�)
	bool IsAnimEnd(int model);

private:
	// �A�j���[�V�������
	struct AnimData			// �A�j���[�V�����f�[�^
	{
		int animNo;			// �A�j���[�V�����ԍ�
		int attachNo;		// �A�^�b�`�ԍ�
		float totalTime;	// �A�j���[�V�����̑��Đ�����
		float elapsedTime;	// �A�j���[�V�����̌o�ߎ���
		bool isLoop;		// �A�j���[�V���������[�v���邩
	};

protected:
	// �A�j���[�V������Ԃ̏�����
	void InitAnim(AnimData& anim);

	// �A�j���[�V�����̍X�V
	void UpdateAnimation(int model, AnimData anim, float dt = 1.0f);

	// ���݂̃A�j���[�V�����؂�ւ���񂩂�A�j���[�V�����̃u�����h����ݒ肷��
	void UpdateAnimBlendRate(int model);

private:
	// �A�j���[�V�����̐؂�ւ����
	int m_animChangeFrame;			// ���݂̐؂�ւ��t���[����
	int m_animChangeFrameTotal;		// �؂�ւ��ɂ����鑍�t���[����

	float m_animBlendRate;		// ���݂Ɖߋ��̃A�j���[�V�����̍�������	(�t���[��)
									// 0.0f:prev���Đ������
									// 1.0f:current���Đ������

	float m_animSpeed;		// �A�j���[�V�����ύX���x

	// �A�j���[�V�������
	AnimData m_current;		// �ύX��A�j���[�V�����f�[�^
	AnimData m_prev;		// �ύX�O�A�j���[�V�����f�[�^

};

