#pragma once
#include "DxLib.h"

class Map
{
public:
	Map();
	virtual ~Map();

	void Init();
	void Update();
	void Draw();

	struct PointPos {
		VECTOR point1;
		VECTOR point2;
		VECTOR point3;
		VECTOR point4;
	}m_pointPos;

	PointPos GetPointPos() const { return m_pointPos; }

private:
};

