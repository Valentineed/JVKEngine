#pragma once

#include "GraphicAPI.h"
#include "Common.h"
#include "DescriptorBindFrag.h"
#include "DescriptorBindVert.h"
#include "Image.h"
#include "imgui.h"

#include <algorithm>
#include <cassert>

#define MAX_SETS_IN_POOL 400

class Model;

struct BindLayouts
{
	void AddBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags flags)
	{
		if (flags == VK_SHADER_STAGE_VERTEX_BIT)
		{
			vertexLayouts.push_back(DescriptorBindVertexLayout::MakeDescriptorLayout(binding, descriptorType));
		}
		else if (flags == VK_SHADER_STAGE_FRAGMENT_BIT)
		{
			fragmentLayouts.push_back(DescriptorBindFragmentLayout::MakeDescriptorLayout(binding, descriptorType));
		}
		f = flags;
	}
	std::vector<DescriptorBindVertexLayout> vertexLayouts;
	std::vector<DescriptorBindFragmentLayout> fragmentLayouts;
	VkShaderStageFlags f;
};

struct DescriptorSetLayout
{
	VkDescriptorSetLayout setLayout;
	std::vector<DescriptorBindVertexLayout> vertexBindings;
	std::vector<DescriptorBindFragmentLayout> fragmentBindings;

	static DescriptorSetLayout CreateDescriptorSetLayout(const BindLayouts& binds)
	{
		GraphicAPI* api = GraphicAPI::GetInstance();
		DescriptorSetLayout descSLayout;
		descSLayout.vertexBindings = binds.vertexLayouts;
		descSLayout.fragmentBindings = binds.fragmentLayouts;

		auto sortLambda = [](const auto& left, const auto& right) { return left.binding < right.binding; };
		std::sort(descSLayout.vertexBindings.begin(), descSLayout.vertexBindings.end(), sortLambda);
		std::sort(descSLayout.fragmentBindings.begin(), descSLayout.fragmentBindings.end(), sortLambda);

		std::vector<VkDescriptorSetLayoutBinding> bindings;
		for (auto& vertexBind : descSLayout.vertexBindings)
		{
			VkDescriptorSetLayoutBinding layoutBinding{};
			layoutBinding.binding = vertexBind.binding;
			layoutBinding.descriptorCount = 1;
			layoutBinding.descriptorType = vertexBind.descriptorType;
			layoutBinding.pImmutableSamplers = nullptr;
			layoutBinding.stageFlags = binds.f;

			bindings.push_back(layoutBinding);
		}
		for (auto& fragmentBind : descSLayout.fragmentBindings)
		{
			VkDescriptorSetLayoutBinding layoutBinding{};
			layoutBinding.binding = fragmentBind.binding;
			layoutBinding.descriptorCount = 1;	//TODO
			layoutBinding.descriptorType = fragmentBind.descriptorType;
			layoutBinding.pImmutableSamplers = nullptr;
			layoutBinding.stageFlags = binds.f;

			bindings.push_back(layoutBinding);
		}

		//--------------------------------------------------------------------Test--------------------------------------------------------------------

		//VkDescriptorSetLayoutBindingFlagsCreateInfoEXT setLayoutBindingFlags{};
		//setLayoutBindingFlags.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO_EXT;
		//setLayoutBindingFlags.bindingCount = 2;
		//std::vector<VkDescriptorBindingFlagsEXT> descriptorBindingFlags = {
		//	0,
		//	VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT_EXT
		//};
		//setLayoutBindingFlags.pBindingFlags = descriptorBindingFlags.data();

		//--------------------------------------------------------------------------------------------------------------------------------------------

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = (uint32_t)bindings.size();
		layoutInfo.pBindings = bindings.data();

		//--------------------------------------------------------------------Test--------------------------------------------------------------------

		//layoutInfo.pNext = &setLayoutBindingFlags;

		//--------------------------------------------------------------------------------------------------------------------------------------------

		if (vkCreateDescriptorSetLayout(api->GetDevice(), &layoutInfo, nullptr, &descSLayout.setLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create descriptor set layout!");
		}

		return descSLayout;
	}
};

struct DescriptorSet
{
	std::vector<SavedTexture> textures;

	DescriptorSetLayout descriptorSetLayout;
	VkDescriptorSet set;

	//vertex and fragment buffers must be sorted in lowest to highest binding values.
	//e.g. if you're binding a uniform buffer in slot 0 and a camera in slot 1
	//the uniform buffer MUST come before the camera buffer.

	void CreateDescriptorPool(std::vector<DescriptorSetLayout> setLayouts);

	void CreateImGuiDescriptorPool()
	{
		GraphicAPI* api = GraphicAPI::GetInstance();

		VkDescriptorPoolSize pool_sizes[] =
		{
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};
		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
		pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
		pool_info.pPoolSizes = pool_sizes;
		vkCreateDescriptorPool(api->GetDevice(), &pool_info, nullptr, &api->MutImGuiDescriptorPool());
	}

	void CreateDescriptorSets(std::vector<Model>& models);

};