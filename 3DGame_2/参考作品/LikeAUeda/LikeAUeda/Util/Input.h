#pragma once
#include <array>
#include <map>
#include<unordered_map>
#include<string>

/// <summary>
/// 入力管理クラス
/// </summary>

enum class InputType
{
	keybd,	// キーボード
	pad,	// パッド
	mouse	// マウス
};

// コマンドの種類
using InputTable_t = std::unordered_map<std::string, std::map<InputType, int>>;

class Input
{
public:
	// 入力情報の更新
	void Update();

	/// 押された瞬間を取得
	bool IsTriggered(const char* command)const;
	// 今押しているかを取得
	bool IsPressing(const char* command)const;
	// 離された瞬間を取得
	bool IsReleased(const char* command)const;

	// コマンドの種類
	Input();

private:
	// コマンド名と入力をペアにしたテーブル
	InputTable_t m_commandTable;

	// コマンドの入力を覚えておく
	std::map<std::string, bool> m_inputData;		// 現在の入力
	std::map<std::string, bool> m_lastInputData;	// 直前の入力
};

