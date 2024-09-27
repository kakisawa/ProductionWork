#include "Util.h"
#include <stdio.h>
#include <algorithm>

namespace MathUtil 
{
	//��_�Ԃ̋����̃T�C�Y���擾����
	float GetSizeOfDistanceTwoPoints(const VECTOR& pos, const VECTOR& targetPos)
	{
		VECTOR distance = {};
		distance = VSub(pos, targetPos);

		return VSize(distance);
	}

	//VECTOR�^�A�x���@���ʓx�@�ɕϊ�����
	VECTOR VECTORDegreeToRadian(VECTOR rot)
	{
		rot.x = rot.x * DX_PI_F / 180.0f;
		rot.y = rot.y * DX_PI_F / 180.0f;
		rot.z = rot.z * DX_PI_F / 180.0f;

		return rot;
	}

	//VECTOR�^�A�ʓx�@��x���@�ɕϊ�����
	VECTOR VECTORRadianToDegree(VECTOR rot)
	{
		rot.x = rot.x / DX_PI_F * 180.0f;
		rot.y = rot.y / DX_PI_F * 180.0f;
		rot.z = rot.z / DX_PI_F * 180.0f;

		return rot;
	}

	//float�^�A�x���@���ʓx�@�ɕϊ�����
	float DegreeToRadian(float angle)
	{
		angle = angle * DX_PI_F / 180.0f;

		return angle;
	}

	//float�^�A�ʓx�@��x���@�ɕϊ�����
	float RadianToDegree(float angle)
	{
		angle = angle / DX_PI_F * 180.0f;

		return angle;
	}

}

namespace StrUtil
{
	//�����񂩂琔�l���擾����
	int StrUtil::GetNumberFromString(const std::string& name, const std::string& sign)
	{
		//������̐����擾
		int strSize = static_cast<int>(name.size());

		//������̐���0�ȉ���������0��Ԃ�
		if (strSize <= 0)
		{
			return 0;
		}

		//�����񂩂������������ԍ����擾����
		int searchStrNum = static_cast<int>(name.find(sign));

		//��L�Ŏ擾�����ԍ�����ꕶ����܂ł��擾
		std::string str = name.substr(searchStrNum + 1, strSize);

		//������𐔒l�ɕϊ�����
		int num = atoi(str.c_str());

		return num;
	}

	//������Ɛ��l������������������擾����
	std::string GetConcatenateNumAndStrings(const std::string& name, const std::string& sign, int num)
	{
		//���l�𕶎���ɂ���
		std::string value = std::to_string(num);

		//name�ɋL��(sign)�ƕ�����ɕϊ�����value(num)��A��
		std::string str = name + sign + value;

		return str;
	}

	//�����񂩂�sign�ȍ~�̕�������폜������������擾����
	std::string GetStringWithPartsAfterTheSymbolDeleted(const std::string& name,const std::string& sign)
	{
		//�������ۊ�
		std::string str = name;

		//sing��������Ɋ܂܂�Ă��邩�T��
		if (name.find(sign) != -1) 
		{
			//sign�ȍ~�̕�������폜������������擾����
			str = str.erase(name.find(sign));
		}

		return str;
	}

	//�����񂩂�L���ȍ~�̕�������擾����
	std::string GetStringAfterSign(const std::string& name,const std::string& sign)
	{
		//������̃T�C�Y���擾
		int size = static_cast<int>(name.size());

		//�u.�v��������̉��Ԗڂ����擾����
		int dotNum = static_cast<int>(name.find(sign));

		//�u.�v�ȍ~����ꕶ����܂ł̕�������擾����
		std::string str = name.substr(dotNum + 1, size);

		return str;
	}

	//�����񂩂�L���ȑO�̕�������擾����
	std::string GetStringBeforeSign(const std::string& name,const std::string& sign)
	{
		//�u.�v��������̉��Ԗڂ����擾����
		int dotNum = static_cast<int>(name.find(sign));

		//�u.�v�ȍ~����Ō�܂ł̕�������擾����
		std::string str = name.substr(0, dotNum);

		return str;
	}
}