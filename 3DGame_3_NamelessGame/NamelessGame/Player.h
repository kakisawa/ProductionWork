#pragma once
#include "DxLib.h"

class Player
{
public:
	Player();
	~Player();

	void Init();
	void Update();
	void Draw();

	VECTOR GetPos() const { return pos; }

private:
	int model;

	VECTOR pos;
};