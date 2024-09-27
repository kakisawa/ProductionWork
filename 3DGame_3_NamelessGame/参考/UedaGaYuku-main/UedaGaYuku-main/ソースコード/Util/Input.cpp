#include "Input.h"
#include "DxLib.h"
#include <string>
#include <algorithm>

/// <summary>
/// ���͏��̍X�V
/// </summary>
void Input::Update()
{
	// ���O�̓��͂��R�s�[
	m_lastInputData = m_inputData;

	/*�n�[�h�E�F�A���̓`�F�b�N*/
	// �p�b�h���̎擾
	int padstate = GetJoypadInputState(DX_INPUT_PAD1);
	// ���݂̃L�[�{�[�h���͂��擾
	char keystate[256];
	GetHitKeyStateAll(keystate);

	// m_inputData�̓��e���X�V
	for (const auto& cmd : m_commandTable)
	{
		auto& input = m_inputData[cmd.first];	// �R�}���h�̖��O������̓f�[�^�����
		for (const auto& hardIO : cmd.second)
		{
			// �L�[�{�[�h�̃`�F�b�N
			input = false;
			if (hardIO.first == InputType::keybd)
			{
				if (keystate[hardIO.second])
				{
					input = true;
					break;
				}
			}
			// �p�b�h�̃`�F�b�N
			else if (hardIO.first == InputType::pad)
			{
				if (padstate & hardIO.second)
				{
					input = true;
					break;
				}
			}
		}
	}
}


/// <summary>
/// �����ꂽ�u�Ԃ��擾
/// </summary>
/// <param name="command">�R�}���h������</param>
/// <returns>ture:�����ꂽ�u��</returns>
bool Input::IsTriggered(const char* command) const
{
	auto it = m_inputData.find(command);

	// �v�f���Ȃ��ꍇ
	if (it == m_inputData.end())
	{
		return false;
	}

	return(m_inputData.at(command) && !m_lastInputData.at(command));
}


/// <summary>
/// �������Ă��邩���擾
/// </summary>
/// <param name="command">�R�}���h������</param>
/// <returns>true:������Ă���</returns>
bool Input::IsPressing(const char* command) const
{
	auto it = m_inputData.find(command);

	// �v�f���Ȃ��ꍇ
	if (it == m_inputData.end()	)
	{
		return false;
	}

	return m_inputData.at(command);
}


/// <summary>
/// �����ꂽ�u�Ԃ��擾
/// </summary>
/// <param name="command">�R�}���h������</param>
/// <returns>true:�����ꂽ�u��</returns>
bool Input::IsReleased(const char* command) const
{
	auto it = m_inputData.find(command);

	// �v�f���Ȃ��ꍇ
	if (it == m_inputData.end())
	{
		return false;
	}

	return(!m_inputData.at(command) && m_lastInputData.at(command));
}


