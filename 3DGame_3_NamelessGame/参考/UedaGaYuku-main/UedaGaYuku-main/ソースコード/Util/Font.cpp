#include "DxLib.h"
#include "Font.h"

namespace
{
	// �t�H���g��
	const char* kFontName = "Yuji Syuku";

	// �ǂݍ��ރt�H���g�t�@�C����
	const LPCSTR kFontDataPath[] =
	{
		"data/font/YujiSyuku-Regular.ttf",
	};

	// �t�H���g�̃f�[�^
	struct FontData
	{
		const char* name;	// �t�H���g��
		int size;			// �t�H���g�T�C�Y
		int thick;			// �t�H���g�̑���(-1:�f�t�H���g)
		int type;			// �t�H���g�̃^�C�v(-1:�f�t�H���g)
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
		{"�l�r�@�S�V�b�N", 16, -1, -1},
	};


	/// <summary>
	/// �t�H���g�̃��[�h
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
				// �t�H���g�Ǎ��G���[����
				MessageBox(NULL, "�t�H���g�Ǎ����s", "", MB_OK);
			}
		}

		// �t�H���g�f�[�^����
		for (int i = 0; i < Font::m_fontHandle.size(); i++)
		{
			Font::m_fontHandle[i] = CreateFontToHandle(data[i].name, data[i].size, data[i].thick, data[i].type);
		}
	}


	/// <summary>
	/// �t�H���g�̃A�����[�h
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
				MessageBox(NULL, "�t�H���g�폜���s", "", MB_OK);
			}
		}

		// �t�H���g�f�[�^�폜
		for (const auto& handle : m_fontHandle)
		{
			DeleteFontToHandle(handle);
		}
	}
}
