#pragma once
class Stage
{
public:
	Stage();
	virtual ~Stage();

	void Init();
	void Update();
	void Draw();


private:

	int bgGraph;

	int m_stageUpperLeftX;
	int m_stageUpperRightX;
	int m_stageLowerLeftZ;
	int m_stageLowerRightZ;
};