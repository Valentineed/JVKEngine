#pragma once
#include "LightComponent.h"

class PointLight : public LightComponent
{
public:
	PointLight(const Vector3& position, const Vector3& ambientColor, const Vector3& diffuseColor, const Vector3& specularColor, float constAttenuation,
		float linAttenuation, float quadAttenuation);
	
	[[nodiscard]] float GetConstantAttenuation() const;
	void SetConstantAttenuation(float constAttenuation);

	[[nodiscard]] float GetLinearAttenuation() const;
	void SetLinearAttenuation(float linAttenuation);

	[[nodiscard]] float GetQuadraticAttenuation() const;
	void SetQuadraticAttenuation(float quadAttenuation);

				/*Light Settings*/
	/*--------------------------------------------*/
	//Attenuation
	float m_kConstant = 0;
	float m_kLinear = 0;
	float m_kQuadratic = 0;

	//PointLight
	bool m_bIsPoint = true;
	/*--------------------------------------------*/
private:
	void NotifySystemChangeAtt();
};
