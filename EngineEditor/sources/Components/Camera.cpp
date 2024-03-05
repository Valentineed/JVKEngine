
#include "Camera.h"
#define _USE_MATH_DEFINES
#include <math.h>

Camera::Camera(vec3 position, vec3 front, vec3 worldUp)
{
	m_cameraPos = position;
	m_cameraFront = front;
	m_worldUp = worldUp;
	UpdateVectors();
}
void Camera::MoveCamera(MovCamera direction, float deltaTime)
{
	if (!m_isLookingAt)
	{
		//TODO
		/*vec3 front = vec3(m_cameraFront.x, 0, m_cameraFront.z).Normalize();
		vec3 right = vec3(m_cameraRight.x, 0, m_cameraRight.z).Normalize();*/

		float velocity = m_movementSpeed * deltaTime;

		switch (direction)
		{
		case MovCamera::FORWARD:
			m_cameraPos += m_cameraFront * velocity;
			break;
		case  MovCamera::BACKWARD:
			m_cameraPos -= m_cameraFront * velocity;
			break;
		case MovCamera::LEFT:
			m_cameraPos += m_cameraRight * velocity;
			break;
		case MovCamera::RIGHT:
			m_cameraPos -= m_cameraRight * velocity;
			break;
		case MovCamera::UP:
			m_cameraPos += m_cameraUp * velocity;
			break;
		case MovCamera::DOWN:
			m_cameraPos -= m_cameraUp * velocity;
			break;
		}
	}
	else
	{
		float velocity = m_movementSpeed * deltaTime;
		if (direction == MovCamera::FORWARD && m_rotation.x < 1.0f)
		{
			m_rotation.y += velocity;
		}
		if (direction == MovCamera::BACKWARD && m_rotation.y > 0.0f)
		{
			m_rotation.y -= velocity;
		}
		if (direction == MovCamera::LEFT)
		{
			m_rotation.x -= velocity;
			m_rotation.z -= velocity;
		}
		if (direction == MovCamera::RIGHT)
		{
			m_rotation.x += velocity;
			m_rotation.y += velocity;
		}
	}

	UpdateVectors();
}

void Camera::MouseMoveCamera(float xOffset, float yOffset, bool contrainHeight)
{
	if (m_isLookingAt)
	{
		return;
	}

	xOffset *= m_sensitivity;
	yOffset *= m_sensitivity;

	m_yaw += xOffset;
	m_pitch += yOffset;

	if (contrainHeight)
	{
		if (m_pitch > 89.0f)
		{
			m_pitch = 89.0f;
		}
		if (m_pitch < -89.0f)
		{
			m_pitch = -89.0f;
		}
	}

	UpdateVectors();
}

void Camera::SetIsLookingAt(bool isLookingAt)
{
	m_isLookingAt = isLookingAt;
	UpdateVectors();
}

void Camera::SetTarget(vec3 target)
{
	m_targetPos = target;
	UpdateVectors();
}

void Camera::SetCameraPos(vec3 pos)
{
	SetTarget(pos);
	//m_isLookingAt = true;
	m_cameraPos.x = pos.x + m_OffsetFromPlayer.x;
	m_cameraPos.y = pos.y + m_OffsetFromPlayer.y;
	m_cameraPos.z = pos.z + m_OffsetFromPlayer.z;
	UpdateVectors();
}

void Camera::UpdateVectors()
{
	if (!m_isLookingAt)
	{
		// Calculate the new Front vector
		float yaw, pitch;
		yaw = m_yaw * (float)(M_PI / 180);
		pitch = m_pitch * (float)(M_PI / 180);

		vec3 front{ 0.f, 0.f,0.f };
		front.x = cosf(yaw) * cosf(pitch);
		front.y = sinf(pitch);
		front.z = sinf(yaw) * cosf(pitch);

		//re - calculate the right and up
		m_cameraFront = front.Normalize();
		m_cameraRight = m_worldUp.Cross(front).Normalize();
		m_cameraUp = front.Cross(m_cameraRight).Normalize();
	}
	else
	{
		
		m_cameraFront = (m_targetPos - m_cameraPos).Normalize();
		m_cameraPos = m_cameraPos - (m_cameraFront * 0.5f);
		m_cameraRight = m_worldUp.Cross(m_cameraFront).Normalize();
		m_cameraUp = m_cameraFront.Cross(m_cameraRight).Normalize();
	}
}

Matrix4 Camera::GetViewMatrix()
{
	return Matrix4::LookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

Matrix4 Camera::GetPerspectiveMatrix()
{
	return Matrix4::Perspective(Degree(m_fov), (m_width / m_height), m_near, m_far);
}

Matrix4 Camera::GetOrthographicMatrix()
{
	m_fov *= (float)(M_PI / 180);
	float top = tanf(m_fov * 0.5f) * m_near;
	float bot = -top;
	float right = top * (m_width / m_height);
	float left = -right;
	return Matrix4::Orthographic(left, right, bot, top, m_near, m_far);
}