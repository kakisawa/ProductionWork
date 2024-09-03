#pragma once
#include "EnemyBase.h"
class EnemyLeft :
    public EnemyBase
{
public:
	EnemyLeft();
	~EnemyLeft();
	
	void Init()override;
	void Update()override;
	void Draw()override;
	void End()override;

	void SetModelFramePosition(int ModelHandle, char *FrameName, int SetModelHandle);

private:
	int m_sordModel;
};

