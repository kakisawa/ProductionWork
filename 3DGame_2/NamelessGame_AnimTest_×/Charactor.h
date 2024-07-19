#pragma once
class Charactor
{
	// しゅるい　
	enum class Type
	{
		None,
		Player,
		Enemy,
	};

public:
	Charactor();
	virtual ~Charactor();

	//モデルハンドルセット
	void SetModelHandle(int handle) { m_model = handle; }

	virtual void Init() {};

	virtual void Update() {};

protected:
	int m_model;		// モデル

	float m_angle;		// 向き角度

};