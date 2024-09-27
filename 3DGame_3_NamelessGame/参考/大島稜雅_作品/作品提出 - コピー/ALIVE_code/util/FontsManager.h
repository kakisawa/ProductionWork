#pragma once
#include <string>
#include <unordered_map>

class FontsManager
{
public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~FontsManager();

	/// <summary>
	/// �C���X�^���X�쐬
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static FontsManager& GetInstance() {
		static FontsManager instance;
		return instance;
	}

	/// <summary>
	/// �t�H���g��������
	/// </summary>
	/// <param name="filename">�t�@�C���̖��O</param>
	/// <param name="extension">�g���q</param>
	/// <param name="fontSize">�t�H���g�̃T�C�Y�w��</param>
	void AddFonts(const std::string& filename, const std::string& extension,const int fontSize);


	////////////////Getter////////////////

	/// <summary>
	/// �t�H���g��K�p�����e�L�X�g�̉������擾����
	/// </summary>
	/// <param name="string">�t�H���g��K�p���镶����</param>
	/// <param name="filename">�t�H���g�̖��O</param>
	/// <returns>�t�H���g��G�����e�L�X�g�̉���</returns>
	int GetStringSize(const std::string& string, const std::string& filename);

	/// <summary>
	/// �t�H���g���擾����
	/// </summary>
	/// <param name="filename">�t�H���g�̖��O</param>
	/// <returns>�쐬�����t�H���g</returns>
	int GetFontHandle(const std::string& filename);

	/// <summary>
	/// �����̒l�����Ƀt�H���g�̖��O��Ԃ�
	/// </summary>
	/// <param name="fontHandle">�t�H���g�n���h��</param>
	/// <returns>�t�H���g��</returns>
	std::string GetFontName(const int fontHandle);

private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	FontsManager();

	FontsManager(const FontsManager&) = delete;
	void operator = (const FontsManager&) = delete;
private:
	std::unordered_map<std::string,int> fontHandle_;		//��������L�[�Ƃ����t�H���g��ۊǂ��邽�߂̔z��
};

