#include "SoundManager.h"
#include <cassert>

namespace {
    constexpr char sound_file_signature[] = "SND_";
    constexpr char sound_config_file_path[] = "./sound.conf";
    constexpr float sound_config_version = 1.0f;
}

//�R���X�g���N�^
SoundManager::SoundManager() 
{
    //3D�T�E���h��XAudio���g�p���邩
    SetEnableXAudioFlag(true);

    //3D�T�E���h��1���[�g�����ǂ̂��炢�̒l�ɂ��邩
    Set3DSoundOneMetre(1.0f);
}

//�f�X�g���N�^
SoundManager::~SoundManager()
{
    for (const auto& sound : nameAndHandleTable_) {
        DeleteSoundMem(sound.second);
    }

    SaveSoundConfig();
}

//�����t�@�C�������[�h����
void SoundManager::LoadSound()
{
    LoadSoundConfig();
    Load2DSoundSEFile("pull");
    Load2DSoundSEFile("alarm");
    Load2DSoundSEFile("stopAlarm");
    Load2DSoundSEFile("checkSoundSE");
    Load2DSoundBGMFile("checkSoundBGM");

    Load3DSoundSEFile("asphaltStep");
    Load3DSoundSEFile("ironStep");

    Load3DSoundSEFile("hit");
    Load3DSoundSEFile("door");
    Load3DSoundSEFile("crank");
    Load3DSoundSEFile("switchOn");
    Load3DSoundSEFile("pullLever");
    Load3DSoundSEFile("stopCrank");
}

//�ǂݍ��񂾃T�E���h�𗬂�
void SoundManager::PlaySE(const std::string& name)
{
    assert(nameAndHandleTable_[name] != -1);

    //�������ɓǂݍ��񂾃T�E���h���Đ�����
    PlaySoundMem(nameAndHandleTable_[name], DX_PLAYTYPE_BACK);
}

//�ǂݍ���BGM�𗬂�
void SoundManager::PlayBGM(const std::string& name)
{
    //�t�@�C���p�X�̐���
    std::string path = "data/sound/BGM/";
    path = path + name;
    path = path + ".mp3";

    PlayMusic(path.c_str(), DX_PLAYTYPE_BACK);
}

//BGM�̉��ʂ𒲐�����
void SoundManager::SetSEVolume(unsigned int volume)
{
    for (auto& record : nameAndHandleTable_) {
        ChangeVolumeSoundMem(volume, record.second);
    }
    volumeSE_ = volume;
}

//BGM�̉��ʂ��擾����
void SoundManager::SetBGMVolume(unsigned int volume)
{
    SetVolumeMusic(volume);
    volumeBGM_ = volume;
}

void SoundManager::SetBGMRate(const float rate)
{
    assert(0.0f <= rate && rate <= 1.0f);
    SetVolumeMusic(static_cast<int>(static_cast<float>(volumeBGM_ * rate)));
}

//SE���~�߂�
void SoundManager::StopSE(const std::string& name)
{
    assert(static_cast<int>(name.size()) > 0);

    //�Đ����̃T�E���h���~�߂�
    StopSoundMem(nameAndHandleTable_[name]);
}

//BGM���~�߂�
void SoundManager::StopBGM()
{
    StopMusic();
}

//BGM�ASE�̉��ʂ��O���o�͂���
void SoundManager::SaveSoundConfig()
{
    SoundConfigInfo conf = {};
    std::copy_n(std::begin(sound_file_signature), sizeof(char) * 4, std::begin(conf.signature));
    conf.version = sound_config_version;
    conf.volumeSE = volumeSE_;
    conf.volumeBGM = volumeBGM_;

    FILE* fp = nullptr;
    fopen_s(&fp, sound_config_file_path, "wb");
    if (fp) {
        fwrite(&conf, sizeof(conf), 1, fp);
        fclose(fp);
    }
}

//�T�E���h�̃|�W�V�����A��������͈͂�ݒ肷��
void SoundManager::Set3DSoundInfo(const VECTOR& pos,const float audioRange, const std::string& name)
{
    assert(nameAndHandleTable_[name] != -1);

    //�T�E���h�̃|�W�V���������߂�
    Set3DPositionSoundMem(pos, nameAndHandleTable_[name]);

    //�T�E���h����������͈͂�ݒ�
    Set3DRadiusSoundMem(audioRange, nameAndHandleTable_[name]);
}

//3D�T�E���h���Đ�����ۂ̃��X�i�[�̈ʒu�����߂�
void SoundManager::Set3DSoundListenerInfo(const VECTOR& pos,const VECTOR& rot)
{
    //���X�i�[�̈ʒu�ƌ�����ݒ�
    Set3DSoundListenerPosAndFrontPos_UpVecY(pos, rot);
}

//�T�E���h���Đ��������ׂ�
int SoundManager::CheckSoundFile(const std::string& name)
{
    int check = DxLib::CheckSoundMem(nameAndHandleTable_[name]);

    return check;
}

//2DSE�����[�h����
int SoundManager::Load2DSoundSEFile(const std::string& fileName)
{
    //�t�@�C���p�X�̐���
    std::string path = "data/sound/SE/";
    path += fileName;
    path += ".mp3";

    //��������2D�T�E���h�����[�h����
    int handle = LoadSoundMem(path.c_str());
    assert(handle >= 0);

    //�z��Ƀ��[�h�������̂�������
    nameAndHandleTable_[fileName] = handle;

    return handle;
}

//2DBGM�����[�h����
int SoundManager::Load2DSoundBGMFile(const std::string& fileName)
{
    //�t�@�C���p�X�̐���
    std::string path = "data/sound/BGM/";
    path += fileName;
    path += ".mp3";

    //��������2D�T�E���h�����[�h����
    int handle = LoadSoundMem(path.c_str());
    assert(handle >= 0);

    //�z��Ƀ��[�h�������̂�������
    nameAndHandleTable_[fileName] = handle;

    return handle;
}

//3DSE�����[�h����
int SoundManager::Load3DSoundSEFile(const std::string& fileName)
{
    //�t�@�C���p�X�̐���
    std::string path = "data/sound/SE/";
    path += fileName;
    path += ".mp3";

    //��������3D�T�E���h�����[�h����
    SetCreate3DSoundFlag(true);
    int handle = LoadSoundMem(path.c_str());
    SetCreate3DSoundFlag(false);
    assert(handle >= 0);

    //�z��Ƀ��[�h�������̂�������
    nameAndHandleTable_[fileName] = handle;

    return handle;
}

//3DBGM�����[�h����
int SoundManager::Load3DSoundBGMFile(const std::string& fileName)
{
    //�t�@�C���p�X�̐���
    std::string path = "data/sound/BGM/";
    path += fileName;
    path += ".mp3";

    //��������3D�T�E���h�����[�h����
    SetCreate3DSoundFlag(true);
    int handle = LoadSoundMem(path.c_str());
    SetCreate3DSoundFlag(false);
    assert(handle >= 0);

    //�z��Ƀ��[�h�������̂�������
    nameAndHandleTable_[fileName] = handle;

    return handle;
}

//�T�E���h���̓ǂݍ���
void SoundManager::LoadSoundConfig()
{
    SoundConfigInfo conf = {};
    FILE* fp = nullptr;
    fopen_s(&fp, sound_config_file_path, "rb");
    if (fp) {
        fread(&conf, sizeof(conf), 1, fp);
        fclose(fp);
        volumeBGM_ = conf.volumeBGM;
        volumeSE_ = conf.volumeSE;
    }
}