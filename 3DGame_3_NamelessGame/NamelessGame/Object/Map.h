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



	VECTOR GetPointPos1() const { return m_pointPos1; }
	VECTOR GetPointPos2() const { return m_pointPos2; }
	VECTOR GetPointPos3() const { return m_pointPos3; }
	VECTOR GetPointPos4() const { return m_pointPos4; }

private:

	VECTOR m_pointPos1;
	VECTOR m_pointPos2;
	VECTOR m_pointPos3;
	VECTOR m_pointPos4;
};

