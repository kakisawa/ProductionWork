#pragma once
#include <DxLib.h>
#include <vector>

class Effect2D
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle">�摜</param>
	/// <param name="size">�`��T�C�Y</param>
	/// <param name="pos">�`��ꏊ</param>
	Effect2D(const std::vector<int>& handle,const float size,const VECTOR& pos);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Effect2D();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �摜��3D��Ԃɕ`�悷��
	/// </summary>
	void DrawBillboard();

	/// <summary>
	/// ���݂��Ă��邩�ǂ����擾����
	/// </summary>
	/// <returns>true�F���Ă���@false�F���Ă��Ȃ�</returns>
	bool IsEnable() const { return isEnabled_; }
private:

	int currentNum_ = 0;			//���݂̔ԍ�

	float drawSize_ = 0.0f;			//�`��T�C�Y

	bool isEnabled_ = false;		//���݂��Ă��邩

	VECTOR drawPos_ = {};			//�`��ꏊ

	std::vector<int> handle_;		//�摜�n���h��
};

