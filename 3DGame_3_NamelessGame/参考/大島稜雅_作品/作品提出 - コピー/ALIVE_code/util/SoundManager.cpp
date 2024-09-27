#include "SoundManager.h"
#include <cassert>

namespace {
    constexpr char sound_file_signature[] = "SND_";
    constexpr char sound_config_file_path[] = "./sound.conf";
    constexpr float sound_config_version = 1.0f;
}

//コンストラクタ
SoundManager::SoundManager() 
{
    //3DサウンドにXAudioを使用するか
    SetEnableXAudioFlag(true);

    //3Dサウンドの1メートルをどのくらいの値にするか
    Set3DSoundOneMetre(1.0f);
}

//デストラクタ
SoundManager::~SoundManager()
{
    for (const auto& sound : nameAndHandleTable_) {
        DeleteSoundMem(sound.second);
    }

    SaveSoundConfig();
}

//音声ファイルをロードする
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

//読み込んだサウンドを流す
void SoundManager::PlaySE(const std::string& name)
{
    assert(nameAndHandleTable_[name] != -1);

    //メモリに読み込んだサウンドを再生する
    PlaySoundMem(nameAndHandleTable_[name], DX_PLAYTYPE_BACK);
}

//読み込んだBGMを流す
void SoundManager::PlayBGM(const std::string& name)
{
    //ファイルパスの生成
    std::string path = "data/sound/BGM/";
    path = path + name;
    path = path + ".mp3";

    PlayMusic(path.c_str(), DX_PLAYTYPE_BACK);
}

//BGMの音量を調整する
void SoundManager::SetSEVolume(unsigned int volume)
{
    for (auto& record : nameAndHandleTable_) {
        ChangeVolumeSoundMem(volume, record.second);
    }
    volumeSE_ = volume;
}

//BGMの音量を取得する
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

//SEを止める
void SoundManager::StopSE(const std::string& name)
{
    assert(static_cast<int>(name.size()) > 0);

    //再生中のサウンドを止める
    StopSoundMem(nameAndHandleTable_[name]);
}

//BGMを止める
void SoundManager::StopBGM()
{
    StopMusic();
}

//BGM、SEの音量を外部出力する
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

//サウンドのポジション、聞こえる範囲を設定する
void SoundManager::Set3DSoundInfo(const VECTOR& pos,const float audioRange, const std::string& name)
{
    assert(nameAndHandleTable_[name] != -1);

    //サウンドのポジションを決める
    Set3DPositionSoundMem(pos, nameAndHandleTable_[name]);

    //サウンドが聞こえる範囲を設定
    Set3DRadiusSoundMem(audioRange, nameAndHandleTable_[name]);
}

//3Dサウンドを再生する際のリスナーの位置を決める
void SoundManager::Set3DSoundListenerInfo(const VECTOR& pos,const VECTOR& rot)
{
    //リスナーの位置と向きを設定
    Set3DSoundListenerPosAndFrontPos_UpVecY(pos, rot);
}

//サウンドが再生中か調べる
int SoundManager::CheckSoundFile(const std::string& name)
{
    int check = DxLib::CheckSoundMem(nameAndHandleTable_[name]);

    return check;
}

//2DSEをロードする
int SoundManager::Load2DSoundSEFile(const std::string& fileName)
{
    //ファイルパスの生成
    std::string path = "data/sound/SE/";
    path += fileName;
    path += ".mp3";

    //メモリに2Dサウンドをロードする
    int handle = LoadSoundMem(path.c_str());
    assert(handle >= 0);

    //配列にロードしたものを加える
    nameAndHandleTable_[fileName] = handle;

    return handle;
}

//2DBGMをロードする
int SoundManager::Load2DSoundBGMFile(const std::string& fileName)
{
    //ファイルパスの生成
    std::string path = "data/sound/BGM/";
    path += fileName;
    path += ".mp3";

    //メモリに2Dサウンドをロードする
    int handle = LoadSoundMem(path.c_str());
    assert(handle >= 0);

    //配列にロードしたものを加える
    nameAndHandleTable_[fileName] = handle;

    return handle;
}

//3DSEをロードする
int SoundManager::Load3DSoundSEFile(const std::string& fileName)
{
    //ファイルパスの生成
    std::string path = "data/sound/SE/";
    path += fileName;
    path += ".mp3";

    //メモリに3Dサウンドをロードする
    SetCreate3DSoundFlag(true);
    int handle = LoadSoundMem(path.c_str());
    SetCreate3DSoundFlag(false);
    assert(handle >= 0);

    //配列にロードしたものを加える
    nameAndHandleTable_[fileName] = handle;

    return handle;
}

//3DBGMをロードする
int SoundManager::Load3DSoundBGMFile(const std::string& fileName)
{
    //ファイルパスの生成
    std::string path = "data/sound/BGM/";
    path += fileName;
    path += ".mp3";

    //メモリに3Dサウンドをロードする
    SetCreate3DSoundFlag(true);
    int handle = LoadSoundMem(path.c_str());
    SetCreate3DSoundFlag(false);
    assert(handle >= 0);

    //配列にロードしたものを加える
    nameAndHandleTable_[fileName] = handle;

    return handle;
}

//サウンド情報の読み込み
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