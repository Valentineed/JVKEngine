#pragma once
#include "LightComponent.h"

class SpotLight :public LightComponent
{
public:
	SpotLight(const Vector3& position, const Vector3& direction, float cutOff, float outerCutOff, const Vector3& ambientColor, const Vector3& diffuseColor, const Vector3& specularColor, float constAttenuation,
		float linAttenuation, float quadAttenuation);

	[[nodiscard]] const Vector3 GetDirection() const;
	void SetDirection(const Vector3& direction);
	
	[[nodiscard]] float GetCutOff() const;
	void SetCutOff(float cutOff);

	[[nodiscard]] float GetOuterCutOff() const;
	void SetOuterCutOff(float outerCutOff);

	[[nodiscard]] float GetConstantAttenuation() const;
	void SetConstantAttenuation(float constAttenuation);

	[[nodiscard]] float GetLinearAttenuation() const;
	void SetLinearAttenuation(float linAttenuation);

	[[nodiscard]] float GetQuadraticAttenuation() const;
	void SetQuadraticAttenuation(float quadAttenuation);

					/*Light Settings*/
	/*--------------------------------------------*/
	
	//SpotLight Direction
	Vector3 m_lightDirection;
	
	//SpotLight Cones
	float m_cutOff = 0;
	float m_outerCutOff = 0;

	//Attenuation
	float m_kConstant = 0;
	float m_kLinear = 0;
	float m_kQuadratic = 0;

private:
	void NotifySystemChangeAtt();
	void NotifySystemChangeSpotSettings();
};
