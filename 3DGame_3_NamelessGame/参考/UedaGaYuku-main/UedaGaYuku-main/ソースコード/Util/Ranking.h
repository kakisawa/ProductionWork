#pragma once
#include <string>
#include <vector>

/// <summary>
/// ランキング情報を更新する
/// </summary>
class Ranking
{
public:
	Ranking();
	~Ranking();
	void CreateRanking(int m_stageKind);			  // ランキング作成
	void UpdateRanking(int stageKind, int clearTime); // ランキング更新
	void UpdateRankInText();						  // ランクイン時のテキストを更新する
	void GetRanking(int stage);						  // ランキング取得
	void DrawStageSelectRanking();					  // ステージ選択時ランキング描画
	void DrawClearRanking(int totalClearTime);		  // クリア時ランキング描画
	int CheckRankIn(int totalClearTime, int index);   // ランキング圏内に入ったかチェックする

private:
	std::string HttpGet(const char* domain, const char* url); // Http通信でGet命令を送る

private:
	int m_rankInTextDispTime;		// ランクインのテキストを表示する時間
	int m_rankInTextAlpha;			// ランクインのテキストのα値

	// ネットワーク用の変数
	std::string uri;
	std::string createRank;
	std::string getRank;

	static const int kDataSize = 2560;	// データサイズ
	char StrBuf[kDataSize] = "";		// データバッファ
	IPDATA Ip;							// 接続用ＩＰアドレスデータ
	int NetHandle;						// ネットワークハンドル
	int DataLength;						// 受信データ量保存用変数
	size_t m_pos;						// 文字の長さを取得する
	int m_lineCount;					// 表示する行数をカウントする
	std::vector<int> m_rankingList;		// クリアタイムを保存する
};

