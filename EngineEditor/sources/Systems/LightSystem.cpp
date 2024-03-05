#include "LightSystem.h"

#include "LightManager.h"
#include "../Components/Lights/DirectionalLight.h"
#include "../Components/Lights/LightComponent.h"
#include "../Components/Lights/PointLight.h"
#include "../Components/Lights/SpotLight.h"
#include "../ECS/World.h"

void LightSystem::Update()
{
	if (!m_commonModifyIndex.empty())
	{
		for (size_t index : m_commonModifyIndex)
		{
			if (index <= m_maxLight + 1)
			{
				auto& lightInManager = *m_lightManager->GetLight(m_lights[index]->indexManager);
				lightInManager.m_ambientColor = m_lights[index]->GetAmbientColor();
				lightInManager.m_lightPos = m_lights[index]->GetPosition();
				lightInManager.m_diffuseColor = m_lights[index]->GetDiffuseColor();
				lightInManager.m_specularColor = m_lights[index]->GetSpecularColor();
				m_lights[index]->bModify = false;
			}
			
		}
		m_commonModifyIndex.clear();
	}

	if(!m_attenuationModifIndex.empty())
	{
		for (size_t index : m_attenuationModifIndex)
		{
			auto lightInManager = m_lightManager->GetLight(m_lights[index]->indexManager);
			if(m_lights[index]->type == Type::POINT)
			{
				auto pointLight = GetLight<PointLight>(index);
				lightInManager->m_kConstant = pointLight->m_kConstant;
				lightInManager->m_kLinear = pointLight->m_kLinear;
				lightInManager->m_kQuadratic = pointLight->m_kQuadratic;
				m_lights[index]->bModify = false;
			}
			else if(m_lights[index]->type == Type::SPOT)
			{
				auto spotLight = GetLight<SpotLight>(index);
				lightInManager->m_kConstant = spotLight->m_kConstant;
				lightInManager->m_kLinear = spotLight->m_kLinear;
				lightInManager->m_kQuadratic = spotLight->m_kQuadratic;
				m_lights[index]->bModify = false;
			}
		}
		m_attenuationModifIndex.clear();
	}

	if(!m_spotSettingsModifIndex.empty())
	{
		for (size_t index : m_spotSettingsModifIndex)
		{
			auto& lightInManager = *m_lightManager->GetLight(m_lights[index]->indexManager);
			auto spotLight = GetLight<SpotLight>(index);
			lightInManager.m_lightDirection = spotLight->m_lightDirection;
			lightInManager.m_cutOff = std::cosf(spotLight->m_cutOff);
			lightInManager.m_outerCutOff = std::cosf(spotLight->m_outerCutOff);
			m_lights[index]->bModify = false;
		}
		m_spotSettingsModifIndex.clear();
	}
}

size_t LightSystem::Register(PointLight& light)
{
	light.indexManager = m_lightManager->PointLight(light.GetPosition(), light.GetAmbientColor(), light.GetDiffuseColor(),
		light.GetSpecularColor(), light.GetConstantAttenuation(), light.GetLinearAttenuation(),
		light.GetQuadraticAttenuation());

	if(m_freeIndex.empty())
	{
		const size_t index = m_lights.size();
		m_lights.push_back(std::make_unique<PointLight>(light));
		m_lights.back()->indexSystem = index;
		return index;
	}
	const size_t index = m_freeIndex.top();
	m_freeIndex.pop();
	m_lights[index] = std::make_unique<PointLight>(light);
	m_lights[index]->indexSystem = index;
	return index;
}

size_t LightSystem::Register(DirectionalLight& light)
{
	light.indexManager = m_lightManager->DirectionalLight(light.GetPosition(), light.GetAmbientColor(),
		light.GetDiffuseColor(), light.GetSpecularColor());
	
	if (m_freeIndex.empty())
	{
		const size_t index = m_lights.size();
		m_lights.push_back(std::make_unique<DirectionalLight>(light));
		m_lights.back()->indexSystem = index;
		return index;
	}
	const size_t index = m_freeIndex.top();
	m_freeIndex.pop();
	m_lights[index] = std::make_unique<DirectionalLight>(light);
	m_lights[index]->indexSystem = index;
	return index;
}

size_t LightSystem::Register(SpotLight& light)
{
	light.indexManager = m_lightManager->SpotLight(light.GetPosition(), light.GetDirection(), light.GetCutOff(),
		light.GetOuterCutOff(), light.GetAmbientColor(), light.GetDiffuseColor(), light.GetSpecularColor(),
		light.GetConstantAttenuation(), light.GetLinearAttenuation(), light.GetQuadraticAttenuation());

	if (m_freeIndex.empty())
	{
		const size_t index = m_lights.size();
		m_lights.push_back(std::make_unique<SpotLight>(light));
		m_lights.back()->indexSystem = index;
		return index;
	}
	const size_t index = m_freeIndex.top();
	m_freeIndex.pop();
	m_lights[index] = std::make_unique<SpotLight>(light);
	m_lights[index]->indexSystem = index;
	return index;
}

void LightSystem::RemoveLight(size_t index, int idEntity)
{
	if (m_commonModifyIndex.empty() == false)
	{
		if (m_commonModifyIndex.size() > 2)
		{
			auto iterator = std::find(m_commonModifyIndex.begin(), m_commonModifyIndex.end(), index);

			std::iter_swap(iterator, m_commonModifyIndex.end() - 1);

			m_commonModifyIndex.pop_back();
		}
		else
		{
			m_commonModifyIndex.pop_back();
		}
	}

	if (index == m_lights.size() - m_freeIndex.size() - 1)
	{
		m_lightManager->RemoveLight(m_lights[index]->indexManager);
		m_freeIndex.push(index);
	}
	else
	{
		m_lights.back()->indexManager = m_lights[index]->indexManager;
		m_lightManager->RemoveLight(m_lights[index]->indexManager);
		m_lights[index].reset();
		m_freeIndex.push(index);
	}
	if(idEntity != -1)
	{
		World::GetInstance().ClearComponentID<LightComponent>(idEntity);
	}
}

void LightSystem::Init()
{
	m_lightManager = &LightManager::GetInstance();
	m_maxLight = LightManager::m_maxLight;
}
