#pragma once
#include "LowRenderer/Light.h"
#include "Vector/Vector3.h"
#include <vector>
#include "libgfx_Export.h"

class libgfx_API LightManager
{
public:
	using vec3 = LibMath::Vector3;
	using vec4 = LibMath::Vector4;

	static LightManager& GetInstance();


					/*InitBuffer*/
	/*-----------------------------------------*/
	void Init();
	inline static VkDeviceSize s_alignedSize = 0;
	DataBuffers m_uniformBuffer;
	size_t m_setID;
	inline static const size_t m_maxLight = 10;
	/*-----------------------------------------*/
														/*Lights Options*/
	/*---------------------------------------------------------------------------------------------------------------------------------*/
	static size_t DirectionalLight(const vec4& direction, const vec3& ambientColor, const vec3& diffuseColor, const vec3& specularColor);
	
	static size_t PointLight(const vec4& position, const vec3& ambientColor, const vec3& diffuseColor, const vec3& specularColor, float  constAttenuation,
		float linAttenuation, float quadAttenuation);
	
	static size_t  SpotLight(const vec4& position, const vec3& direction, float cutOff, float outerCutOff, const vec3& ambientColor, const vec3& diffuseColor, const vec3& specularColor, float constAttenuation,
		float linAttenuation, float quadAttenuation);
	/*-------------------------------------------------------------------------------------------------------------------------------*/

							/*User Functions*/
	/*-----------------------------------------------------------*/
	static LowRenderer::Light* GetLight(size_t index);
	static void SetLight(LowRenderer::Light& light, size_t index);
	/*-----------------------------------------------------------*/

	static void RemoveLight(size_t index);

								/*Vulkan utility functions*/
	/*-------------------------------------------------------------------------------------*/
	void CreateAllDescriptorSets(DescriptorSetLayout& setLayout);

	void BindAllDescriptorSets(size_t indexCommandBuffer, VkPipelineLayout& pipelineLayout);

	void UpdateAllDescriptorSets() const;
	
	void DestroyAllBuffer() const;
	
	void UpdateShader(uint32_t currImg);
	/*------------------------------------------------------------------------------------*/

	
	LightManager(LightManager const&) = delete;
	LightManager& operator=(LightManager const&) = delete;
private:
	/*Singleton*/
	LightManager() = default;
	~LightManager() = default;
	static LightManager m_instance;

					/*InitBuffer*/
	/*-----------------------------------------*/
	void ProcessAlignedSize();
	void InitializeBuffer() const;
	/*-----------------------------------------*/
	
	void RemoveLightInShader(size_t index);
	void ClearLight(size_t index, size_t currImg);
#pragma warning(push)
#pragma warning(disable: 4251)
	std::vector<LowRenderer::Light> m_lights;
#pragma warning(pop)
};


