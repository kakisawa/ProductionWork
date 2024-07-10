#pragma once
#include <vector>

/// <summary>
/// 外部ファイル読み込み
/// </summary>
class LoadData
{
public:
	LoadData();
	~LoadData();
	void Draw();

	struct PlayerData
	{
		float maxHP;		// 最大HP
		float maxHeatGauge;	// 最大ヒートゲージ量
		float attack;		// 攻撃力
		float maxMoveSpeed; // 最大移動速度
		float acceleration; // 加速度
		float deceleration;	// 減速度
	};

private:
	std::vector<float> m_data;
};