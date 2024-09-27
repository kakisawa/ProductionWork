#pragma once
#include <string>
#include <DxLib.h>

class UIItem
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	UIItem();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~UIItem();

	/// <summary>
	/// �T�C�Y�����A�A���t�@�l�̕ύX�������UI�`��
	/// </summary>
	/// <param name="scale">�T�C�Y</param>
	/// <param name="alpha">�A���t�@�l</param>
	void AlphaChangeDraw(const float scale, const int alpha);

	/// <summary>
	/// �|�W�V�������ς��UI�̕`��
	/// </summary>
	/// <param name="centerPosX">�|�W�V����X</param>
	/// <param name="centerPosY">�|�W�V����Y</param>
	void ChangePosDraw(const float centerPosX, const float centerPosY);

	/// <summary>
	/// �摜��3D��Ԃɕ`�悷��
	/// </summary>
	/// <param name="drawPos">�`����W</param>
	/// <param name="alpha">�A���t�@�l</param>
	/// <param name="size">�T�C�Y</param>
	void DrawBillboard(const VECTOR& drawPos, const int alpha, const float size);

	/// <summary>
	/// UI�摜���쐬���邽�߂̈ʒu��񓙂�ݒ肷��
	/// </summary>
	/// <param name="centerPosX">�|�W�V������X</param>
	/// <param name="centerPosY">�|�W�V������Y</param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="str">������</param>
	/// <param name="fontHandle">�t�H���g�n���h��</param>
	void CreateUIGraphSetUp(const float centerPosX, const  float centerPosY, const int width, const int height, const std::string& str, const int fontHandle);

	/// <summary>
	/// UI�摜�̍쐬
	/// </summary>
	/// <param name="fontHandle">�t�H���g�n���h��</param>
	void CreateUIGraph(const int fontHandle);

	/// <summary>
	/// ��������擾����
	/// </summary>
	/// <returns>������</returns>
	const std::string& GetString() const { return UIString_; }
private:
	
	int makeScreenHandle_ = 0;				//�쐬�����X�N���[��
	int makeScreenWidth_ = 0;				//�X�N���[���̉���
	int makeScreenHeight_ = 0;				//�X�N���[���̏c��

	float centerPosX_ = 0;					//���S�ʒuX
	float centerPosY_ = 0;					//���S�ʒuY
	float scale_ = 1.0f;					//�g�k��

	std::string UIString_ = {};				//UI�̕�����

};

