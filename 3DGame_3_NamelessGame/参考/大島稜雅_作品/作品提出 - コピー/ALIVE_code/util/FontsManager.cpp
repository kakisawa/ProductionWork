#include "FontsManager.h"
#include <DxLib.h>
#include <cassert>

//�R���X�g���N�^
FontsManager::FontsManager()
{
	AddFonts("�s�O�� 00",".otf", 42);
	AddFonts("�s�O�� 00",".otf", 33);
	AddFonts("�s�O�� 00",".otf", 21);
}

//�f�X�g���N�^
FontsManager::~FontsManager()
{
	for (const auto& font : fontHandle_)
	{
		DeleteFontToHandle(font.second);
	}
}

// �t�H���g��������
void FontsManager::AddFonts(const std::string& filename, const std::string& extension, const int fontSize)
{
	//�t�H���g�p�X
	std::string path = "data/fonts/";
	path += filename;
	path += extension;

	//�t�H���g�̍쐬
	AddFontResourceEx(path.c_str(), FR_PRIVATE, NULL);

	//�t�H���g���͓����ł��T�C�Y�ō��ʉ����邽�߂�
	//�t�H���g�T�C�Y�𖼑O�̍Ō�ɉ�����
	std::string name = filename + std::to_string(fontSize);

	//�쐬�������O���L�[�Ƀn���h����z��ɉ�����
	fontHandle_[name] = CreateFontToHandle(filename.c_str(), fontSize, -1, -1);
	assert(fontHandle_[filename] != -1);

}

// �t�H���g��K�p�����e�L�X�g�̉������擾����
int FontsManager::GetStringSize(const std::string& string, const std::string& filename)
{
	int strWidth = GetDrawStringWidthToHandle(string.c_str(), static_cast<int>(strlen(string.c_str())), fontHandle_[filename]);
	return strWidth;
}

// �t�H���g���擾����
int FontsManager::GetFontHandle(const std::string& filename)
{
	return fontHandle_[filename];
}

// �����̒l�����Ƀt�H���g�̖��O��Ԃ�
std::string FontsManager::GetFontName(const int fontHandle)
{
	std::string fontname = {};

	//�����Ɠ����l�̃t�H���g�����z���T��
	for (const auto& font : fontHandle_)
	{
		if (fontHandle == font.second) 
		{
			fontname = font.first;
		}
	}

	return fontname;
}
