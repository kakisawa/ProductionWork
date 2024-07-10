#pragma once
#include <array>
#include <map>
#include<unordered_map>
#include<string>

/// <summary>
/// ���͊Ǘ��N���X
/// </summary>

enum class InputType
{
	keybd,	// �L�[�{�[�h
	pad,	// �p�b�h
	mouse	// �}�E�X
};

// �R�}���h�̎��
using InputTable_t = std::unordered_map<std::string, std::map<InputType, int>>;

class Input
{
public:
	// ���͏��̍X�V
	void Update();

	/// �����ꂽ�u�Ԃ��擾
	bool IsTriggered(const char* command)const;
	// �������Ă��邩���擾
	bool IsPressing(const char* command)const;
	// �����ꂽ�u�Ԃ��擾
	bool IsReleased(const char* command)const;

	// �R�}���h�̎��
	Input();

private:
	// �R�}���h���Ɠ��͂��y�A�ɂ����e�[�u��
	InputTable_t m_commandTable;

	// �R�}���h�̓��͂��o���Ă���
	std::map<std::string, bool> m_inputData;		// ���݂̓���
	std::map<std::string, bool> m_lastInputData;	// ���O�̓���
};

