#pragma once
#include <DxLib.h>
#include <string>

namespace MathUtil 
{
	/// <summary>
	/// ��_�Ԃ̋����̃T�C�Y���擾����
	/// </summary>
	/// <param name="pos">�|�W�V����</param>
	/// <param name="targetPos">�ڕW�̃|�W�V����</param>
	/// <returns>��̃|�W�V�����̋����̃T�C�Y</returns>
	float GetSizeOfDistanceTwoPoints(const VECTOR& pos, const VECTOR& targetPos);

	/// <summary>
	/// VECTOR�^�A�x���@���ʓx�@�ɕϊ�����
	/// </summary>
	/// <param name="rot">��]</param>
	/// <returns>�ʓx�@�ɒ�������]</returns>
	VECTOR VECTORDegreeToRadian(VECTOR rot);

	/// <summary>
	/// VECTOR�^�A�ʓx�@��x���@�ɕϊ�����
	/// </summary>
	/// <param name="rot">��]</param>
	/// <returns>�x���@�ɒ�������]</returns>
	VECTOR VECTORRadianToDegree(VECTOR rot);

	/// <summary>
	/// float�^�A�x���@���ʓx�@�ɕϊ�����
	/// </summary>
	/// <param name="angle">�p�x</param>
	/// <returns>�ʓx</returns>
	float DegreeToRadian(float angle);

	/// <summary>
	/// float�^�A�ʓx�@��x���@�ɕϊ�����
	/// </summary>
	/// <param name="angle">�p�x</param>
	/// <returns>�x��</returns>
	float RadianToDegree(float angle);
}

namespace StrUtil 
{
	/// <summary>
	/// �����񂩂琔�l���擾����
	/// </summary>
	/// <param name="name">���O</param>
	/// <param name="sign">�L��</param>
	/// <returns>���l</returns>
	int GetNumberFromString(const std::string& name,const std::string& sign);

	/// <summary>
	/// ������Ɛ��l������������������擾����
	/// </summary>
	/// <param name="name">���O</param>
	/// <param name="sign">�L��</param>
	/// <param name="num">���l</param>
	/// <returns>������</returns>
	std::string GetConcatenateNumAndStrings(const std::string& name,const std::string& sign,const int num);

	/// <summary>
	/// �����񂩂�sign�ȍ~�̕�������폜������������擾����
	/// </summary>
	/// <param name="name">���O</param>
	/// <param name="sign">�L��</param>
	/// <returns>������</returns>
	std::string GetStringWithPartsAfterTheSymbolDeleted(const std::string& name, const std::string& sign);

	/// <summary>
	/// �����񂩂�L���ȍ~�̕�������擾����
	/// </summary>
	/// <param name="name">���O</param>
	/// <param name="sign">�L��</param>
	/// <returns>������</returns>
	std::string GetStringAfterSign(const std::string& name,const std::string& sign);

	/// <summary>
	/// �����񂩂�L���ȑO�̕�������擾����
	/// </summary>
	/// <param name="name">���O</param>
	/// <param name="sign">�L��</param>
	/// <returns>������</returns>
	std::string GetStringBeforeSign(const std::string& name,const std::string& sign);
}