#pragma once
#include <vector>

/// <summary>
/// サウンド管理
/// </summary>
namespace Sound
{
	//BGMの種類
	enum class BgmKind
	{
		kTitle,			// タイトルBGM
		kStageSelect,	// ステージ選択BGM
		kStage1,		// ステージ1BGM
		kStage2,		// ステージ2BGM
		kClear,			// クリアBGM
		kGameover,		// ゲームオーバーBGM
		kBgmNum			// BGMの数
	};

	// SEの種類
	enum class SeKind
	{
		kTitleDisp,			// タイトル表示
		kCursor,			// カーソル移動
		kSelect,			// 決定
		kWalk,				// 歩き
		kRun,				// 走り
		kAttack,			// 攻撃
		kGuardAttack,		// ガード時攻撃
		kAccumulateGauge,	// 必殺技ゲージが溜まった時
		kSpecialAttack,		// 必殺技
		kAvoid,				// 回避
		kBattleStart,		// バトル開始時
		kLastAttack,		// 最後に攻撃した時
		kBattleEnd,			// バトル終了時
		kClearCheers,		// クリア時の歓声
		kSeNum				// SEの数
	};

	void Load();
	void UnLode();
	void ChangeBgmVol(Input& input); // BGMの音量を変更する
	void ChangeSeVol(Input& input);  // SEの音量を変更する
	int GetBgmVol();				 // BGMの音量を取得
	int GetSeVol();					 // SEの音量を取得

	extern std::vector<int> m_bgmHandle;
	extern std::vector<int> m_seHandle;
};

