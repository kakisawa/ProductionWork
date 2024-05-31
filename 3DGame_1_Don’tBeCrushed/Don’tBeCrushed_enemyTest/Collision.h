#pragma once
class Collision
{
public:
	Collision();
	~Collision();

	void Init();
	void Update();
	void Draw();

private:


	enum Col
	{
		top,
		bottom,
		right,
		left,
	};
};