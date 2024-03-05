#pragma once

#include "GraphicAPI.h"
#include "LowRenderer/Camera.h"
#include "Renderpass.h"
#include "Pipeline.h"
#include "Image.h"
#include "LowRenderer/Light.h"
#include "Window/Window.h"
#include "Descriptors.h"

#include <chrono>
#include <random>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#include "libgfx_Export.h"
#include "LowRenderer/Model/Model.h"
#include "UI/UIWindowManager.h"

class LightManager;
class Window;

//TODO move pipeline ?
struct libgfx_API PipelineWithTarget
{
	Pipeline pipeline;
#pragma warning(push)
#pragma warning(disable: 4251)
	std::vector<Model> models;
#pragma warning(pop)
};

struct MeshIDMgr
{
	MeshIDMgr(std::string_view str, size_t c, size_t IDmesh) :
		count(c), paths(str), meshID(IDmesh)
	{
	}

	size_t count = static_cast<size_t>(-1);
	std::string paths;
	size_t meshID;
};

struct TextureIDMgr
{
	TextureIDMgr(std::string_view str, size_t c, size_t IDtext) :
		count(c), name(str), textID(IDtext)
	{
	}

	size_t count = static_cast<size_t>(-1);
	std::string name;
	size_t textID;
};

class RenderingMgr
{
public:
	Window* m_window;
	DescriptorSet descriptorSetEN;
	std::vector<DescriptorSetLayout> layouts;

	Sampler mainSampler;
	VkSampler m_viewportSampler;

	UIWindowManager m_UIWindowManager;

	/*Renderpass and IMGUI renderpass*/
	RenderPass m_renderpass;
	RenderPass m_imguiRenderPass;
#pragma warning(push)
#pragma warning(disable: 4251)
	std::vector<PipelineWithTarget> pipelines;
#pragma warning(pop)
	std::vector<MeshIDMgr> m_meshIdMgrs;
	std::vector<TextureIDMgr> m_textureIDMgrs;
	Matrix4 MVP;
	//TODO std::vector<Camera>
	LowRenderer::Camera* currCam = nullptr;

	std::vector<ImTextureID> m_viewportTextureIDs;
	
	//-------------------------------------------------------------------------------LIGHT------------------------------------------------------------------------------------
	LightManager* m_lightManager = nullptr;
	
	RenderingMgr() = default;
	~RenderingMgr() = default;
	
	/**
	* @brief Init Vulkan API
	*
	* @param User window pointer
	* @param Active aliasing
	*/
	libgfx_API void  InitGraphicAPI(Window* window, bool aliasing);
	
	libgfx_API void  Cleanup();
	void CleanupSwapChain();

	void CreateCommandBuffer();
	void RecreateSwapChain(Window* window);
	void RecreateFlyingCommandBuffer(int currImg);
	void CreateImGuiCommandBuffer();

	libgfx_API Model& EmplaceModel(std::string_view path);
	libgfx_API const size_t  EmplaceTexture(std::string_view path);

	void CreateViewportTexturesID();

	std::pair<std::vector<uint32_t>, std::vector<Vector3>> libgfx_API GetModelVertex(size_t index);

	void CreateSetLayouts();

	void PipelineCreation();

	void CreateDescriptorSets();

	void UpdateDescriptorSets();

	void UpdateUniformBuffer(uint32_t currImg);

	void FramebufferResizeCallback(int /*width*/, int /*height*/);
	
	void libgfx_API RemoveModel(size_t idModel);
	void libgfx_API RemoveTexture(size_t idModel);

	//return Id Model
	[[nodiscard]] size_t libgfx_API AddModel(std::string_view objPath);

	void libgfx_API ChangeModel(size_t index, std::string_view objPath);

	void libgfx_API AddTexture(std::string_view texturePath, size_t idModel);

	[[nodiscard]] size_t libgfx_API LoadModel(std::string_view objPath, std::string_view texturePath = "resources/images/missing-texture.png");
	
	/**
	* @brief Set matrix transform
	*
	* @param index Pipeline
	* @param index model
	* @param Matrix4 of model
	*/
	void libgfx_API SetTransform(size_t indexPipeline, size_t modelID, Matrix4& transform);

	void libgfx_API DrawFrame(Window* window);

	void SendStackedCmdBuffers();

	//void AddCamera(std::weak_ptr<Camera> cam)
	//{
	//	cameras.push_back(cam);
	//}

	void libgfx_API LoadDefaultModel();
	
	void libgfx_API LoadSimpleModel(std::vector<Vertex> vertices, std::vector<uint32_t> indices);

	void CreateImguiObjects();

	void RenderUI(uint64_t imageIndex);
private:
	GraphicAPI* m_graphicAPI = nullptr;
};