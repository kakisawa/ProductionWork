#pragma once
#include <DxLib.h>
#include <string>
#include <unordered_map>

class SoundManager
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SoundManager();

	/// <summary>
	/// インスタンス作成
	/// </summary>
	/// <returns>インスタンス</returns>
	static SoundManager& GetInstance() {
		static SoundManager instance;
		return instance;
	}

	/// <summary>
	/// 音声ファイルをロードする
	/// </summary>
	void LoadSound();

	/// <summary>
	/// 読み込んだサウンドを流す
	/// </summary>
	/// <param name="name">ファイル名</param>
	void PlaySE(const std::string& name);

	/// <summary>
	/// 読み込んだBGMを流す
	/// </summary>
	/// <param name="name">ファイル名</param>
	void PlayBGM(const std::string& name);

	/// <summary>
	/// SEを止める
	/// </summary>
	/// <param name="name">音のファイル名</param>
	void StopSE(const std::string& name = "");

	/// <summary>
	/// BGMを止める
	/// </summary>
	void StopBGM();

	/// <summary>
	/// サウンドが再生中か調べる
	/// </summary>
	/// <returns>1:再生中 0:終了 -1:エラー</returns>
	int CheckSoundFile(const std::string& name);


	////////////////Getter////////////////

	/// <summary>
	/// SEの音量を取得する
	/// </summary>
	/// <returns>音量</returns>
	int GetSEVolume()const { return volumeSE_; };

	/// <summary>
	/// BGMの音量を取得する
	/// </summary>
	/// <returns>音量</returns>
	int GetBGMVolume()const { return volumeBGM_; }


	////////////////Setter////////////////

	/// <summary>
	/// SE音量を調整する
	/// </summary>
	/// <param name="volume">音量</param>
	void SetSEVolume(unsigned int volume);

	/// <summary>
	/// BGMの音量を調整する
	/// </summary>
	/// <param name="volume">音量</param>
	void SetBGMVolume(unsigned int volume);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="rate"></param>
	void SetBGMRate(const float rate);

	/// <summary>
	/// サウンドのポジション、聞こえる範囲を設定する
	/// </summary>
	/// <param name="pos">サウンドのポジション</param>
	/// <param name="audioRange">聞こえる範囲</param>
	/// <param name="name">サウンド名</param>
	void Set3DSoundInfo(const VECTOR& pos, const float audioRange, const std::string& name);

	/// <summary>
	/// 3Dサウンドを再生する際のリスナーの位置を決める
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	void Set3DSoundListenerInfo(const VECTOR& pos, const VECTOR& rot);
private:
	/// <summary>
	/// BGM、SEの音量を外部出力する
	/// </summary>
	void SaveSoundConfig();

	/// <summary>
	/// サウンド情報の読み込み
	/// </summary>
	void LoadSoundConfig();

	/// <summary>
	/// 2DSEをロードする
	/// </summary>
	/// <param name="fileName">拡張子、場所抜きのファイル単体の名前</param>
	/// <returns>ロードしたサウンド</returns>
	int Load2DSoundSEFile(const std::string& fileName);

	/// <summary>
	/// 2DBGMをロードする
	/// </summary>
	/// <param name="fileName">拡張子、場所抜きのファイル単体の名前</param>
	/// <returns>ロードしたサウンド</returns>
	int Load2DSoundBGMFile(const std::string& fileName);

	/// <summary>
	/// 3DSEをロードする
	/// </summary>
	/// <param name="fileName">拡張子、場所抜きのファイル単体の名前</param>
	/// <returns>ロードしたサウンド</returns>
	int Load3DSoundSEFile(const std::string& fileName);

	/// <summary>
	/// 3DBGMをロードする
	/// </summary>
	/// <param name="fileName">拡張子、場所抜きのファイル単体の名前</param>
	/// <returns>ロードしたサウンド</returns>
	int Load3DSoundBGMFile(const std::string& fileName);
private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SoundManager();

	SoundManager(const SoundManager&) = delete;
	void operator = (const SoundManager&) = delete;
private:
	//計　12byte
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

