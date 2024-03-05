#pragma once
#include "Matrix/Matrix4.h"
#include "Buffers.h"

using namespace  LibMath;
struct DescriptorSetLayout;

namespace LowRenderer
{
	class Camera
	{
	public:
		using vec3 = Vector3;
		using mat4 = Matrix4;

		libgfx_API Camera();
		virtual ~Camera();

		vec3 m_pos;
		mat4 m_view;
		mat4 m_proj;
		size_t m_setID = static_cast<size_t>(-1);
		DataBuffers m_cameraBuffer;
		
		static VkDeviceSize m_alignedSize;

		static void ProcessAlignedSize();
		
		void Destroy();
		void UploadData();
		void CreateDescriptorSet(DescriptorSetLayout& setLayout);
		void UpdateDescriptorSet();
	};
}
