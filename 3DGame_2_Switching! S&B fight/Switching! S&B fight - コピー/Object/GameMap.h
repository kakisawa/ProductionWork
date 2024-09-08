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

	VECTOR m_mapLeftBack;	// ステージ左奥座標
	VECTOR m_mapRightFront;	// ステージ右前座標
};