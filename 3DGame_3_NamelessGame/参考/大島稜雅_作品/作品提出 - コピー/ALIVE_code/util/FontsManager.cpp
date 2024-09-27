#include "FontsManager.h"
#include <DxLib.h>
#include <cassert>

//コンストラクタ
FontsManager::FontsManager()
{
	AddFonts("ピグモ 00",".otf", 42);
	AddFonts("ピグモ 00",".otf", 33);
	AddFonts("ピグモ 00",".otf", 21);
}

//デストラクタ
FontsManager::~FontsManager()
{
	for (const auto& font : fontHandle_)
	{
		DeleteFontToHandle(font.second);
	}
}

// フォントを加える
void FontsManager::AddFonts(const std::string& filename, const std::string& extension, const int fontSize)
{
	//フォントパス
	std::string path = "data/fonts/";
	path += filename;
	path += extension;

	//フォントの作成
	AddFontResourceEx(path.c_str(), FR_PRIVATE, NULL);

	//フォント名は同じでもサイズで差別化するために
	//フォントサイズを名前の最後に加える
	std::string name = filename + std::to_string(fontSize);

	//作成した名前をキーにハンドルを配列に加える
	fontHandle_[name] = CreateFontToHandle(filename.c_str(), fontSize, -1, -1);
	assert(fontHandle_[filename] != -1);

}

// フォントを適用したテキストの横幅を取得する
int FontsManager::GetStringSize(const std::string& string, const std::string& filename)
{
	int strWidth = GetDrawStringWidthToHandle(string.c_str(), static_cast<int>(strlen(string.c_str())), fontHandle_[filename]);
	return strWidth;
}

// フォントを取得する
int FontsManager::GetFontHandle(const std::string& filename)
{
	return fontHandle_[filename];
}

// 引数の値を元にフォントの名前を返す
std::string FontsManager::GetFontName(const int fontHandle)
{
	std::string fontname = {};

	//引数と同じ値のフォントを持つ配列を探す
	for (const auto& font : fontHandle_)
	{
		if (fontHandle == font.second) 
		{
			fontname = font.first;
		}
	}

	return fontname;
}
