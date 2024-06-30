#include "Camera.h"
#include <cmath>

namespace {

	// ƒJƒƒ‰î•ñ
	constexpr float kCameraDist = 3.5f;
	constexpr float kCameraHeight = 10;

	constexpr float kCameraNear = 0.5f;
	constexpr float kCameraFar = 180.0f;


	// ƒJƒƒ‰î•ñ
	float cameraAngle = -DX_PI_F / 2;
}

Camera::Camera()
{
	// ƒJƒƒ‰
	SetCameraNearFar(kCameraNear, kCameraFar);
	VECTOR cameraPos;
	cameraPos.x = cosf(cameraAngle) * kCameraDist;
	cameraPos.y = kCameraHeight;
	cameraPos.z = sinf(cameraAngle) * kCameraDist;
	cameraPos = VAdd(cameraPos, VGet(0, 50.0f, 0));

	SetCameraPositionAndTarget_UpVecY(cameraPos, VGet(0.0f, 2.0f, 0.0f));

	ChangeLightTypeDir(VGet(0.0f, -10.0f, 0.0f));
}

void Camera::Update()
{
}
