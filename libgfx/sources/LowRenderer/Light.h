#pragma once

#include "Vector/Vector.h"
#include "Buffers.h"

using namespace LibMath;
struct DescriptorSetLayout;

namespace LowRenderer
{
	class Light
	{
	public:
		Light() = default;
		
		//Constructor directional Light
		Light(const Vector4& position, const Vector3& ambientColor, const Vector3& diffuseColor, const Vector3& specularColor);

		//Constructor Point Light
		Light(const Vector4& position, const Vector3& ambientColor, const Vector3& diffuseColor, const Vector3& specularColor, float constAttenuation,
			float linAttenuation, float quadAttenuation, bool isPoint = true);

		//Constructor Spot Light
		Light(const Vector4& position, const Vector3& direction, float cutOff, float outerCutOff, const Vector3& ambientColor, const Vector3& diffuseColor, const Vector3& specularColor, float constAttenuation,
			float linAttenuation, float quadAttenuation);

		
						/*Light Settings*/
		/*--------------------------------------------*/
		//Position
		Vector4 m_lightPos;

		//SpotLight Direction
		Vector3 m_lightDirection;
		
		//SpotLight Cones
		float m_cutOff = 0;
		float m_outerCutOff = 0;

		//Color Settings
		Vector3 m_ambientColor;
		Vector3 m_diffuseColor;
		Vector3 m_specularColor;
		
		//Attenuation for the point and spotLight
		float m_kConstant = 0;
		float m_kLinear = 0;
		float m_kQuadratic = 0;
		
		//PointLight
		bool m_bIsPoint = false;
		/*--------------------------------------------*/

		Vector4 off;
					/*Vulkan utility functions*/
		/*--------------------------------------------------------------*/
		//Send all data in shader
		void UploadBuffer(uint32_t currImg) const;

		//Index in buffer LightManager
		size_t m_indexBuffer = 0;
	};
}