#include "LightComponent.h"

#include "../../ECS/World.h"
#include "../../Systems/LightSystem.h"

const Vector4 LightComponent::GetPosition() const
{
	return m_lightPos;
}

void LightComponent::SetPosition(const Vector4& pos)
{
	m_lightPos = pos;
	NotifySystem();
}

const Vector3 LightComponent::GetAmbientColor() const
{
	return  m_ambientColor;
}

void LightComponent::SetAmbientColor(const Vector3& ambientColor)
{
	m_ambientColor = ambientColor;
	NotifySystem();
}

const Vector3 LightComponent::GetDiffuseColor() const
{
	return  m_diffuseColor;
}

void LightComponent::SetDiffuseColor(const Vector3& diffuseColor)
{
	m_diffuseColor = diffuseColor;
	NotifySystem();
	
}

const Vector3 LightComponent::GetSpecularColor() const
{
	return m_specularColor;
	
}

void LightComponent::SetSpecularColr(const Vector3& specularColor)
{
	m_specularColor = specularColor;
	NotifySystem();
	
}

void LightComponent::NotifySystem()
{
	if (!bModify)
	{
		bModify = true;
		World::GetInstance().GetSystem<LightSystem>()->m_commonModifyIndex.push_back(indexSystem);
	}
}
