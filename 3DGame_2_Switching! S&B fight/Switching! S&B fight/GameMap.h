#pragma once

class GameMap
{
public:
	GameMap();
	~GameMap();

	void Init();
	void Update();
	void Draw();
	void End();

	VECTOR GetMapLeftBack() { return m_mapLeftBack; }
	VECTOR GetMapRightFront() { return m_mapRightFront; }

private:
	int m_model;

	VECTOR m_mapLeftBack;	// �X�e�[�W�������W
	VECTOR m_mapRightFront;	// �X�e�[�W�E�O���W
};