/// <summary>
/// �R�}���h�̎��
/// </summary>
Input::Input()
{
	m_commandTable["OK"] = { {InputType::keybd, KEY_INPUT_RETURN},
							 {InputType::pad, PAD_INPUT_A} };

	m_commandTable["back"] = { {InputType::keybd, KEY_INPUT_Q},
							   {InputType::pad, PAD_INPUT_B} };

	m_commandTable["A"] = { {InputType::keybd, KEY_INPUT_RETURN},
							{InputType::pad, PAD_INPUT_A} };

	m_commandTable["B"] = { {InputType::keybd, KEY_INPUT_B},
							{InputType::pad, PAD_INPUT_B} };

	m_commandTable["X"] = { {InputType::keybd, KEY_INPUT_X},
							{InputType::pad, PAD_INPUT_C} };

	m_commandTable["Y"] = { {InputType::keybd, KEY_INPUT_Y},
							{InputType::pad, PAD_INPUT_X} };

	m_commandTable["RB"] = { { InputType::keybd, KEY_INPUT_P },
									{InputType::pad, PAD_INPUT_Z } },

	m_commandTable["LB"] = { { InputType::keybd, KEY_INPUT_Q },
								{InputType::pad, PAD_INPUT_Y } },

	m_commandTable["up"] = { {InputType::keybd, KEY_INPUT_UP},
							 {InputType::pad, PAD_INPUT_UP} };

	m_commandTable["down"] = { {InputType::keybd, KEY_INPUT_DOWN},
							   {InputType::pad, PAD_INPUT_DOWN} };
	
	m_commandTable["left"] = { {InputType::keybd, KEY_INPUT_LEFT},
							   {InputType::pad, PAD_INPUT_LEFT} };

	m_commandTable["right"] = { {InputType::keybd, KEY_INPUT_RIGHT},
							    {InputType::pad, PAD_INPUT_RIGHT} };

	m_commandTable["jump"] = { {InputType::keybd, KEY_INPUT_SPACE},
							   {InputType::pad, PAD_INPUT_A} };

	m_commandTable["pause"] = { {InputType::keybd, KEY_INPUT_TAB},
								{InputType::pad, PAD_INPUT_R} };

	m_commandTable["operation"] = { {InputType::keybd, KEY_INPUT_Q},
									{InputType::pad, PAD_INPUT_L} };

	m_commandTable["sceneChange"] = { {InputType::keybd, KEY_INPUT_Z},
									  {InputType::pad, PAD_INPUT_10} };

	/*�U��*/
	m_commandTable["punch"] = { {InputType::keybd, KEY_INPUT_Z},
								{InputType::pad, PAD_INPUT_C} };
	m_commandTable["kick"] = { {InputType::keybd, KEY_INPUT_X},
							   {InputType::pad, PAD_INPUT_X} };
	m_commandTable["avoidance"] = { { InputType::keybd, KEY_INPUT_C },
									{InputType::pad, PAD_INPUT_A } },
	m_commandTable["fighting"] = { { InputType::keybd, KEY_INPUT_SPACE },
									{InputType::pad, PAD_INPUT_Z } },
	m_commandTable["guard"] = { { InputType::keybd, KEY_INPUT_Q },
									{InputType::pad, PAD_INPUT_Y } },
	m_commandTable["special"] = { { InputType::keybd, KEY_INPUT_W },
								{InputType::pad, PAD_INPUT_B } },

	/*�J������]*/
	// ��
	m_commandTable["rotateL"] = { {InputType::keybd, KEY_INPUT_A},
								  {InputType::pad, PAD_INPUT_Y} };
	// �E
	m_commandTable["rotateR"] = { {InputType::keybd, KEY_INPUT_D},
								  {InputType::pad, PAD_INPUT_Z} };
	// ��
	m_commandTable["rotateU"] = { {InputType::keybd, KEY_INPUT_W},
							      {InputType::pad, PAD_INPUT_10} };
	// ��
	m_commandTable["rotateD"] = { {InputType::keybd, KEY_INPUT_S},
								  {InputType::pad, PAD_INPUT_10} };

	/*�f�o�b�O�@�\*/
	m_commandTable["debug_damage"] = { {InputType::keybd, KEY_INPUT_LSHIFT},
								  {InputType::pad, PAD_INPUT_11} };
	m_commandTable["debug_pause"] = { {InputType::keybd, KEY_INPUT_P},
									  {InputType::pad, PAD_INPUT_11} };
	m_commandTable["debug_enter"] = { {InputType::keybd, KEY_INPUT_RETURN},
									  {InputType::pad, PAD_INPUT_11} };
	m_commandTable["debug_clear"] = { {InputType::keybd, KEY_INPUT_B},
									  {InputType::pad, PAD_INPUT_10} };
	m_commandTable["debug_gameover"] = { {InputType::keybd, KEY_INPUT_N},
										 {InputType::pad, PAD_INPUT_10} };
	m_commandTable["end"] = { {InputType::keybd, KEY_INPUT_ESCAPE},
							  {InputType::pad, PAD_INPUT_10} };
}