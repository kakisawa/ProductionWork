#pragma once

/// <summary>
/// “G‚ÌŠî’êƒNƒ‰ƒX
/// </summary>
class EnemyBase
{
public:
	EnemyBase();
	virtual ~EnemyBase();
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

