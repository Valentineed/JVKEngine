#include "LightManager.h"

#include "Descriptors.h"
#include "Core/SwapChain.h"

LightManager LightManager::m_instance;

LightManager& LightManager::GetInstance()
{
	return m_instance;
}

void LightManager::Init()
{
	ProcessAlignedSize();
	m_uniformBuffer = DataBuffers::CreatHostVisibileBuffer(s_alignedSize * GraphicAPI::GetInstance()->GetSwapChain()->ImageCount() * m_maxLight);

	InitializeBuffer();
}

void LightManager::InitializeBuffer() const
{
	GraphicAPI* api = GraphicAPI::GetInstance();
	const size_t swapchain_imagecount = api->GetSwapChain()->ImageCount();
	
	void* data = nullptr;
	vkMapMemory(api->GetDevice(), m_uniformBuffer.bufferMemory, 0, VK_WHOLE_SIZE, 0, &data);
	memset(data, 0, s_alignedSize * m_maxLight * swapchain_imagecount);
	vkUnmapMemory(api->GetDevice(), m_uniformBuffer.bufferMemory);
}

void LightManager::ProcessAlignedSize()
{
	s_alignedSize = GraphicAPI::GetInstance()->Align(sizeof(GraphicAPI::LightBufferObject),
		GraphicAPI::GetInstance()->GetPhysProperties().limits.
		minUniformBufferOffsetAlignment);
}

size_t LightManager::DirectionalLight(const vec4& direction, const vec3& ambientColor, const vec3& diffuseColor,
                                    const vec3& specularColor)
{
	size_t index = m_instance.m_lights.size();
	m_instance.m_lights.emplace_back(direction, ambientColor, diffuseColor, specularColor);
	m_instance.m_lights.back().m_indexBuffer = index;
	return index;
}

size_t LightManager::PointLight(const vec4& position, const vec3& ambientColor, const vec3& diffuseColor, const vec3& specularColor, float constAttenuation, float linAttenuation, float quadAttenuation)
{
	size_t index = m_instance.m_lights.size();
	m_instance.m_lights.emplace_back(position, ambientColor, diffuseColor, specularColor, constAttenuation, linAttenuation,
		quadAttenuation, true);
	m_instance.m_lights.back().m_indexBuffer = index;
	return index;
}

size_t LightManager::SpotLight(const vec4& position, const vec3& direction, float cutOff, float outerCutOff, const vec3& ambientColor, const vec3& diffuseColor, const vec3& specularColor, float constAttenuation, float linAttenuation, float quadAttenuation)
{
	size_t index = m_instance.m_lights.size();
	m_instance.m_lights.emplace_back(position, direction, cutOff, outerCutOff, ambientColor, diffuseColor, specularColor, constAttenuation,
		linAttenuation, quadAttenuation);
	m_instance.m_lights.back().m_indexBuffer = index;
	return index;
}

LowRenderer::Light* LightManager::GetLight(size_t index)
{
	return &m_instance.m_lights[index];
}

void LightManager::SetLight(LowRenderer::Light& light, size_t index)
{
	m_instance.m_lights[index] = light;
}

void LightManager::RemoveLight(size_t index)
{
	if(m_instance.m_lights.size() - 1 == index)
	{
		m_instance.m_lights.pop_back();
	}
	else
	{
		m_instance.m_lights.back().m_indexBuffer = index;
		
		std::iter_swap(m_instance.m_lights.begin() + index, m_instance.m_lights.end() - 1);

		m_instance.m_lights.pop_back();
	}
	m_instance.RemoveLightInShader(m_instance.m_lights.size());
	
}

void LightManager::RemoveLightInShader(size_t index)
{
	GraphicAPI* api = GraphicAPI::GetInstance();

	const size_t swapchain_imagecount = api->GetSwapChain()->ImageCount();


	for (size_t imageCount = 0; imageCount < swapchain_imagecount; imageCount++)
	{
		ClearLight(index, imageCount);
	}
}

