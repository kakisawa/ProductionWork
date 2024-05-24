#pragma once
class Enemy
{
public:
	Enemy();
	~Enemy();

	void Init();
	void Update();
	void Draw();
	void End();

	int m_enemyNum = 4;

	int EnemyModelBase;
	float EnemyScale;
	int EnemyModel[m_enemyNum];


	// “G
	VECTOR m_pos[m_enemyNum];
};

