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

	int bgGraph;	// �w�i�摜

	VECTOR m_stageUpperLeft;	// �X�e�[�W������W
	VECTOR m_stageLowerRight;	// �X�e�[�W�E�����W
};