void LightManager::ClearLight(size_t index, size_t currImg)
{
	GraphicAPI* api = GraphicAPI::GetInstance();
	GraphicAPI::LightBufferObject lbo{};
	Vector4 zeroVec4(0, 0, 0, 0);
	Vector3 zeroVec = Vector3::Zero;
	lbo.position = zeroVec4;
	lbo.direction = zeroVec;

	lbo.ambientColor = zeroVec;
	lbo.specularColor = zeroVec;
	lbo.diffuseColor = zeroVec;

	lbo.kConstant = 0;
	lbo.kLinear = 0;
	lbo.kQuadratic = 0;

	lbo.cutOff = 0;
	lbo.outerCutOff = 0;

	lbo.bIsPoint = false;

	//Send to Shader
	void* data = nullptr;
	LightManager* lightManager = &GetInstance();
	vkMapMemory(api->GetDevice(), lightManager->m_uniformBuffer.bufferMemory, s_alignedSize * (m_maxLight * currImg + index), s_alignedSize, 0, &data);
	memcpy(data, &lbo, sizeof(lbo));
	vkUnmapMemory(api->GetDevice(), lightManager->m_uniformBuffer.bufferMemory);
}

void LightManager::CreateAllDescriptorSets(DescriptorSetLayout& setLayout)
{
	GraphicAPI* api = GraphicAPI::GetInstance();

	const size_t swapChainImageCount = api->GetSwapChain()->ImageCount();
	std::vector<VkDescriptorSetLayout> layouts(swapChainImageCount, setLayout.setLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = api->GetDescriptorPool();	//If using Pool of pool method, need to dynamically swap descpPool
	allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImageCount);
	allocInfo.pSetLayouts = layouts.data();

	std::vector<VkDescriptorSet>* NewSet = __nullptr;

	NewSet = &api->MutDescriptorSets().emplace_back();
	m_setID = api->GetDescriptorSets().size() - 1;
	NewSet->resize(swapChainImageCount);

	if (vkAllocateDescriptorSets(api->GetDevice(), &allocInfo, (*NewSet).data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate descriptor sets!");
	}
}

void LightManager::BindAllDescriptorSets(size_t indexCommandBuffer, VkPipelineLayout& pipelineLayout)
{
	GraphicAPI* api = GraphicAPI::GetInstance();
	vkCmdBindDescriptorSets(api->GetCommandBuffers()[indexCommandBuffer], VK_PIPELINE_BIND_POINT_GRAPHICS,
		pipelineLayout, 0, 1, &api->GetDescriptorSets()[m_setID][indexCommandBuffer], 0, nullptr);
}

void LightManager::UpdateAllDescriptorSets() const
{
	GraphicAPI* api = GraphicAPI::GetInstance();

	std::vector<VkWriteDescriptorSet> descriptorWrites;
	descriptorWrites.reserve(api->GetDescriptorSets()[m_setID].size());
	for (size_t iSet = 0; iSet < api->GetDescriptorSets()[m_setID].size(); iSet++)
	{
		VkDescriptorBufferInfo lightInfo{};
		lightInfo.buffer = m_uniformBuffer.buffer;
		lightInfo.offset = s_alignedSize * iSet * m_maxLight;
		lightInfo.range = s_alignedSize * m_maxLight;

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = api->GetDescriptorSets()[m_setID][iSet];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &lightInfo;

		descriptorWrites.push_back(descriptorWrite);
	}
	vkUpdateDescriptorSets(api->GetDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

void LightManager::DestroyAllBuffer() const
{
	GraphicAPI* api = GraphicAPI::GetInstance();
	vkDestroyBuffer(api->GetDevice(), m_uniformBuffer.buffer, nullptr);
	vkFreeMemory(api->GetDevice(), m_uniformBuffer.bufferMemory, nullptr);
}

void LightManager::UpdateShader(uint32_t currImg)
{
	for (auto& light : m_lights)
	{
		light.UploadBuffer(currImg);
	}
}
