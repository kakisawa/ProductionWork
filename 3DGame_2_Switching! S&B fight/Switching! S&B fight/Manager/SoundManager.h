#pragma once
#include<map>
#include<string>

class SoundManager
{
public:

	//BGM�̎��
	enum class BGM_Type
	{
		kTitleBGM,		// �^�C�g��BGM
		kGameBGM,		// �Q�[��BGM
		kGameClearBGM,	// �Q�[���N���ABGM
		kGameOverBGM,	// �Q�[���I�[�o�[BGM
		kOptionBGM,		// �I�v�V����BGM
		MAX
	};

	//SE�̎��
	enum class SE_Type
	{
		kSelectSE,		// �Z���N�gSE
		kButtonSE,		// ����SE
		kBackSE,		// �L�����Z��SE
		MAX,
	};

	//�T�E���h�̏�����
	void InitSound(void);

	//BGM�̃p�X�̏�����
	void InitBGM(void);

	/// <summary>
	/// BGM�̃��[�h
	/// </summary>
	/// <param name="_bgm">BGM�̎��</param>
	void LoadBGM(BGM_Type bgm);

	//SE�̃p�X�̏�����
	void InitSE(void);

	/// <summary>
	/// SE�̃��[�h	
	/// </summary>
	/// <param name="_bgm">SE�̎��</param>
	void LoadSE(SE_Type se);

	/// <summary>
	/// BGM�̍Đ�
	/// </summary>
	/// <param name="_bgm">BGM�̎��</param>
	/// <param name="_playType">�Đ��`��</param>
	/// <param name="_volumePar">����(%�\�L)</param>
	/// <param name="_topPositionFlag">�ŏ�����Đ����n�߂邩 (true:�ŏ�����)</param>
	void PlayBGM(BGM_Type bgm, int playType, int volumePar = 100, bool topPositionFlag = true);

	/// <summary>
	/// SE�̍Đ�
	/// </summary>
	/// <param name="_se">SE�̎��</param>
	/// <param name="_playType">�Đ��`��</param>
	/// <param name="_volumePar">����(%�\�L)</param>
	/// <param name="_topPositionFlag">�ŏ�����Đ����n�߂邩 (true:�ŏ�����)</param>
	void PlaySE(SE_Type se, int playType, int volumePar = 100, bool topPositionFlag = true);

	/// <summary>
	/// BGM���Đ������ǂ�����Ԃ�
	/// </summary>
	/// <param name="_bgm">BGM�̎��</param>
	/// <returns>true:�Đ���</returns>
	bool CheckPlayBGM(BGM_Type bgm);

	/// <summary>
	/// SE���Đ������ǂ�����Ԃ�
	/// </summary>
	/// <param name="_se">SE�̎��</param>
	/// <returns>true:�Đ���</returns>
	bool CheckPlaySE(SE_Type se);

	/// <summary>
	/// BGM���~�߂�
	/// </summary>
	/// <param name="_bgm">�~�߂�BGM</param>
	void StopBGM(BGM_Type bgm);

	/// <summary>
	/// SE���~�߂�
	/// </summary>
	/// <param name="_se">�~�߂�SE</param>
	void StopSE(SE_Type se);

	//�T�E���h�̉��
	void ReleaseSound(void);

private:

	//�T�E���h�̃p�X
	std::map<BGM_Type, std::string> m_bgmPass;	//BGM�̃p�X

	std::map<SE_Type, std::string> m_sePass;		//SE�̃p�X

	//�T�E���h�̃n���h��
	std::map<BGM_Type, int> m_bgm;	//BGM�n���h��

	std::map<SE_Type, int> m_se;		//SE�n���h��
};