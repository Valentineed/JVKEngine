#pragma once
#include <string>


#include "imgui.h"
#include "Vector/Vector4.h"
#include "Vector/Vector3.h"

#include <libgfx_Export.h>

class ActorLightCompData
{
public:
	ActorLightCompData() = default;

	//Getters

	libgfx_API ImVec4& GetAmbiantColorLight();
	libgfx_API ImVec4& GetDiffuseColorLight();
	libgfx_API ImVec4& GetSpecularColorLight();

	libgfx_API LibMath::Vector3& GetLightDirection();
	libgfx_API LibMath::Vector4& GetLightPosition();
	
	libgfx_API float& GetLightCutOff();
	libgfx_API float& GetLightOuterCutOff();
	libgfx_API float& GetConstantAttenuation();
	libgfx_API float& GetLinearAttenuation();
	libgfx_API float& GetQuadraticAttenuation();
	
	libgfx_API std::string& GetLightType();

	//Setters

	void libgfx_API SetAmbiantColorLight(ImVec4 ambiantColor);
	void libgfx_API SetDiffuseColorLight(ImVec4 diffuseColor);
	void libgfx_API SetSpecularColorLight(ImVec4 specularColor);
	
	void libgfx_API SetLightDirection(LibMath::Vector3 direction);
	void libgfx_API SetLightPosition(LibMath::Vector4 position);
	
	void libgfx_API SetLightCutOff(float cutOff);
	void libgfx_API SetLightOuterCutOff(float outerCutOff);
	void libgfx_API SetConstantAttenuation(float constAtt);
	void libgfx_API SetLinearAttenuation(float linAtt);
	void libgfx_API SetQuadraticAttenuation(float quadAtt);
	
	void libgfx_API SetLightType(std::string lightType);

	//Others
	void libgfx_API SetDirectionalLightData(LibMath::Vector4 position);
	
	void libgfx_API SetSpotLightData(LibMath::Vector4 position,
									 LibMath::Vector3 direction,
									 float cutOff,
									 float outerCutOff,
									 float constAtt,
									 float linAtt,
									 float quadAtt);

	void libgfx_API SetPointLightData(LibMath::Vector4 position, float constAtt, float linAtt, float quadAtt);
	
	void libgfx_API SetLightColors(LibMath::Vector3 ambiantColor, LibMath::Vector3 diffuseColor, LibMath::Vector3 specularColor);

	
	

	
private:
	ImVec4 m_ambiantColorLight;
	ImVec4 m_diffuseColorLight;
	ImVec4 m_specularColorLight;

	LibMath::Vector3 m_lightDirection;
	LibMath::Vector4 m_lightPosition;
	
	float m_lightCutOff;
	float m_lightOuterCutOff;
	float m_lightConstAttenuation;
	float m_lightLinAttenuation;
	float m_lightQuadAttenuation;

	std::string m_lightType;
};

