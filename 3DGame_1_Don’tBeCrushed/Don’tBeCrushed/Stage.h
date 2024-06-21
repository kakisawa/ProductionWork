#pragma once
class Stage
{
public:
	Stage();
	virtual ~Stage();

	void Init();
	void Update();
	void Draw();

	VECTOR GetStageUpperLeft() { return m_stageUpperLeft; }
	VECTOR GetStageLowerRight() { return m_stageLowerRight; }

private:

	int bgGraph;	// 背景画像

	VECTOR m_stageUpperLeft;	// ステージ左上座標
	VECTOR m_stageLowerRight;	// ステージ右下座標
};