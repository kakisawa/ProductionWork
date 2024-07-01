#include "Camera.h"
#include <cmath>

namespace {

	// ƒJƒƒ‰î•ñ
	constexpr float kCameraDist = 3.5f;
	constexpr float kCameraHeight = 10;

	constexpr float kCameraNear = 0.1f;
	constexpr float kCameraFar = 100.0f;


	// ƒJƒƒ‰î•ñ
	float cameraAngle = -DX_PI_F / 2;
}

Camera::Camera()
{
	SetCameraNearFar(kCameraNear, kCameraFar);
}

void Camera::Init_Title(VECTOR pos)
{
	m_playerPos = pos;

	cameraPos=VAdd(cameraPos, VGet(0, 50.0f, -40));	

	SetCameraPositionAndTarget_UpVecY(cameraPos, m_playerPos);
	ChangeLightTypeDir(VGet(0.0f, -10.0f, 0.0f));
}

void Camera::Init_Game()
{
	// ƒJƒƒ‰
	cameraPos.x = cosf(cameraAngle) * kCameraDist;
	cameraPos.y = kCameraHeight;
	cameraPos.z = sinf(cameraAngle) * kCameraDist;
	cameraPos = VAdd(cameraPos, VGet(0, 50.0f, 0));

	SetCameraPositionAndTarget_UpVecY(cameraPos, VGet(0.0f, 2.0f, 0.0f));

	ChangeLightTypeDir(VGet(0.0f, -10.0f, 0.0f));
}

void Camera::Update()
{
	cameraPos = VAdd(cameraPos, cameraAddPos);

	SetCameraPositionAndTarget_UpVecY(cameraPos, m_playerPos);
}
