#pragma once
#include "ObjectData.h"
#include <memory>

class Model;

class LightBulb
{
public: 
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="info">�z�u�f�[�^</param>
	LightBulb(const int handle, const ObjectInfo& info);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~LightBulb();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	////////////////Getter////////////////

	/// <summary>
	/// ��]�x�N�g�����擾����
	/// </summary>
	/// <returns>��]�x�N�g��</returns>
	VECTOR GetRotVec() const;

	/// <summary>
	/// ����̃t���[���̍��W���擾����
	/// </summary>
	/// <returns></returns>
	VECTOR GetFramePos() const;
private:

	float elapsedTime_ = 0.0f;			//�o�ߎ���
	float angle_ = 30.0f;				//�p�x
	float targetAngle_ = -30.0f;		//�ڕW�̊p�x

	VECTOR rot_ = {};					//��]�x�N�g��

	std::shared_ptr<Model> model_;		//���f���|�C���^

};

