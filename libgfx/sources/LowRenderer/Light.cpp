#include "Light.h"
#include "Descriptors.h"
#include "GraphicAPI.h"
#include "LightManager.h"

namespace LowRenderer
{
	Light::Light(const Vector4& position, const Vector3& ambientColor, const Vector3& diffuseColor, const Vector3& specularColor):
	m_lightPos(position), m_ambientColor(ambientColor), m_diffuseColor(diffuseColor), m_specularColor(specularColor)
	{
		off = Vector4(0, 0, 0, 0);
	}

	Light::Light(const Vector4& position, const Vector3& ambientColor, const Vector3& diffuseColor, const Vector3& specularColor,
		float constAttenuation, float linAttenuation, float quadAttenuation, bool isPoint) :
		m_lightPos(position), m_ambientColor(ambientColor), m_diffuseColor(diffuseColor), m_specularColor(specularColor),
		m_kConstant(constAttenuation), m_kLinear(linAttenuation), m_kQuadratic(quadAttenuation), m_bIsPoint(isPoint)
	{
		off = Vector4(0, 0, 0, 0);
	}

	Light::Light(const Vector4& position, const Vector3& direction, float cutOff, float outerCutOff, const Vector3& ambientColor,
		const Vector3& diffuseColor, const Vector3& specularColor, float constAttenuation, float linAttenuation,
		float quadAttenuation) :
		m_lightPos(position), m_lightDirection(direction), m_cutOff(cutOff), m_outerCutOff(outerCutOff), m_ambientColor(ambientColor),
		m_diffuseColor(diffuseColor), m_specularColor(specularColor), m_kConstant(constAttenuation),
		m_kLinear(linAttenuation), m_kQuadratic(quadAttenuation)
	{
		off = Vector4(0, 0, 0, 0);
	}

	void Light::UploadBuffer(uint32_t currImg) const
	{
		GraphicAPI* api = GraphicAPI::GetInstance();

		
		GraphicAPI::LightBufferObject lbo{};
		lbo.position = m_lightPos;
		lbo.direction = m_lightDirection;

		lbo.ambientColor = m_ambientColor;
		lbo.specularColor = m_specularColor;
		lbo.diffuseColor = m_diffuseColor;

		lbo.kConstant = m_kConstant;
		lbo.kLinear = m_kLinear;
		lbo.kQuadratic = m_kQuadratic;

		lbo.cutOff = m_cutOff;
		lbo.outerCutOff = m_outerCutOff;

		lbo.bIsPoint = m_bIsPoint;
		lbo.off = off;

		//Send to Shader
		void* data = nullptr;
		LightManager* lightManager = &LightManager::GetInstance();
		auto offset = LightManager::s_alignedSize * (LightManager::m_maxLight * currImg + m_indexBuffer);
		vkMapMemory(api->GetDevice(), lightManager->m_uniformBuffer.bufferMemory, offset, LightManager::s_alignedSize, 0, &data);
		auto sizeOff = sizeof(lbo);
		memcpy(data, &lbo, sizeOff);
		vkUnmapMemory(api->GetDevice(), lightManager->m_uniformBuffer.bufferMemory);
	}
}
