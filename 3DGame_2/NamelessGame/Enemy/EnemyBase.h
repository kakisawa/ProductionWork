#pragma once
#include "DxLib.h"

class EnemyBase
{
public:
	EnemyBase(const char*);
	virtual ~EnemyBase();

	virtual void Init();
	virtual void Update();
	virtual void Draw();
	virtual void End();

protected:
	int m_model;		// �G���f��

	VECTOR m_pos;		// �G���W
};