#pragma once
#include "Matrix/Matrix4.h"
#include <vector>

using namespace  LibMath;
struct Transform
{
	Transform() = default;
	bool bModify = false;
	size_t indexParent = s_invalidIndex;
	size_t myIndexSystem = s_invalidIndex;
	size_t indexInParentVec = s_invalidIndex;
	std::vector<size_t> indexChildren;

	//TODO add more utility function
	//--------------------------------------
	
	void SetLocalPosition(const Vector3& pos);
	
	void SetLocalPosition(float x, float y, float z);

	//void SetWorldPosition(Vector3 pos)
	//void SetWorldPosition(float x, float y, float z)
	
	void Translate(const Vector3& pos);
	
	void Translate(float x, float y, float z);

	//-------------------------------------------
	
	void SetScale(const Vector3& scale);
	
	void SetScale(float x, float y, float z);

	void Scale(const Vector3& scale);
	
	void Scale(float x, float y, float z);

	//-------------------------------------------

	void SetLocalRotation(const Vector3& rotation, bool bisRadian = false);
	
	void SetLocalRotation(const Quaternion& quaternion);

	//void SetWorldRotation(Vector3 pos);
	//void SetWorldRotation(float x, float y, float z)
	
	void Rotate(const Vector3& rotation, bool bisRadian = false);

	void Rotate(const Quaternion& quaternion);

	//------------------------------------------

	Vector3 GetWorldLocation();

	Vector3 GetLocalLocation();

	Vector3 GetLocalScale();

	Vector3 GetWorldScale();

	Vector3& GetFront();
	Vector3& GetRight();
	Vector3& GetUp();
	
	Quaternion GetLocalRotation();

	Quaternion GetWorldRotation();

	void SetLocalTransformMat(Matrix4& mat);

	void UpdateVectors();
	
	Matrix4 m_worldTransform = Matrix4::Identity();
	Matrix4 m_localTransform = Matrix4::Identity();
private:
	void NotifySystem();
	void SetRotation(const Matrix4& rot);
	Vector3 m_right = {1.f,0.f,0.f};
	Vector3 m_up = {0.f,1.f,0.f};
	Vector3 m_front = { 0.f,0.f,-1.f };
protected:
	static  const  size_t s_invalidIndex = SIZE_MAX;
};
