#include "DxLib.h"
#include "LoadData.h"
#include <fstream>
#include <sstream>

// 定数
namespace
{
	const char* const kFileName = "data/data.csv";
}


/// <summary>
/// コンストラクタ
/// </summary>
LoadData::LoadData()
{
	std::ifstream file(kFileName);
	m_data.clear();
	std::string line;

	// ファイルの入力取得
	// std::getline(読み取るファイルの変数, 入力文字列を格納する変数);
	while (std::getline(file, line))
	{
		std::istringstream stream(line);
		std::string field;

		// 文字列分割
		// getline(istringstream型の変数, 分割した文字列を格納する変数, '何で分割するか')
		while (getline(stream, field, ','))
		{
			// 文字列をfloatに変換してm_dataに追加する
			try
			{
				m_data.push_back(std::stof(field));
			}
			catch (const std::invalid_argument& e)
			{
				// 無効な文字列をスキップ
			}
		}
	}
}


/// <summary>
/// デストラクタ
/// </summary>
LoadData::~LoadData()
{
}


/// <summary>
/// 描画
/// </summary>
void LoadData::Draw()
{
#ifdef _DEBUG
	// 読み込んだデータを表示する
	int y = 0;
	for (const auto& data : m_data)
	{
		DrawFormatString(0, y, 0xffffff, "%f:", data);
		y += 20;
	}
#endif

}
