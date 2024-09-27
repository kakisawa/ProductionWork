#include "DxLib.h"
#include "Font.h"

namespace
{
	// フォント名
	const char* kFontName = "Yuji Syuku";

	// 読み込むフォントファイル名
	const LPCSTR kFontDataPath[] =
	{
		"data/font/YujiSyuku-Regular.ttf",
	};

	// フォントのデータ
	struct FontData
	{
		const char* name;	// フォント名
		int size;			// フォントサイズ
		int thick;			// フォントの太さ(-1:デフォルト)
		int type;			// フォントのタイプ(-1:デフォルト)
	};
}

namespace Font
{
	std::vector<int> m_fontHandle;

	FontData data[] =
	{
		// Size96_4
		{kFontName, 96, 4, DX_FONTTYPE_ANTIALIASING_8X8},
		// Size64_4
		{kFontName, 64, 4, DX_FONTTYPE_ANTIALIASING_8X8},
		// Size50_4
		{kFontName, 50, 4, DX_FONTTYPE_ANTIALIASING_8X8},
		// Size50_4_EDGE
		{kFontName, 50, 4, DX_FONTTYPE_ANTIALIASING_EDGE_8X8},
		// Size48_4
		{kFontName, 48, 4, DX_FONTTYPE_ANTIALIASING_8X8},
		// Size48_4_EDGE
		{kFontName, 48, 4, DX_FONTTYPE_ANTIALIASING_EDGE_8X8},
		// Size42_4
		{kFontName, 42, 4, DX_FONTTYPE_ANTIALIASING_8X8},
		// Size42_4_EDGE
		{kFontName, 42, 4, DX_FONTTYPE_ANTIALIASING_EDGE_8X8},
		// Size40_4
		{kFontName, 40, 4, DX_FONTTYPE_ANTIALIASING_EDGE_8X8},
		// Size36_4_EDGE
		{kFontName, 36, 4, DX_FONTTYPE_ANTIALIASING_EDGE_8X8},
		// Size30_4_EDGE
		{kFontName, 30, 4, DX_FONTTYPE_ANTIALIASING_EDGE_8X8},
		// Size28_4
		{kFontName, 28, 4, DX_FONTTYPE_ANTIALIASING_8X8},
		// Size24_4
		{kFontName, 24, 4, DX_FONTTYPE_ANTIALIASING_8X8},
		// Size24_4_EDGE
		{kFontName, 24, 4,  DX_FONTTYPE_ANTIALIASING_EDGE_8X8},
		// Size16_4
		{kFontName, 16, 4, DX_FONTTYPE_ANTIALIASING_8X8},
		// Normal
		{"ＭＳ　ゴシック", 16, -1, -1},
	};


	/// <summary>
	/// フォントのロード
	/// </summary>
	void Font::Load()
	{
		Font::m_fontHandle.resize(static_cast<int>(Font::FontId::kNum));

		for (auto& fontPath : kFontDataPath)
		{
			if (AddFontResourceEx(fontPath, FR_PRIVATE, NULL) > 0) {
			}
			else
			{
				// フォント読込エラー処理
				MessageBox(NULL, "フォント読込失敗", "", MB_OK);
			}
		}

		// フォントデータ生成
		for (int i = 0; i < Font::m_fontHandle.size(); i++)
		{
			Font::m_fontHandle[i] = CreateFontToHandle(data[i].name, data[i].size, data[i].thick, data[i].type);
		}
	}


	/// <summary>
	/// フォントのアンロード
	/// </summary>
	void Font::UnLoad()
	{
		for (auto& fontPath : kFontDataPath)
		{
			if (RemoveFontResourceEx(fontPath, FR_PRIVATE, NULL))
			{
			}
			else
			{
				MessageBox(NULL, "フォント削除失敗", "", MB_OK);
			}
		}

		// フォントデータ削除
		for (const auto& handle : m_fontHandle)
		{
			DeleteFontToHandle(handle);
		}
	}
}
