#pragma once
#include "../object/ObjectData.h"
#include <list>
#include <memory>
#include <DxLib.h>

class Player;
class ObjectBase;
class Model;

class Switch
{
private:
	//�X�C�b�`�̃A�j���[�V����
	enum class Anim
	{
		On,
		Off,
	};
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="objInfo">�z�u�f�[�^</param>
	Switch(const ObjectInfo& objInfo);
	
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Switch();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="player">�v���C���[�̎Q��</param>
	void Update(Player& player);
	
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �X�C�b�`���f���Ǝ��̂̏Փ˔�����s��
	/// </summary>
	/// <param name="deadPerson">���̂̃|�C���^</param>
	void HitColl(const std::shared_ptr<ObjectBase>& corpse);

	/// <summary>
	/// �G���x�[�^�[�̏Փ˔���̌���
	/// </summary>
	/// <returns>true�F�Փˁ@false�F�Փ˂��Ă��Ȃ�</returns>
	bool ElevatorCollResult();

	/// <summary>
	/// ���߃I�u�W�F�N�g�̏Փ˔���̌���
	/// </summary>
	/// <returns>true�F�Փˁ@false�F�Փ˂��Ă��Ȃ�</returns>
	bool TransCollResult();

	/// <summary>
	/// �Փˌ��ʂ̍폜
	/// </summary>
	void DeleteHitResult();

	/// <summary>
	/// �N��������ύX����
	/// </summary>
	/// <param name="time">����</param>
	void ChangeDuringStartup(const float time);

	/// <summary>
	/// �T�E���h��炷
	/// </summary>
	void PlayOnSound();

	////////////////Getter////////////////

	/// <summary>
	/// �N������
	/// </summary>
	/// <returns>true : �N���� false : �N�����Ă��Ȃ�</returns>
	bool GetIsDuringStartup()  const { return isDuringStartup_; }

	/// <summary>
	/// �Փ˔�����s�����f����ǉ�����
	/// </summary>
	/// <returns>�Փ˔�����s�����f���̃|�C���^</returns>
	const std::shared_ptr<Model>& GetModelPointer() const;

private:
	/// <summary>
	/// �X�C�b�`���f���ƃv���C���[�̏Փ˔�����s��
	/// </summary>
	/// <param name="player">�v���C���[�̎Q��</param>
	void HitCollPlayer(Player& player);

	/// <summary>
	/// �X�C�b�`���I���ɂ���
	/// </summary>
	void OnAnim();

	/// <summary>
	/// �X�C�b�`���I�t�ɂ���
	/// </summary>
	void OffAnim();
private:

	bool isDuringStartup_ = false;									//�X�C�b�`�������ꂽ���@true�Fon�@false�Foff

	VECTOR pos_ = {};									//�|�W�V����

	std::shared_ptr<Model> model_;						//���f���N���X�̃X�}�[�g�|�C���^
	std::list<MV1_COLL_RESULT_POLY_DIM> hitDim_;		//�Փ˔���̔z��

	void(Switch::* stateFunc_)();
};

