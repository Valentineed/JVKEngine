#include "Transform.h"


#include "../ECS/World.h"
#include "../Systems/TransformSystem.h"

void Transform::SetLocalPosition(const Vector3& pos)
{
	m_localTransform.raw[12] = pos.x;
	m_localTransform.raw[13] = pos.y;
	m_localTransform.raw[14] = pos.z;

	if (!bModify)
	{
		bModify = true;
		NotifySystem();
	}
}

void Transform::SetLocalPosition(float x, float y, float z)
{
	m_localTransform.raw[12] = x;
	m_localTransform.raw[13] = y;
	m_localTransform.raw[14] = z;
	if (!bModify)
	{
		bModify = true;
		NotifySystem();
	}
}

void Transform::Translate(const Vector3& pos)
{
	m_localTransform.Translate(pos);
	if (!bModify)
	{
		bModify = true;
		NotifySystem();
	}
}

void Transform::Translate(float x, float y, float z)
{
	m_localTransform.Translate(x,y,z);
	if (!bModify)
	{
		bModify = true;
		NotifySystem();
	}
}

void Transform::SetScale(const Vector3& scale)
{
	//m_localTransform.raw[0] = scale.x;
	//m_localTransform.raw[5] = scale.y;
	//m_localTransform.raw[10] = scale.z;
	//m_localTransform.raw[15] = 1.f;
	auto mat = Matrix4::Identity();
	mat.Scale(scale);
	m_localTransform *= mat;
	if (!bModify)
	{
		bModify = true;
		NotifySystem();
	}
	
}

void Transform::SetScale(float x, float y, float z)
{
	m_localTransform.raw[0] = x;
	m_localTransform.raw[5] = y;
	m_localTransform.raw[10] = z;
	m_localTransform.raw[15] = 1.f;
	if (!bModify)
	{
		bModify = true;
		NotifySystem();
	}
}

void Transform::Scale(const Vector3& scale)
{
	m_localTransform.Scale(scale);
	if (!bModify)
	{
		bModify = true;
		NotifySystem();
	}
}
void Transform::Scale(float x, float y, float z)
{
	m_localTransform.Scale(x,y,z);
	if (!bModify)
	{
		bModify = true;
		NotifySystem();
	}
}

void Transform::SetRotation(const Matrix4& rot)
{
	m_localTransform.raw[0] = rot.raw[0];
	m_localTransform.raw[1] = rot.raw[1];
	m_localTransform.raw[2] = rot.raw[2];
	m_localTransform.raw[4] = rot.raw[4];
	m_localTransform.raw[5] = rot.raw[5];
	m_localTransform.raw[6] = rot.raw[6];
	m_localTransform.raw[8] = rot.raw[8];
	m_localTransform.raw[9] = rot.raw[9];
	m_localTransform.raw[10] = rot.raw[10];
	m_localTransform.raw[15] = 1.f;
	if (!bModify)
	{
		bModify = true;
		NotifySystem();
	}
}

void Transform::SetLocalRotation(const Vector3& rotation, bool bisRadian)
{
	if (bisRadian)
	{
		auto temp = Matrix4::Rotation(Radian(rotation.x), Radian(rotation.y), Radian(rotation.z));
		SetRotation(temp);
	}
	else
	{
		auto temp = Matrix4::Rotation(Degree(rotation.x), Degree(rotation.y), Degree(rotation.z));
		SetRotation(temp);
	}

	if (!bModify)
	{
		bModify = true;
		NotifySystem();
	}
}

void Transform::SetLocalRotation(const Quaternion& quaternion)
{
	auto temp = Matrix4::Rotation(quaternion);
	SetRotation(temp);
}

void Transform::Rotate(const Vector3& rotation, bool bisRadian)
{
	m_localTransform.Rotate(rotation, bisRadian);
	if (!bModify)
	{
		bModify = true;
		NotifySystem();
	}
}

void Transform::Rotate(const Quaternion& quaternion)
{
	m_localTransform.Rotate(quaternion);
	if (!bModify)
	{
		bModify = true;
		NotifySystem();
	}
}

Vector3 Transform::GetWorldLocation()
{
	return { m_worldTransform.raw[12],
	m_worldTransform.raw[13],
	m_worldTransform.raw[14] };
}

Vector3 Transform::GetLocalLocation()
{
	return { m_localTransform.raw[12],
	   m_localTransform.raw[13],
	   m_localTransform.raw[14] };
}

Vector3 Transform::GetLocalScale()
{
	Vector3 col1(m_localTransform.raw[0], m_localTransform.raw[4], m_localTransform.raw[8]);
	Vector3 col2(m_localTransform.raw[1], m_localTransform.raw[5], m_localTransform.raw[9]);
	Vector3 col3(m_localTransform.raw[2], m_localTransform.raw[6], m_localTransform.raw[10]);

	float scaleX = col1.Magnitude();
	float scaleY = col2.Magnitude();
	float scaleZ = col3.Magnitude();

	return Vector3(scaleX, scaleY, scaleZ);
}

Vector3 Transform::GetWorldScale()
{
	Vector3 col1(m_worldTransform.raw[0], m_worldTransform.raw[4], m_worldTransform.raw[8]);
	Vector3 col2(m_worldTransform.raw[1], m_worldTransform.raw[5], m_worldTransform.raw[9]);
	Vector3 col3(m_worldTransform.raw[2], m_worldTransform.raw[6], m_worldTransform.raw[10]);

	float scaleX = col1.Magnitude();
	float scaleY = col2.Magnitude();
	float scaleZ = col3.Magnitude();

	return Vector3(scaleX, scaleY, scaleZ);
}

Vector3& Transform::GetFront()
{
	return m_front;
}

Vector3& Transform::GetRight()
{
	return m_right;
}

Vector3& Transform::GetUp()
{
	return m_up;
}

Quaternion Transform::GetLocalRotation()
{
	Quaternion TempQuat(m_localTransform);

	return TempQuat;
}

void Transform::SetLocalTransformMat(Matrix4& mat)
{
	m_localTransform = mat;
	if (!bModify)
	{
		bModify = true;
		NotifySystem();
	}
}

void Transform::UpdateVectors()
{
	Quaternion rot = GetWorldRotation();
	m_front = rot * m_front;
	m_right = rot * m_right;
	m_up = rot * m_up;
}

Quaternion Transform::GetWorldRotation()
{
	Quaternion TempQuat(m_worldTransform);

	return TempQuat;
}

void Transform::NotifySystem()
{
	World::GetInstance().GetSystem<TransformSystem>()->modifyTindex.push_back(myIndexSystem);
}

