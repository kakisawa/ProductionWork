#include "Camera.h"
#include <cmath>

namespace {

	// ÉJÉÅÉâèÓïÒ
	constexpr float kCameraTitleDist = 20;
	constexpr float kCameraTitleHeight = 20.0f;

	constexpr float kCameraDist = 5;
	constexpr float kCameraHeight = 10;

	constexpr float kCameraNear = 0.1f;
	constexpr float kCameraFar = 100.0f;


	// ÉJÉÅÉâèÓïÒ
	float cameraAngle = -DX_PI_F / 2;
	float cameraAddAngle = cameraAngle;
}

Camera::Camera()
{
	SetCameraNearFar(kCameraNear, kCameraFar);
}

void Camera::Init_Title(VECTOR pos)
{
	m_playerPos = pos;

	/*cameraPos.x = cosf(cameraAngle) * kCameraDist;
	cameraPos.y = kCameraHeight;
	cameraPos.z = sinf(cameraAngle) * kCameraDist;
	SetCameraPositionAndTarget_UpVecY(cameraPos, VGet(0, 1.2f, 0));*/
}

void Camera::Init_Game()
{
	// ÉJÉÅÉâ
	cameraPos.x = cosf(cameraAngle) * kCameraDist;
	cameraPos.y = kCameraHeight;
	cameraPos.z = sinf(cameraAngle) * kCameraDist;
	cameraPos = VAdd(cameraPos, VGet(0, 50.0f, 0));

	SetCameraPositionAndTarget_UpVecY(cameraPos, VGet(0.0f, 2.0f, 0.0f));

	ChangeLightTypeDir(VGet(0.0f, -10.0f, 0.0f));
}

void Camera::Update()
{
	cameraAddAngle += m_cameraAngle;

	cameraPos.x = cosf(cameraAddAngle) * kCameraTitleDist;
	cameraPos.y = kCameraTitleHeight;
	cameraPos.z = sinf(cameraAddAngle) * kCameraTitleDist;
	SetCameraPositionAndTarget_UpVecY(cameraPos, m_playerPos);
}
