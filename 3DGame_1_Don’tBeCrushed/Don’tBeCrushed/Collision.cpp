#include "Collision.h"
#include "Game.h"
#include <cassert>

namespace {
	const VECTOR VecInit = VGet(0.0, 0.0, 0.0);
}

Collision::Collision() :
	p1{ VecInit },
	e1{ VecInit }
{
}

Collision::~Collision()
{
}

void Collision::PlayerDraw(unsigned int color, bool isFill)
{
	DrawCube3D(p1.m_leftBottom, p1.m_rightUp, color, 0x000000, isFill);

	DrawFormatString(0, 420, 0x00ffff, "p1.m_rightBottom.x=%.0f:z=%0.f",
		p1.m_rightBottom.x, p1.m_rightBottom.z);

	//// �v���C���[���W�m�F�p
	//DrawString(0, 800 - 20, "Player", 0xffffff);
	//DrawFormatString(0, 800, 0xffffff, "m_leftBottom.x=%.2f,z=%.2f",
	//	p1.m_leftBottom.x, p1.m_leftBottom.z);
	//DrawFormatString(0, 820, 0xffffff, "m_leftUp.x = % .2f, z = % .2f",
	//	p1.m_leftUp.x, p1.m_leftUp.z);
	//DrawFormatString(0, 840, 0xffffff, "m_rightBottom.x=%.2f,z=%.2f",
	//	p1.m_rightBottom.x, p1.m_rightBottom.z);
	//DrawFormatString(0, 860, 0xffffff, "m_rightUp.x=%.2f,z=%.2f",
	//	p1.m_rightUp.x, p1.m_rightUp.z);
}

void Collision::EnemyDraw(unsigned int color, bool isFill)
{
	DrawCube3D(e1.m_leftBottom, e1.m_rightUp, color, 0x000000, isFill);

	DrawFormatString(0, 300, color, "e1.m_leftUp.x=%.0f:z=%0.f", e1.m_leftUp.x, e1.m_leftUp.z);
	DrawFormatString(0, 330, color, "e1.m_leftBottom.x=%.0f:z=%0.f", e1.m_leftBottom.x, e1.m_leftBottom.z);
	DrawFormatString(0, 360, color, "e1.m_rightBottom.x=%.0f:z=%0.f", e1.m_rightBottom.x, e1.m_rightBottom.z);
	DrawFormatString(0, 390, color, "e1.m_rightUp.x=%.0f:z=%0.f", e1.m_rightUp.x, e1.m_rightUp.z);
	


	//// �G���W�m�F�p
	//DrawString(0, 500 - 20, "Enemy", 0xffffff);
	//DrawFormatString(0, 500, 0xffffff, "m_leftBottom.x=%.2f,z=%.2f",
	//	e1.m_leftBottom.x, e1.m_leftBottom.z);
	//DrawFormatString(0, 520, 0xffffff, "m_leftUp.x = % .2f, z = % .2f",
	//	e1.m_leftUp.x, e1.m_leftUp.z);
	//DrawFormatString(0, 540, 0xffffff, "m_rightBottom.x=%.2f,z=%.2f",
	//	e1.m_rightBottom.x, e1.m_rightBottom.z);
	//DrawFormatString(0, 560, 0xffffff, "m_rightUp.x=%.2f,z=%.2f",
	//	e1.m_rightUp.x, e1.m_rightUp.z);
}

void Collision::SetLB(VECTOR pos, float width, float height)
{
	p1.m_leftBottom = VSub(pos, VGet(width*0.5f, 0, height * 0.5f));	// �������W
	p1.m_leftUp = VAdd(p1.m_leftBottom, VGet(0.0f, 0.0f, height));			// ������W
	p1.m_rightUp = VAdd(pos, VGet(width * 0.5f, 0, height * 0.5f));	// �E����W
	p1.m_rightBottom = VAdd(p1.m_leftBottom, VGet(height, 0.0f, 0.0f));	// �E�����W
}

/// <summary>
/// �c�����̓G��(���Ղ��Č����ꍇ��)�������W����E����W���Z�b�g
/// </summary>
/// <param name="pos">�G���W</param>
/// <param name="width">�{�̃T�C�Y(�w�\����)</param>
/// <param name="height">�{�̃T�C�Y(�c�̒���)</param>
/// <param name="depth">�{�̃T�C�Y(���s/����)</param>
void Collision::SetPortrait(VECTOR pos, float width, float height, float depth, bool flag)
{	
	if (flag==false)
	{
		e1.m_leftUp = VAdd(pos, VGet(width, 0.0f, depth * 0.5f));
		e1.m_leftBottom = VAdd(e1.m_leftUp, VGet(0.0f, 0.0f, -depth));//
		e1.m_rightUp = VAdd(e1.m_leftUp, VGet(height, 0.0f, 0.0f));//
		e1.m_rightBottom = VAdd(e1.m_leftBottom, VGet(height, 0.0f, 0.0f));
	}
	else
	{
		e1.m_leftUp = VAdd(pos, VGet(-(height + width), 0.0f, -(depth * 0.5f)));
		e1.m_leftBottom = VAdd(e1.m_leftUp, VGet(0.0f, 0.0f, depth));//
		e1.m_rightUp = VAdd(e1.m_leftUp, VGet(height, 0.0f, 0.0f));//
		e1.m_rightBottom = VAdd(e1.m_rightUp, VGet(0.0f, 0.0f, -depth));
	}
}

/// <summary>
/// �������̓G��(���Ղ��Č����ꍇ��)�������W����E����W���Z�b�g
/// </summary>
/// <param name="pos">�G���W</param>
/// <param name="width">�{�̃T�C�Y(�w�\����)</param>
/// <param name="height">�{�̃T�C�Y(�c�̒���)</param>
/// <param name="depth">�{�̃T�C�Y(���s/����)</param>
void Collision::SetLandscape(VECTOR pos, float width, float height, float depth, bool flag)
{
	if (!flag)
	{
		e1.m_leftUp = VAdd(pos, VGet(-(height*0.70), 0.0f, -width*0.8 ));
		e1.m_leftBottom = VAdd(e1.m_leftUp, VGet(0.0f, 0.0f, -depth));
		e1.m_rightUp = VAdd(e1.m_leftUp, VGet(height*1.4f, 0.0f, 0.0f));
		e1.m_rightBottom = VAdd(e1.m_leftBottom, VGet(height * 1.4f, 0.0f, 0.0f));//
	}
	else
	{
		e1.m_leftBottom = VAdd(pos, VGet(-(height * 0.7f), 0.0f, width * 0.8f));
		e1.m_leftUp = VAdd(pos, VGet(0.0f, 0.0f, width * 0.8 + depth));
		// e1.m_leftUp = VAdd(pos, VGet((height * 0.5f), 0.0f, width * 0.8+ depth));
		
		//e1.m_rightUp = VAdd(e1.m_leftUp, VGet(-height * 1.3, 0.0f, 0.0f));
	//	e1.m_rightBottom = VAdd(e1.m_rightUp, VGet(-depth, 0.0f, 0.0f));
	}
}

bool Collision::IsCollision(int num)
{

	if (num == 0)
	{
		if (e1.m_leftBottom.x <= p1.m_rightUp.x)
		{
			return true;
		}

			
		// DrawFormatString(0, 330, color, "e1.m_rightBottom.x=%.0f", e1.m_rightUp.x);
	}
	if (num == 1)
	{

	}
	if (num == 2)
	{

	}
	if (num == 3)
	{

	}

	return false;
}
