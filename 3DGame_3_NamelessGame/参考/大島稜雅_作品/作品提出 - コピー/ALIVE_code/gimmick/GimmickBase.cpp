#include "GimmickBase.h"

#include "../object/Player.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/ExternalFile.h"

//�쐬�ς݂̃��f�����R�s�[����R���X�g���N�^
GimmickBase::GimmickBase(const int handle, const Material materialType, const ObjectInfo& objInfo): OrnamentBase(handle, materialType,objInfo)
{
}

//�f�X�g���N�^
GimmickBase::~GimmickBase()
{
}

//�w��̃I�u�W�F�N�g�̔z�u�f�[�^���擾����
ObjectInfo GimmickBase::GetLoadObjectInfo(const std::string& name, int num)
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	//���O�ɔԍ��𑫂�����������擾����
	std::string str = StrUtil::GetConcatenateNumAndStrings(name, ".", num);

	//��L�Ŏ擾����������̃I�u�W�F�N�g�̔z�u�f�[�^���擾����	
	ObjectInfo objInfo = file.GetSpecifiedGimmickInfo(str);

	//�z�u�f�[�^��Ԃ�
	return objInfo;
}