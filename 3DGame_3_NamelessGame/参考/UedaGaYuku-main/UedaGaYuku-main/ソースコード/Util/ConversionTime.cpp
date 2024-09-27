#include "ConversionTime.h"

namespace Conversion
{
	/// <summary>
	/// ���ɕϊ�����
	/// </summary>
	/// <param name="frame">�t���[����</param>
	/// <returns>��</returns>
	int ChangeMin(int frame)
	{
		return (frame / 60) / 60;
	}

	/// <summary>
	/// �b�ɕϊ�����
	/// </summary>
	/// <param name="frame">�t���[����</param>
	/// <returns>�b</returns>
	int ChangeSec(int frame)
	{
		return (frame / 60) % 60;
	}

	/// <summary>
	/// �~���b�ɕϊ�����
	/// </summary>
	/// <param name="frame">�t���[����</param>
	/// <returns>�~���b</returns>
	int ChangeMilliSec(int frame)
	{
		return (frame * 1000 / 60) % 1000;
	}
}