#pragma once
#include <DxLib.h>
#include <memory>

class Model;
class Player;

class Shot
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="initPos">�����|�W�V����</param>
	/// <param name="moveVec">�ړ��x�N�g��</param>
	Shot(const int handle, const  VECTOR& initPos, const VECTOR& moveVec);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Shot();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �Փ˔���
	/// </summary>
	/// <param name="player">�v���C���[�̎Q��</param>
	void HitCheck(Player& player);

	/// <summary>
	/// ���݂��Ă��邩�ǂ����擾����
	/// </summary>
	/// <returns>true�F���݂��Ă���@false�F���Ă��Ȃ�</returns>
	bool GetIsEnabled() const { return isEnable_; }

private:

	bool isEnable_ = false;						//���݂��Ă��邩

	VECTOR pos_ = {};							//�|�W�V����
	VECTOR moveVec_ = {};						//�ړ��x�N�g��
	VECTOR initialisePos_ = {};					//���������̃|�W�V����

	std::shared_ptr<Model> model_;				//���f���|�C���^�̃��X�g

};

