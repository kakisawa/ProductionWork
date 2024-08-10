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
	
	void SetModelFramePosition(int ModelHandle, char* FrameName, int SetModelHandle);

private:
	int m_sordModel;
};