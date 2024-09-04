#pragma once
#include<map>
#include<string>

class SoundManager
{
public:

	//BGMの種類
	enum class BGM_Type
	{
		kTitleBGM,		// タイトルBGM
		kGameBGM,		// ゲームBGM
		kGameClearBGM,	// ゲームクリアBGM
		kGameOverBGM,	// ゲームオーバーBGM
		kOptionBGM,		// オプションBGM
		MAX
	};

	//SEの種類
	enum class SE_Type
	{
		kSelectSE,		// セレクトSE
		kButtonSE,		// 決定SE
		kBackSE,		// キャンセルSE
		MAX,
	};

	//サウンドの初期化
	void InitSound(void);

	//BGMのパスの初期化
	void InitBGM(void);

	/// <summary>
	/// BGMのロード
	/// </summary>
	/// <param name="_bgm">BGMの種類</param>
	void LoadBGM(BGM_Type bgm);

	//SEのパスの初期化
	void InitSE(void);

	/// <summary>
	/// SEのロード	
	/// </summary>
	/// <param name="_bgm">SEの種類</param>
	void LoadSE(SE_Type se);

	/// <summary>
	/// BGMの再生
	/// </summary>
	/// <param name="_bgm">BGMの種類</param>
	/// <param name="_playType">再生形式</param>
	/// <param name="_volumePar">音量(%表記)</param>
	/// <param name="_topPositionFlag">最初から再生を始めるか (true:最初から)</param>
	void PlayBGM(BGM_Type bgm, int playType, int volumePar = 100, bool topPositionFlag = true);

	/// <summary>
	/// SEの再生
	/// </summary>
	/// <param name="_se">SEの種類</param>
	/// <param name="_playType">再生形式</param>
	/// <param name="_volumePar">音量(%表記)</param>
	/// <param name="_topPositionFlag">最初から再生を始めるか (true:最初から)</param>
	void PlaySE(SE_Type se, int playType, int volumePar = 100, bool topPositionFlag = true);

	/// <summary>
	/// BGMが再生中かどうかを返す
	/// </summary>
	/// <param name="_bgm">BGMの種類</param>
	/// <returns>true:再生中</returns>
	bool CheckPlayBGM(BGM_Type bgm);

	/// <summary>
	/// SEが再生中かどうかを返す
	/// </summary>
	/// <param name="_se">SEの種類</param>
	/// <returns>true:再生中</returns>
	bool CheckPlaySE(SE_Type se);

	/// <summary>
	/// BGMを止める
	/// </summary>
	/// <param name="_bgm">止めるBGM</param>
	void StopBGM(BGM_Type bgm);

	/// <summary>
	/// SEを止める
	/// </summary>
	/// <param name="_se">止めるSE</param>
	void StopSE(SE_Type se);

	//サウンドの解放
	void ReleaseSound(void);

private:

	//サウンドのパス
	std::map<BGM_Type, std::string> m_bgmPass;	//BGMのパス

	std::map<SE_Type, std::string> m_sePass;		//SEのパス

	//サウンドのハンドル
	std::map<BGM_Type, int> m_bgm;	//BGMハンドル

	std::map<SE_Type, int> m_se;		//SEハンドル
};