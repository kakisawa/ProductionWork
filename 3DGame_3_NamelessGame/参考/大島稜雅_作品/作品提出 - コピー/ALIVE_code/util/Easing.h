#pragma once
#include <cmath>
#include <algorithm>

/// <summary>
/// �Q�l�T�C�g
/// https://game-ui.net/?p=835
/// https://qiita.com/HnniTns/items/9e3799d3b414dccbbd7d
/// </summary>

namespace Easing {

	template <typename T>
	/// <summary>
	/// �n�߂ƍŌ�͊ɂ₩�ɒ��Ԃł͋}�ɂ���C�[�W���O�֐�
	/// </summary>
	/// <typeparam name="T">�e���v���[�g�^</typeparam>
	/// <param name="elapsedTime">�o�ߎ���</param>
	/// <param name="totalTime">���b�����Ĉړ����邩</param>
	/// <param name="maxValue">�ő�l</param>
	/// <param name="currentValue">���݂̒l</param>
	/// <returns>�l</returns>
	T InOutCubic(T elapsedTime, T totalTime,T maxValue,T currentValue);
	
	template <typename T>
	/// <summary>
	/// �n�߂͋}�ōŌ�ɂȂ�ɂ�Ċɂ₩�ɂȂ�
	/// </summary>
	/// <typeparam name="T">�e���v���[�g�^</typeparam>
	/// <param name="elapsedTime">�o�ߎ���</param>
	/// <param name="totalTime">���b�����Ĉړ����邩</param>
	/// <param name="maxValue">�ő�l</param>
	/// <param name="currentValue">���݂̒l</param>
	/// <returns>�l</returns>
	T OutCirc(T elapsedTime, T totalTime, T maxValue, T currentValue);

	template <typename T>

	/// <summary>
	/// �n�߂ƍŌ�͊ɂ₩�ɒ��Ԃł͋}�ɂ���C�[�W���O�֐��@InOutCubic���������͂������
	/// </summary>
	/// <typeparam name="T">�e���v���[�g�^</typeparam>
	/// <param name="elapsedTime">�o�ߎ���</param>
	/// <param name="totalTime">���b�����Ĉړ����邩</param>
	/// <param name="maxValue">�ő�l</param>
	/// <param name="currentValue">���݂̒l</param>
	/// <returns>�l</returns>
	T InOutSine(T elapsedTime, T totalTime, T maxValue, T currentValue);
}

namespace Easing
{
	template <typename T>
	T InOutCubic(T elapsedTime, T totalTime, T maxValue, T currentValue) {
		maxValue -= currentValue;
		elapsedTime /= totalTime;

		if (elapsedTime / 2.0f < 1.0f) {
			return maxValue / 2.0f * elapsedTime * elapsedTime * elapsedTime + currentValue;
		}

		elapsedTime -= maxValue;

		return maxValue / 2.0f * (elapsedTime * elapsedTime * elapsedTime + 2.0f) + currentValue;
	}

	template <typename T>
	T OutCirc(T elapsedTime, T totalTime, T maxValue, T currentValue) {
		maxValue -= currentValue;
		elapsedTime /= (totalTime - 1.0f);

		elapsedTime = (std::max)((std::min)(elapsedTime, 1.0f), -1.0f);

		return maxValue * std::sqrt(1.0f - elapsedTime * elapsedTime) + currentValue;
	}

	template <typename T>
	T InOutSine(T elapsedTime, T totalTime, T maxValue, T currentValue) {
		maxValue -= currentValue;

		return -maxValue / 2 * (std::cos(elapsedTime * DX_PI_F / totalTime) - 1) + currentValue;
	}
}
