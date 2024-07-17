#pragma once
class Charactor
{
	// ����邢�@
	enum class Type
	{
		None,
		Player,
		Enemy,
	};

public:
	Charactor();
	virtual ~Charactor();

	//���f���n���h���Z�b�g
	void SetModelHandle(int handle) { m_model = handle; }

	virtual void Init() {};

	virtual void Update() {};

protected:
	int m_model;		// ���f��

	float m_angle;		// �����p�x

};