#pragma once
#include <DxLib.h>
#include <string>
#include <unordered_map>

class SoundManager
{
public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SoundManager();

	/// <summary>
	/// �C���X�^���X�쐬
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static SoundManager& GetInstance() {
		static SoundManager instance;
		return instance;
	}

	/// <summary>
	/// �����t�@�C�������[�h����
	/// </summary>
	void LoadSound();

	/// <summary>
	/// �ǂݍ��񂾃T�E���h�𗬂�
	/// </summary>
	/// <param name="name">�t�@�C����</param>
	void PlaySE(const std::string& name);

	/// <summary>
	/// �ǂݍ���BGM�𗬂�
	/// </summary>
	/// <param name="name">�t�@�C����</param>
	void PlayBGM(const std::string& name);

	/// <summary>
	/// SE���~�߂�
	/// </summary>
	/// <param name="name">���̃t�@�C����</param>
	void StopSE(const std::string& name = "");

	/// <summary>
	/// BGM���~�߂�
	/// </summary>
	void StopBGM();

	/// <summary>
	/// �T�E���h���Đ��������ׂ�
	/// </summary>
	/// <returns>1:�Đ��� 0:�I�� -1:�G���[</returns>
	int CheckSoundFile(const std::string& name);


	////////////////Getter////////////////

	/// <summary>
	/// SE�̉��ʂ��擾����
	/// </summary>
	/// <returns>����</returns>
	int GetSEVolume()const { return volumeSE_; };

	/// <summary>
	/// BGM�̉��ʂ��擾����
	/// </summary>
	/// <returns>����</returns>
	int GetBGMVolume()const { return volumeBGM_; }


	////////////////Setter////////////////

	/// <summary>
	/// SE���ʂ𒲐�����
	/// </summary>
	/// <param name="volume">����</param>
	void SetSEVolume(unsigned int volume);

	/// <summary>
	/// BGM�̉��ʂ𒲐�����
	/// </summary>
	/// <param name="volume">����</param>
	void SetBGMVolume(unsigned int volume);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="rate"></param>
	void SetBGMRate(const float rate);

	/// <summary>
	/// �T�E���h�̃|�W�V�����A��������͈͂�ݒ肷��
	/// </summary>
	/// <param name="pos">�T�E���h�̃|�W�V����</param>
	/// <param name="audioRange">��������͈�</param>
	/// <param name="name">�T�E���h��</param>
	void Set3DSoundInfo(const VECTOR& pos, const float audioRange, const std::string& name);

	/// <summary>
	/// 3D�T�E���h���Đ�����ۂ̃��X�i�[�̈ʒu�����߂�
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	void Set3DSoundListenerInfo(const VECTOR& pos, const VECTOR& rot);
private:
	/// <summary>
	/// BGM�ASE�̉��ʂ��O���o�͂���
	/// </summary>
	void SaveSoundConfig();

	/// <summary>
	/// �T�E���h���̓ǂݍ���
	/// </summary>
	void LoadSoundConfig();

	/// <summary>
	/// 2DSE�����[�h����
	/// </summary>
	/// <param name="fileName">�g���q�A�ꏊ�����̃t�@�C���P�̖̂��O</param>
	/// <returns>���[�h�����T�E���h</returns>
	int Load2DSoundSEFile(const std::string& fileName);

	/// <summary>
	/// 2DBGM�����[�h����
	/// </summary>
	/// <param name="fileName">�g���q�A�ꏊ�����̃t�@�C���P�̖̂��O</param>
	/// <returns>���[�h�����T�E���h</returns>
	int Load2DSoundBGMFile(const std::string& fileName);

	/// <summary>
	/// 3DSE�����[�h����
	/// </summary>
	/// <param name="fileName">�g���q�A�ꏊ�����̃t�@�C���P�̖̂��O</param>
	/// <returns>���[�h�����T�E���h</returns>
	int Load3DSoundSEFile(const std::string& fileName);

	/// <summary>
	/// 3DBGM�����[�h����
	/// </summary>
	/// <param name="fileName">�g���q�A�ꏊ�����̃t�@�C���P�̖̂��O</param>
	/// <returns>���[�h�����T�E���h</returns>
	int Load3DSoundBGMFile(const std::string& fileName);
private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SoundManager();

	SoundManager(const SoundManager&) = delete;
	void operator = (const SoundManager&) = delete;
private:
	//�v�@12byte
	struct SoundConfigInfo {
		char signature[4];				//4byte
		float version;					//4byte
		unsigned short volumeSE;		//2byte
		unsigned short volumeBGM;		//2byte
	};

	int volumeSE_ = 255;
	int volumeBGM_ = 255;

	std::unordered_map<std::string, int> nameAndHandleTable_;
};

