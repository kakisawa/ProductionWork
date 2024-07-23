#pragma once
#include "EnemyBase.h"
class EnemyRight :
	public EnemyBase
{
public:
	EnemyRight();
	~EnemyRight();

	void Init()override;
	void Update()override;
	void Draw()override;
	void End()override;
};