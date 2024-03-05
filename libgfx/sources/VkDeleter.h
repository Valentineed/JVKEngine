#pragma once
#include <vulkan/vulkan.h>

#include <functional>
#include <memory>

template<typename Type>
struct VkDeleter
{
	VkDeleter() : VkDeleter([](Type, VkAllocationCallbacks*) {}) {}
	VkDeleter(std::function<void(Type, VkAllocationCallbacks*)> deletef)
		: m_resources(std::make_shared<ResourceBlock>(ResourceBlock{}))
	{
		m_resources->deleter = [=](Type obj) { deletef(obj, nullptr); };
	}

	VkDeleter(const VkDeleter<VkInstance>& instance, std::function<void(VkInstance, Type, VkAllocationCallbacks*)> deletef)
		: m_resources(std::make_shared<ResourceBlock>(ResourceBlock{}))
	{
		m_resources->deleter = [&instance, deletef](Type obj) { deletef(instance, obj, nullptr); };
	}

	VkDeleter(const VkDeleter<VkDevice>& device, std::function<void(VkDevice, Type, VkAllocationCallbacks*)> deletef)
		: m_resources(std::make_shared<ResourceBlock>(ResourceBlock{}))
	{
		m_resources->deleter = [&device, deletef](Type obj) { deletef(device, obj, nullptr); };
	}

	const Type* operator&() const
	{
		return &m_resources->object;
	}
	Type* Replace()
	{
		m_resources->Cleanup();
		return &m_resources->object;
	}

	operator Type() const
	{
		return m_resources->object;
	}

	void operator=(Type right)
	{
		if (right != m_resources->object)
		{
			m_resources->Cleanup();
			m_resources->object = right;
		}
	}

	template<typename V>
	bool operator==(V right)
	{
		return m_resources->object == Type(right);
	}
private:
	/*shared_ptr to assert things arent getting deleted too early*/
	struct ResourceBlock
	{
		Type object{ VK_NULL_HANDLE };
		std::function<void(Type)> deleter;
		~ResourceBlock()
		{
			Cleanup();
		}

		void Cleanup()
		{
			if (object != VK_NULL_HANDLE)
			{
				deleter(object);
			}
			object = VK_NULL_HANDLE;
		}
	};

	std::shared_ptr<ResourceBlock> m_resources;
};
 