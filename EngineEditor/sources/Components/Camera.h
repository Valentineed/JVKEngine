#pragma once
#include "Matrix/Matrix4.h"
using namespace  LibMath;
enum class MovCamera
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

struct Camera
{
	using vec3 = Vector3;
	using mat4 = Matrix4;
	Camera(vec3 position = vec3{ 0.f, 0.f, 0.f }, vec3 front = vec3{ 0.f, 0.f, -1.f }, vec3 up = vec3{ 0.f, 1.f, 0.f });
	Camera(LibMath::Radian fov,
		float aspecRatio,
		float near,
		float far);

	vec3& GetCameraPos() { return m_cameraPos; }
	mat4 GetViewMatrix();
	mat4 GetPerspectiveMatrix();
	mat4 GetOrthographicMatrix();

	//Euler Angles
	float m_yaw = -90.f;
	float m_pitch = 0.f;
	float m_movementSpeed = 2.6f;
	float m_sensitivity = 0.02f;

	float m_fov = 65.f;
	float m_width = 1920.f;
	float m_height =1027.f;
	float m_near = 0.1f;
	float m_far = 100.f;

	void MoveCamera(MovCamera direction, float deltaTime);
	void MouseMoveCamera(float xOffset, float yOffset, bool contrainHeight = true);
	void SetIsLookingAt(bool isLookingAt);
	void SetTarget(vec3 target);
	void SetCameraPos(vec3 pos);

	vec3 m_cameraFront;
	vec3 m_cameraUp;
	vec3 m_cameraRight;
	vec3 m_OffsetFromPlayer = {0.f, 0.f, 0.f};
	bool m_bFreeIndex = false;

	vec3 m_worldUp;
private:
	vec3 m_cameraPos;
	vec3 m_rotation;

	
	
	void UpdateVectors();

	bool m_isLookingAt = false;
	float m_radius = 8.5f;

	vec3 m_targetPos{ 0.0f, 0.0f, 0.0f };


};
