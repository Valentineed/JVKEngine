#include "PointLight.h"


#include "../../ECS/World.h"
#include "../../Systems/LightSystem.h"


PointLight::PointLight(const Vector3& position, const Vector3& ambientColor, const Vector3& diffuseColor, const Vector3& specularColor, float constAttenuation, float linAttenuation, float quadAttenuation)
{
	m_lightPos = { position.x, position.y, position.z, 1.0f };
	m_ambientColor = ambientColor;
	m_diffuseColor = diffuseColor;
	m_specularColor = specularColor;
	m_kConstant = constAttenuation;
	m_kLinear = linAttenuation;
	m_kQuadratic = quadAttenuation;
	
	type = Type::POINT;
}

float PointLight::GetConstantAttenuation() const
{
	return m_kConstant;
}

void PointLight::SetConstantAttenuation(float constAttenuation)
{
	m_kConstant = constAttenuation;
	NotifySystemChangeAtt();
}

float PointLight::GetLinearAttenuation() const
{
	return m_kLinear;
}

void PointLight::SetLinearAttenuation(float linAttenuation)
{
	m_kLinear = linAttenuation;
	NotifySystemChangeAtt();
}

float PointLight::GetQuadraticAttenuation() const
{
	return  m_kQuadratic;
}

void PointLight::SetQuadraticAttenuation(float quadAttenuation)
{
	m_kQuadratic = quadAttenuation;
	NotifySystemChangeAtt();
}

void PointLight::NotifySystemChangeAtt()
{
	if (!bModify)
	{
		bModify = true;
		World::GetInstance().GetSystem<LightSystem>()->m_attenuationModifIndex.push_back(indexSystem);
	}
}