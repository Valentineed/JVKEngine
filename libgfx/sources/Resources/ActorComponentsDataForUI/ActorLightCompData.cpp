#include "ActorLightCompData.h"



libgfx_API ImVec4& ActorLightCompData::GetAmbiantColorLight()
{
	return m_ambiantColorLight;
}

libgfx_API ImVec4& ActorLightCompData::GetDiffuseColorLight()
{
	return m_diffuseColorLight;
}

libgfx_API ImVec4& ActorLightCompData::GetSpecularColorLight()
{
	return m_specularColorLight;
}

libgfx_API LibMath::Vector3& ActorLightCompData::GetLightDirection()
{
	return m_lightDirection;
}

libgfx_API LibMath::Vector4& ActorLightCompData::GetLightPosition()
{
	return m_lightPosition;
}

libgfx_API float& ActorLightCompData::GetLightCutOff()
{
	return m_lightCutOff;
}

libgfx_API float& ActorLightCompData::GetLightOuterCutOff()
{
	return m_lightOuterCutOff;
}

libgfx_API float& ActorLightCompData::GetConstantAttenuation()
{
	return m_lightConstAttenuation;
}

libgfx_API float& ActorLightCompData::GetLinearAttenuation()
{
	return m_lightLinAttenuation;
}

libgfx_API float& ActorLightCompData::GetQuadraticAttenuation()
{
	return m_lightQuadAttenuation;
}

libgfx_API std::string& ActorLightCompData::GetLightType()
{
	return m_lightType;
}

void libgfx_API ActorLightCompData::SetAmbiantColorLight(ImVec4 ambiantColor)
{
	m_ambiantColorLight = ambiantColor;
}

void libgfx_API ActorLightCompData::SetDiffuseColorLight(ImVec4 diffuseColor)
{
	m_diffuseColorLight = diffuseColor;
}

void libgfx_API ActorLightCompData::SetSpecularColorLight(ImVec4 specularColor)
{
	m_specularColorLight = specularColor;
}

void libgfx_API ActorLightCompData::SetLightDirection(LibMath::Vector3 direction)
{
	m_lightDirection = direction;
}

void libgfx_API ActorLightCompData::SetLightPosition(LibMath::Vector4 position)
{
	m_lightPosition = position;
}

void libgfx_API ActorLightCompData::SetLightCutOff(float cutOff)
{
	m_lightCutOff = cutOff;
}

void libgfx_API ActorLightCompData::SetLightOuterCutOff(float outerCutOff)
{
	m_lightOuterCutOff = outerCutOff;
}

void libgfx_API ActorLightCompData::SetConstantAttenuation(float constAtt)
{
	m_lightConstAttenuation = constAtt;
}

void libgfx_API ActorLightCompData::SetLinearAttenuation(float linAtt)
{
	m_lightLinAttenuation = linAtt;
}

void libgfx_API ActorLightCompData::SetQuadraticAttenuation(float quadAtt)
{
	m_lightQuadAttenuation = quadAtt;
}

void libgfx_API ActorLightCompData::SetLightType(std::string lightType)
{
	m_lightType = lightType;
}

void libgfx_API ActorLightCompData::SetDirectionalLightData(LibMath::Vector4 position)
{
	m_lightType = "Directional";

	m_lightPosition = position;
}

void libgfx_API ActorLightCompData::SetLightColors(LibMath::Vector3 ambiantColor, LibMath::Vector3 diffuseColor, LibMath::Vector3 specularColor)
{
	m_ambiantColorLight = ImVec4(ambiantColor.x, ambiantColor.y, ambiantColor.z, 1.f);
	m_diffuseColorLight = ImVec4(diffuseColor.x, diffuseColor.y, diffuseColor.z, 1.f);
	m_specularColorLight = ImVec4(specularColor.x, specularColor.y, specularColor.z, 1.f);
}

void libgfx_API ActorLightCompData::SetSpotLightData(LibMath::Vector4 position,
													 LibMath::Vector3 direction,
													 float cutOff,
													 float outerCutOff,
													 float constAtt,
													 float linAtt, 
													 float quadAtt)
{
	m_lightType = "Spotlight";

	m_lightDirection = direction;
	m_lightCutOff = cutOff;
	m_lightOuterCutOff = outerCutOff;
	m_lightConstAttenuation = constAtt;
	m_lightLinAttenuation = linAtt;
	m_lightQuadAttenuation = quadAtt;

	m_lightPosition = position;
}

void libgfx_API ActorLightCompData::SetPointLightData(LibMath::Vector4 position, float constAtt, float linAtt, float quadAtt)
{

	m_lightType = "Pointlight";

	
	m_lightConstAttenuation = constAtt;
	m_lightLinAttenuation = linAtt;
	m_lightQuadAttenuation = quadAtt;
	  
	m_lightPosition = position;
}
