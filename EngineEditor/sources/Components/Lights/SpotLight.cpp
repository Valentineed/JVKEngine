#include "SpotLight.h"

#include "../../ECS/World.h"
#include "../../Systems/LightSystem.h"


SpotLight::SpotLight(const Vector3& position, const Vector3& direction, float cutOff, float outerCutOff, const Vector3& ambientColor, const Vector3& diffuseColor, const Vector3& specularColor, float constAttenuation, float linAttenuation, float quadAttenuation)
{
	m_lightPos = { position.x, position.y, position.z, 1.0f };
	m_lightDirection = direction;
	m_cutOff = cutOff;
	m_outerCutOff = outerCutOff;
	m_ambientColor = ambientColor;
	m_diffuseColor = diffuseColor;
	m_specularColor = specularColor;
	m_kConstant = constAttenuation;
	m_kLinear = linAttenuation;
	m_kQuadratic = quadAttenuation;
	
	type = Type::SPOT;
}

const Vector3 SpotLight::GetDirection() const
{
	return m_lightDirection;
}

void SpotLight::SetDirection(const Vector3& direction)
{
	m_lightDirection = direction;
	NotifySystemChangeSpotSettings();
}

float SpotLight::GetCutOff() const
{
	return m_cutOff;
}

void SpotLight::SetCutOff(float cutOff)
{
	m_cutOff = cutOff;
	NotifySystemChangeSpotSettings();
}

float SpotLight::GetOuterCutOff()const
{
	return m_outerCutOff;
}

void SpotLight::SetOuterCutOff(float outerCutOff) 
{
	m_outerCutOff = outerCutOff;
	NotifySystemChangeSpotSettings();
}

float SpotLight::GetConstantAttenuation() const
{
	return  m_kConstant;
}

void SpotLight::SetConstantAttenuation(float constAttenuation)
{
	m_kConstant = constAttenuation;
	NotifySystemChangeAtt();
}

float SpotLight::GetLinearAttenuation() const
{
	return m_kLinear;
}

void SpotLight::SetLinearAttenuation(float linAttenuation)
{
	m_kLinear = linAttenuation;
	NotifySystemChangeAtt();
}

float SpotLight::GetQuadraticAttenuation() const
{
	return m_kQuadratic;
}

void SpotLight::SetQuadraticAttenuation(float quadAttenuation)
{
	m_kQuadratic = quadAttenuation;
	NotifySystemChangeAtt();
}

void SpotLight::NotifySystemChangeAtt()
{
	if (!bModify)
	{
		bModify = true;
		World::GetInstance().GetSystem<LightSystem>()->m_attenuationModifIndex.push_back(indexSystem);
	}
}

void SpotLight::NotifySystemChangeSpotSettings()
{
	if (!bModify)
	{
		bModify = true;
		World::GetInstance().GetSystem<LightSystem>()->m_spotSettingsModifIndex.push_back(indexSystem);
	}
}
