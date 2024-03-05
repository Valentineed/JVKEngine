#pragma once
#include "Vector/Vector3.h"
#include "Vector/Vector4.h"

enum class Type
{
	DIRECTIONAL,
	SPOT,
	POINT
};
using namespace LibMath;
struct LightComponent
{
	[[nodiscard]] const Vector4 GetPosition() const;
	void SetPosition(const Vector4& pos);
	
	[[nodiscard]] const Vector3 GetAmbientColor() const;
	void SetAmbientColor(const Vector3& ambientColor);

	[[nodiscard]] const Vector3 GetDiffuseColor() const;
	void SetDiffuseColor(const Vector3& diffuse);

	[[nodiscard]] const Vector3 GetSpecularColor() const;
	void SetSpecularColr(const Vector3& specularColor) ;
	
	//LightSystem set the index == LightManager index
	size_t indexSystem;
	size_t indexManager;
	bool bModify = false;

	//The children set the type of LightComponent
	Type type;
protected:
	void NotifySystem();
	


	/*Light Common Settings*/
	/*--------------------------------------------*/
	//Position
	Vector4 m_lightPos;
	
	//Color Settings
	Vector3 m_ambientColor;
	Vector3 m_diffuseColor;
	Vector3 m_specularColor;
	/*--------------------------------------------*/
};
