#include "RenderingManager.h"
#include "Buffers.h"
#include "LightManager.h"
#include "Core/SwapChain.h"
#include "Resources/TextureLoader.h"
#include "Resources/SavedData.h"

libgfx_API void RenderingMgr::InitGraphicAPI(Window* window, bool aliasing)
{
	if (window == nullptr)
	{
		throw std::runtime_error("Window is nullptr!");
	}
	m_lightManager = &LightManager::GetInstance();

	/*Set callback to resize*/
	m_window = window;
	window->SetFramebufferSizeCallback(this, &RenderingMgr::FramebufferResizeCallback);

	//Init GraphicAPI
	m_graphicAPI = GraphicAPI::GetInstance();
	m_graphicAPI->Init(window, aliasing);

	m_renderpass = RenderPass::CreateRenderpass();		//! Create m_renderpass before PipelineCreation !
	m_imguiRenderPass = RenderPass::CreateImGuiRenderPass();

	CreateSetLayouts();
	PipelineCreation();									//CreateSetLayout called in PipelineCreation before the vkCreateGraphicsPipelines call
	m_graphicAPI->CreateCommandPool();
	m_graphicAPI->CreateImGuiCommandPool();

	m_graphicAPI->CreateFramebuffers(m_renderpass.GetRenderPass());
	m_graphicAPI->CreateImGuiFramebuffers(m_imguiRenderPass.GetRenderPass());

	DataBuffers::CreateUniformBuffers();
	descriptorSetEN.CreateDescriptorPool(layouts);
	descriptorSetEN.CreateImGuiDescriptorPool();

	/*LightManager::ProcessAlignedSize();*/
	m_lightManager->Init();
	LowRenderer::Camera::ProcessAlignedSize();
	Model::ProcessAlignedSize();

	currCam = new LowRenderer::Camera();

	//descriptorSetEN.textures.emplace_back(DataTexture::CreateTexture(DEFAULT_TEXTURE_PATH, DataTexture::RGBA, false), static_cast<size_t>(descriptorSetEN.textures.size()));
	//m_textureIDMgrs.emplace_back(std::filesystem::path(DEFAULT_TEXTURE_PATH).filename().string(), 1, descriptorSetEN.textures[0].ID);
	EmplaceTexture(DEFAULT_TEXTURE_PATH);

	CreateDescriptorSets();
	UpdateDescriptorSets();
	CreateCommandBuffer();
	CreateImGuiCommandBuffer();
	m_graphicAPI->CreateSyncObjects();

	CreateImguiObjects();

	CreateViewportTexturesID();
}

void RenderingMgr::RecreateSwapChain(Window* window)
{
	GraphicAPI* api = GraphicAPI::GetInstance();
	//We will pause the program until the window is brought back to the foreground. At that point we will recreate the swap chain.
	int width = 0, height = 0;
	window->GetFramebufferSize(&width, &height);
	while (width == 0 || height == 0)
	{
		window->GetFramebufferSize(&width, &height);
		window->WaitEvents();
	}

	std::vector<Model> TempModel;
	for (auto& model : pipelines[0].models)
	{
		TempModel.push_back(std::move(model));
	}

	//Prevents touching resources that are in use.

	CleanupSwapChain();

	api->RecreateSwapChain(window);

	m_renderpass = RenderPass::CreateRenderpass();
	m_imguiRenderPass = RenderPass::CreateImGuiRenderPass();
	PipelineCreation();

	for (auto& model : TempModel)
	{
		pipelines[0].models.push_back(std::move(model));
	}

	api->CreateFramebuffers(m_renderpass.GetRenderPass());
	api->CreateImGuiFramebuffers(m_imguiRenderPass.GetRenderPass());
	//DataBuffers::CreateUniformBuffers(); //-> Destroyed in CleanUp just before Destroy descPool
	//descriptorSetEN.CreateDescriptorPool(layouts);
	//descriptorSetEN.CreateImGuiDescriptorPool();
	//CreateDescriptorSets();
	UpdateDescriptorSets();
	CreateCommandBuffer();
	CreateImGuiCommandBuffer();

	CreateViewportTexturesID();
}

void RenderingMgr::CreateCommandBuffer()
{
	GraphicAPI* api = GraphicAPI::GetInstance();

	//Records display commands
	api->MutCommandBuffers().resize(api->GetSwapChainFBuffers().size());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = api->GetCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)api->GetCommandBuffers().size();

	if (vkAllocateCommandBuffers(api->GetDevice(), &allocInfo, api->MutCommandBuffers().data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffers!");
	}

	for (size_t i = 0; i < api->GetCommandBuffers().size(); i++)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(api->GetCommandBuffers()[i], &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		//The display begins with the launch of the render pass
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = *m_renderpass.GetRenderPass();
		renderPassInfo.framebuffer = api->GetSwapChainFBuffers()[i];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = api->GetSwapChain()->GetSwapChainExtent();

		//Color of clear background + stencil buffer
		std::array<VkClearValue, 3> clearValues{};
		clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		clearValues[2].color = { 0.0f, 0.0f, 0.0f, 1.0f };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		//vkCmdSetScissor(api->GetCommandBuffers()[i], 0, 1, &api->MutScreenScissor());
		//vkCmdSetViewport(api->GetCommandBuffers()[i], 0, 1, &api->MutScreenViewport());

		vkCmdBeginRenderPass(api->GetCommandBuffers()[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		size_t lastMatID = static_cast<size_t>(-1);

		//for (auto&& camera : cameras)
		for (auto& pL : pipelines)
		{
			vkCmdBindPipeline(api->GetCommandBuffers()[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pL.pipeline.graphicsPipeline);

			vkCmdBindDescriptorSets(api->GetCommandBuffers()[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pL.pipeline.pipelineLayout, 2, 1, &api->GetDescriptorSets()[currCam->m_setID][i], 0, nullptr);

			//Bind All Light Descriptor Set in  set 0
			m_lightManager->BindAllDescriptorSets(i, pL.pipeline.pipelineLayout);

			for (auto& model : pL.models)
			{
				if (lastMatID != model.m_matID && model.m_matID != -1)
				{
					vkCmdBindDescriptorSets(api->GetCommandBuffers()[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pL.pipeline.pipelineLayout, 1, 1, &api->GetDescriptorSets()[descriptorSetEN.textures[model.m_matID].setID][i], 0, nullptr);
				}
				//ASK for command buffer, how to init, for descSet does it bother having it not being created when the texture sent is the same
				model.Draw(api->GetCommandBuffers()[i], pL.pipeline.pipelineLayout, api->GetDescriptorSets()[model.m_setID][i]);
				lastMatID = model.m_matID;
			}
		}

		vkCmdEndRenderPass(api->GetCommandBuffers()[i]);

		if (vkEndCommandBuffer(api->GetCommandBuffers()[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}

void RenderingMgr::RecreateFlyingCommandBuffer(int currImg)
{
	GraphicAPI* api = GraphicAPI::GetInstance();

	//Records display commands
	api->MutCommandBuffers().resize(api->GetSwapChainFBuffers().size());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = api->GetCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)api->GetCommandBuffers().size();

	if (vkAllocateCommandBuffers(api->GetDevice(), &allocInfo, api->MutCommandBuffers().data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffers!");
	}

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(api->GetCommandBuffers()[currImg], &beginInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	//The display begins with the launch of the render pass
	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = *m_renderpass.GetRenderPass();
	renderPassInfo.framebuffer = api->GetSwapChainFBuffers()[currImg];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = api->GetSwapChain()->GetSwapChainExtent();

	//Color of clear background + stencil buffer
	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	clearValues[1].depthStencil = { 1.0f, 0 };

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	//vkCmdSetScissor(api->GetCommandBuffers()[i], 0, 1, &api->MutScreenScissor());
	//vkCmdSetViewport(api->GetCommandBuffers()[i], 0, 1, &api->MutScreenViewport());

	vkCmdBeginRenderPass(api->GetCommandBuffers()[currImg], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	//for (auto&& camera : cameras)
	for (auto pL : pipelines)
	{
		vkCmdBindPipeline(api->GetCommandBuffers()[currImg], VK_PIPELINE_BIND_POINT_GRAPHICS, pL.pipeline.graphicsPipeline);

		for (auto model : pL.models)
		{
			//TODO Check if static indice cause problems
			model.Draw(api->GetCommandBuffers()[currImg], pL.pipeline.pipelineLayout, api->GetDescriptorSets()[model.m_matID][currImg]);
		}
	}

	vkCmdEndRenderPass(api->GetCommandBuffers()[currImg]);

	if (vkEndCommandBuffer(api->GetCommandBuffers()[currImg]) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to record command buffer!");
	}
}

void RenderingMgr::CreateImGuiCommandBuffer()
{
	GraphicAPI* api = GraphicAPI::GetInstance();

	api->MutImGuiCommandBuffers().resize(api->GetSwapChainFBuffers().size());

	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.commandPool = api->GetImGuiCommandPool();
	commandBufferAllocateInfo.commandBufferCount = (uint32_t)api->GetImGuiCommandBuffers().size();
	if (vkAllocateCommandBuffers(api->GetDevice(), &commandBufferAllocateInfo, api->MutImGuiCommandBuffers().data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void libgfx_API RenderingMgr::RemoveModel(size_t idModel)
{
	if (idModel != pipelines[0].models.size() - 1)
	{
		std::iter_swap(pipelines[0].models.begin() + idModel, pipelines[0].models.end() - 1);
	}

	GraphicAPI* api = GraphicAPI::GetInstance();
	bool bShouldReplace = true;
	for (size_t modelID = 0; modelID < pipelines[0].models.size(); modelID++)
	{
		if (modelID != pipelines[0].models.size() - 1)
		{
			if (pipelines[0].models[modelID].m_meshID == pipelines[0].models.back().m_meshID)
			{
				bShouldReplace = false;
				m_meshIdMgrs[pipelines[0].models.back().m_meshID].count--;
				break;
			}
		}
	}
	if (bShouldReplace)
	{
		auto& model = pipelines[0].models.back();
		for (auto& targetMesh : pipelines[0].models.back().m_targetMeshes)
		{
			vkDestroyBuffer(api->GetDevice(), targetMesh.indexBuffer.buffer, nullptr);
			targetMesh.indexBuffer.buffer = VK_NULL_HANDLE;
			vkFreeMemory(api->GetDevice(), targetMesh.indexBuffer.bufferMemory, nullptr);
			targetMesh.indexBuffer.bufferMemory = VK_NULL_HANDLE;

			vkDestroyBuffer(api->GetDevice(), targetMesh.vertexBuffer.buffer, nullptr);
			targetMesh.vertexBuffer.buffer = VK_NULL_HANDLE;
			vkFreeMemory(api->GetDevice(), targetMesh.vertexBuffer.bufferMemory, nullptr);
			targetMesh.vertexBuffer.bufferMemory = VK_NULL_HANDLE;
		}
		SavedData* data = SavedData::GetInstance();

		data->RemoveData(m_meshIdMgrs[model.m_meshID].paths);
		m_meshIdMgrs.erase(m_meshIdMgrs.begin() + model.m_meshID);
	}
	pipelines[0].models.back().CleanupModel();
	pipelines[0].models.pop_back();
	RecreateSwapChain(m_window);
}

void libgfx_API RenderingMgr::RemoveTexture(size_t idModel)
{
	size_t idTexture = pipelines[0].models[idModel].m_matID;
	

	bool bShouldReplace = true;
	for (size_t modelID = 0; modelID < pipelines[0].models.size(); modelID++)
	{
		if (modelID != idModel)
		{
			if (pipelines[0].models[modelID].m_matID == idTexture)
			{

				bShouldReplace = false;
				m_textureIDMgrs[pipelines[0].models.back().m_matID].count--;
				break;
			}
		}
	}
	//RecreateSwapChain(m_window);
}

void RenderingMgr::Cleanup()
{
	GraphicAPI* api = GraphicAPI::GetInstance();

	CleanupSwapChain();

	//TODO: Destroy sampler/Image views used in file browser window

	for (auto& pL : pipelines)
	{
		for (auto& model : pL.models)
		{
			m_meshIdMgrs[model.m_meshID].count--;

			if (m_meshIdMgrs[model.m_meshID].count == 0)
			{
				for (auto& targetMesh : model.m_targetMeshes)
				{
					vkDestroyBuffer(api->GetDevice(), targetMesh.indexBuffer.buffer, nullptr);
					vkFreeMemory(api->GetDevice(), targetMesh.indexBuffer.bufferMemory, nullptr);

					vkDestroyBuffer(api->GetDevice(), targetMesh.vertexBuffer.buffer, nullptr);
					vkFreeMemory(api->GetDevice(), targetMesh.vertexBuffer.bufferMemory, nullptr);
				}
			}
			model.CleanupModel();
		}

		//Destroy All Buffers in Light
		m_lightManager->DestroyAllBuffer();
	}

	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	for (auto& texture : descriptorSetEN.textures)
	{
		texture.Destroy();
	}

	//TODO for all cam
	delete currCam;

	//vkDestroyDescriptorSetLayout(api->GetDevice(), descriptorSetEN.descriptorSetLayout.setLayout, nullptr);

	for (auto& layout : layouts)
	{
		vkDestroyDescriptorSetLayout(api->GetDevice(), layout.setLayout, nullptr);
	}

	for (size_t i = 0; i < api->GetMaxFramesIF(); i++)
	{
		vkDestroySemaphore(api->GetDevice(), api->MutRenderFinishedSemaphores()[i], nullptr);
		vkDestroySemaphore(api->GetDevice(), api->MutImageAvailableSemaphores()[i], nullptr);
		vkDestroyFence(api->GetDevice(), api->MutInFlightFences()[i], nullptr);
	}

	const size_t swapChainImgCount = api->GetSwapChain()->ImageCount();
	for (size_t i = 0; i < swapChainImgCount; i++)
	{
		for (size_t j = 0; j < api->GetUniformBuffers()[i].size(); j++)
		{
			vkDestroyBuffer(api->GetDevice(), api->MutUniformBuffers()[i][j], nullptr);
			vkFreeMemory(api->GetDevice(), api->MutUniformBuffersMemory()[i][j], nullptr);
		}
	}

	vkDestroyCommandPool(api->GetDevice(), api->MutCommandPool(), nullptr);
	vkDestroyCommandPool(api->GetDevice(), api->MutImGuiCommandPool(), nullptr);

	vkDestroyDescriptorPool(api->GetDevice(), api->MutDescriptorPool(), nullptr);
	vkDestroyDescriptorPool(api->GetDevice(), api->MutImGuiDescriptorPool(), nullptr);

	vkDestroyDevice(api->GetDevice(), nullptr);

	if (g_enableValidationLayers)
	{
		api->GetDebug().DestroyDebugUtils();
	}

	vkDestroySurfaceKHR(api->GetVkInstance(), api->MutSurface(), nullptr);

	//Destroy Vulkan instance after all clears
	vkDestroyInstance(api->MutVkInstance(), nullptr);
}

void RenderingMgr::CleanupSwapChain()
{
	GraphicAPI* api = GraphicAPI::GetInstance();

	vkDeviceWaitIdle(api->GetDevice());

	api->DestroyStorageSwapChain();

	vkDestroySampler(api->GetDevice(), mainSampler.sampler, nullptr);
	vkDestroySampler(api->GetDevice(), m_viewportSampler, nullptr);

	for (size_t i = 0; i < api->GetSwapChainFBuffers().size(); i++)
	{
		vkDestroyFramebuffer(api->GetDevice(), api->MutSwapChainFBuffers()[i], nullptr);
	}

	for (size_t i = 0; i < api->GetImGuiFBuffers().size(); i++)
	{
		vkDestroyFramebuffer(api->GetDevice(), api->MutImGuiFBuffers()[i], nullptr);
	}

	vkFreeCommandBuffers(api->GetDevice(), api->MutCommandPool(), static_cast<uint32_t>(api->GetCommandBuffers().size()), api->MutCommandBuffers().data());
	vkFreeCommandBuffers(api->GetDevice(), api->MutImGuiCommandPool(), static_cast<uint32_t>(api->GetImGuiCommandBuffers().size()), api->MutImGuiCommandBuffers().data());

	for (auto& pL : pipelines)
	{
		vkDestroyPipeline(api->GetDevice(), pL.pipeline.graphicsPipeline, nullptr);
		vkDestroyPipelineLayout(api->GetDevice(), pL.pipeline.pipelineLayout, nullptr);
	}

	vkDestroyRenderPass(api->GetDevice(), *m_renderpass.GetRenderPass(), nullptr);
	vkDestroyRenderPass(api->GetDevice(), *m_imguiRenderPass.GetRenderPass(), nullptr);
}

Model& RenderingMgr::EmplaceModel(std::string_view path)
{
	GraphicAPI* api = GraphicAPI::GetInstance();
	SavedData* data = SavedData::GetInstance();

	auto tempRsc = data->GetResourceImpl(path);
	Model& loadedModel = dynamic_cast<Model&>(*tempRsc.first);
	if (&loadedModel == nullptr)
	{
		throw std::runtime_error("dynamic_cast on Model from saved data failed");
	}
	{
		std::lock_guard<std::mutex> lock{ api->GetSubmitMutex() };

		std::filesystem::path fileName = path;
		std::string name = fileName.filename().string();

		if (tempRsc.second)
		{
			loadedModel.m_meshID = m_meshIdMgrs.size();

			m_meshIdMgrs.emplace_back(name, 1, loadedModel.m_meshID);				//First added to vector so count == 1
		}
		else
		{
			for (auto& meshIDs : m_meshIdMgrs)
			{
				if (meshIDs.paths == name)
				{
					loadedModel.m_meshID = meshIDs.meshID;
					meshIDs.count++;
					break;
				}
			}
			loadedModel.UploadData();
		}
	}

	loadedModel.CreateDescriptorSet(layouts[3]);
	return loadedModel;
}

const size_t RenderingMgr::EmplaceTexture(std::string_view path)
{
	SavedData* data = SavedData::GetInstance();

	size_t id = 0;
	auto tempRsc = data->GetResourceImpl(path);
	std::filesystem::path fileName = path;
	std::string name = fileName.filename().string();

	if (tempRsc.second)
	{
		id = descriptorSetEN.textures.size();
		Image* tempImg = dynamic_cast<Image*>(tempRsc.first);
		if (tempImg == nullptr)
		{
			throw std::runtime_error("dynamic_cast on Image from saved data failed");
		}
		tempImg->m_ID = id;
		descriptorSetEN.textures.emplace_back(*tempImg, id);
		descriptorSetEN.textures[descriptorSetEN.textures.size() - 1].CreateDescriptorSet(layouts[1]);

		m_textureIDMgrs.emplace_back(name, 1, tempImg->m_ID);
	}
	else
	{
		Image* loadedImg = dynamic_cast<Image*>(tempRsc.first);
		id = loadedImg->m_ID;
		for (auto& texID : m_textureIDMgrs)
		{
			if (texID.name == name)
			{
				texID.count++;
				break;
			}
		}
	}

	return id;
}

void RenderingMgr::CreateViewportTexturesID()
{
	m_viewportSampler = Sampler::CreateSampler(Sampler::Linear).sampler;

	GraphicAPI* api = GraphicAPI::GetInstance();
	// Grab how many images we have in swapchain
	const size_t swapchain_imagecount = api->GetSwapChain()->ImageCount();

	m_viewportTextureIDs.clear();
	m_viewportTextureIDs = std::vector<ImTextureID>(swapchain_imagecount);

	// Create Texture ID's for rendering Viewport to ImGui Window
	for (int i = 0; i < swapchain_imagecount; i++)
	{
		m_viewportTextureIDs[i] = ImGui_ImplVulkan_AddTexture(m_viewportSampler, api->GetSwapChain()->GetImageOffScreen(i).imageView,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}
}

std::pair<std::vector<uint32_t>, std::vector<Vector3>>libgfx_API RenderingMgr::GetModelVertex(size_t index)
{
	std::vector<Vector3> vertPos;
	std::vector<uint32_t> indices;
	for (auto mesh : pipelines[0].models[index].m_targetMeshes)
	{
		indices = mesh.indices;
		for (auto vertex : mesh.vertices)
		{
			vertPos.push_back(vertex.pos);
		}
	}
	return std::make_pair(indices, vertPos);
}

void RenderingMgr::CreateSetLayouts()
{
	BindLayouts modelLayout;
	modelLayout.AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);					//3
	BindLayouts camLayout;
	camLayout.AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT);					//2
	BindLayouts textLayout;
	textLayout.AddBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);			//1
	BindLayouts lightLayout;
	lightLayout.AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT);				//0

	layouts.push_back(DescriptorSetLayout::CreateDescriptorSetLayout(lightLayout));
	layouts.push_back(DescriptorSetLayout::CreateDescriptorSetLayout(textLayout));
	layouts.push_back(DescriptorSetLayout::CreateDescriptorSetLayout(camLayout));
	layouts.push_back(DescriptorSetLayout::CreateDescriptorSetLayout(modelLayout));
}

void RenderingMgr::PipelineCreation()
{
	pipelines.clear();
	mainSampler = Sampler::CreateSampler(Sampler::Linear);

	auto pipeline = Pipeline::CreatePipeline("resources/shaders/vert.spv", "resources/shaders/frag.spv", layouts, m_renderpass.GetRenderPass());
	pipelines.push_back({ std::move(pipeline),{} });
}

void RenderingMgr::CreateDescriptorSets()
{
	for (auto& pL : pipelines)
	{
		//Create All DescriptorSets in layout 0
		m_lightManager->CreateAllDescriptorSets(layouts[0]);

		for (auto& texture : descriptorSetEN.textures)
		{
			texture.CreateDescriptorSet(layouts[1]);
		}

		currCam->CreateDescriptorSet(layouts[2]);

		for (auto& model : pL.models)
		{
			model.CreateDescriptorSet(layouts[3]);
		}
	}
}

void RenderingMgr::UpdateDescriptorSets()
{
	for (auto& pL : pipelines)
	{
		//Update All DescriptorSets in LightManager
		m_lightManager->UpdateAllDescriptorSets();

		for (auto& texture : descriptorSetEN.textures)
		{
			texture.UpdateDescriptorSet(texture.image.m_imageView, mainSampler.sampler);
		}

		currCam->UpdateDescriptorSet();

		for (auto& model : pL.models)
		{
			model.UpdateDescriptorSet();
		}
	}
}

void RenderingMgr::UpdateUniformBuffer(uint32_t currImg)
{
	GraphicAPI* api = GraphicAPI::GetInstance();

	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
	(void)time; //CASTED TO IGNORE WARNING AS ERROR

	if (!pipelines.empty())
	{
		void* data;
		for (size_t i = 0; i < pipelines[0].models.size(); ++i)
		{
			GraphicAPI::UniformBufferObject ubo{};
			ubo.model = pipelines[0].models[i].m_modelMat;
			ubo.view = currCam->m_view;
			ubo.proj = currCam->m_proj;
			ubo.proj[1][1] *= -1;

			GraphicAPI::UniformMVP uniformMVP{};
			//uniformMVP.MVP = ubo.proj * ubo.view * ubo.model;
			uniformMVP.VP = ubo.proj * ubo.view;
			uniformMVP.model = ubo.model;

			//PUSHCONSTANT
			//VkCommandBuffer TempCmdBuff = api->BeginSingleTimeCommands();
			//vkCmdPushConstants(TempCmdBuff, pipelines[0].pipeline.pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Matrix4), &MVP);
			//api->EndSingleTimeCommands(TempCmdBuff);

			//MVP
			data = nullptr;
			vkMapMemory(api->GetDevice(), pipelines[0].models[i].m_uniformBuffer.bufferMemory, pipelines[0].models[i].m_alignedSize * currImg, sizeof(GraphicAPI::UniformMVP), 0, &data);
			memcpy(data, &uniformMVP, sizeof(uniformMVP));
			vkUnmapMemory(api->GetDevice(), pipelines[0].models[i].m_uniformBuffer.bufferMemory);
		}

		//Send All Lights To Shader MapMemory/memcpy/UnmapMemory
		m_lightManager->UpdateShader(currImg);

		GraphicAPI::UniformViewPos uvp{};
		uvp.viewPos = currCam->m_pos;

		//ViewPos
		data = nullptr;
		vkMapMemory(api->GetDevice(), currCam->m_cameraBuffer.bufferMemory, currCam->m_alignedSize * currImg, sizeof(GraphicAPI::UniformViewPos), 0, &data);
		memcpy(data, &uvp, sizeof(uvp));
		vkUnmapMemory(api->GetDevice(), currCam->m_cameraBuffer.bufferMemory);
	}
}

void RenderingMgr::FramebufferResizeCallback(int, int)
{
	auto app = reinterpret_cast<GraphicAPI*>(m_window->GetUserPointer());
	app->MutIsBufferResized() = true;
}

size_t libgfx_API RenderingMgr::AddModel(std::string_view objPath)
{
	Model& newModel = EmplaceModel(objPath);

	size_t index = pipelines[0].models.size();
	pipelines[0].models.push_back(std::move(newModel));

	RecreateSwapChain(m_window);
	return index;
}

void libgfx_API RenderingMgr::ChangeModel(size_t index, std::string_view objPath)
{
	GraphicAPI* api = GraphicAPI::GetInstance();

	bool bShouldReplace = true;
	for (size_t modelID = 0; modelID < pipelines[0].models.size(); modelID++)
	{
		if (modelID != index)
		{
			if (pipelines[0].models[modelID].m_meshID == pipelines[0].models[index].m_meshID)
			{
				m_meshIdMgrs[pipelines[0].models[index].m_meshID].count--;
				size_t idMat = pipelines[0].models[index].m_matID;
				pipelines[0].models[index] = EmplaceModel(objPath);
				pipelines[0].models[index].m_matID = idMat;
				bShouldReplace = false;
				break;
			}
		}
	}
	if (bShouldReplace)
	{
		const auto model = pipelines[0].models[index];
		for (auto targetMesh : model.m_targetMeshes)
		{
			vkDestroyBuffer(api->GetDevice(), targetMesh.indexBuffer.buffer, nullptr);
			targetMesh.indexBuffer.buffer = VK_NULL_HANDLE;
			vkFreeMemory(api->GetDevice(), targetMesh.indexBuffer.bufferMemory, nullptr);
			targetMesh.indexBuffer.bufferMemory = VK_NULL_HANDLE;

			vkDestroyBuffer(api->GetDevice(), targetMesh.vertexBuffer.buffer, nullptr);
			targetMesh.vertexBuffer.buffer = VK_NULL_HANDLE;
			vkFreeMemory(api->GetDevice(), targetMesh.vertexBuffer.bufferMemory, nullptr);
			targetMesh.vertexBuffer.bufferMemory = VK_NULL_HANDLE;
		}
		SavedData* data = SavedData::GetInstance();

		data->RemoveData(m_meshIdMgrs[model.m_meshID].paths);
		m_meshIdMgrs.erase(m_meshIdMgrs.begin() + model.m_meshID);
		pipelines[0].models[index].CleanupModel();
		pipelines[0].models[index] = EmplaceModel(objPath);
	}
	RecreateSwapChain(m_window);
}

void libgfx_API RenderingMgr::AddTexture(std::string_view texturePath, size_t idModel)
{
	if (pipelines[0].models[idModel].m_matID != SIZE_MAX && m_textureIDMgrs[pipelines[0].models[idModel].m_matID].count == 0)
		RemoveTexture(pipelines[0].models[idModel].m_matID);

	pipelines[0].models[idModel].m_matID = EmplaceTexture(texturePath);
	RecreateSwapChain(m_window);
}

size_t libgfx_API RenderingMgr::LoadModel(std::string_view objPath, std::string_view texturePath)
{
	size_t id = EmplaceTexture(texturePath);
	Model& newModel = EmplaceModel(objPath);
	newModel.m_matID = id;

	const size_t index = pipelines[0].models.size();
	pipelines[0].models.push_back(newModel);

	//RecreateSwapChain(m_window);
	return index;
}

void libgfx_API RenderingMgr::SetTransform(size_t indexPipeline, size_t modelID, Matrix4& transform)
{
	pipelines[indexPipeline].models[modelID].m_modelMat = transform;
}

void libgfx_API RenderingMgr::DrawFrame(Window* window)
{
	GraphicAPI* api = GraphicAPI::GetInstance();

	static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	/*The drawFrame function will do the following:
	Acquire an image from the swap chain
	Execute the command buffer corresponding to the framebuffer whose attachment is the image obtained
	Return the image to the swap chain for presentation*/
	//This function helps synchronize the swap chain (wait for semaphores)
	vkWaitForFences(api->GetDevice(), 1, &api->MutInFlightFences()[api->GetCurrentFrame()], VK_TRUE, UINT64_MAX);

	//Acquired an image from the swap chain.
	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(api->GetDevice(), api->GetSwapChain()->Get(), UINT64_MAX, api->GetImageAvailableSemaphores()[api->GetCurrentFrame()], VK_NULL_HANDLE, &imageIndex);

	m_UIWindowManager.Update(m_viewportTextureIDs[imageIndex]);

	//Todo review
	/*VK_ERROR_OUT_OF_DATE_KHR: the swap chain is no longer compatible with the window surface and can no longer be used for display, which usually happens with a window resizing*/
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateSwapChain(window);
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	SendStackedCmdBuffers();

	UpdateUniformBuffer(imageIndex);

	if (api->GetImagesInFlight()[imageIndex] != VK_NULL_HANDLE)
	{
		vkWaitForFences(api->GetDevice(), 1, &api->MutImagesInFlight()[imageIndex], VK_TRUE, UINT64_MAX);
	}
	api->MutImagesInFlight()[imageIndex] = api->GetInFlightFences()[api->GetCurrentFrame()];

	//TODO HOW TO PUSHCONSTANTS -_-
	//VkCommandPoolResetFlagBits resetFlag{};
	//vkResetCommandPool(api->GetDevice(), api->MutCommandPool(), resetFlag);
	//RecreateFlyingCommandBuffer(imageIndex);
	//vkCmdPushConstants(api->GetCommandBuffers()[imageIndex], pipelines[0].pipeline.pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Matrix4), &MVP);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { api->GetImageAvailableSemaphores()[api->GetCurrentFrame()] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &api->MutCommandBuffers()[imageIndex];

	VkSemaphore signalSemaphores[] = { api->GetRenderFinishedSemaphores()[api->GetCurrentFrame()] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(api->GetDevice(), 1, &api->MutInFlightFences()[api->GetCurrentFrame()]);

	//Send the command buffer to the graphic queue
	if (vkQueueSubmit(api->GetGraphicsQueue(), 1, &submitInfo, api->MutInFlightFences()[api->GetCurrentFrame()]) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit draw command buffer!");
	}
	//The present send the result in swap chain (wait for the semaphore)

	{
		//VUL_EXCEPT_RESULT(vkResetCommandPool(device, commandPool, 0));
		VkCommandBufferBeginInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		vkBeginCommandBuffer(api->MutImGuiCommandBuffers()[imageIndex], &info);
	}
	{
		VkClearValue clear_value = { {clear_color.x,clear_color.y,clear_color.z,clear_color.w} };
		VkRenderPassBeginInfo imguiPassInfo = {};
		imguiPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		imguiPassInfo.renderPass = *m_imguiRenderPass.GetRenderPass();
		imguiPassInfo.framebuffer = api->GetImGuiFBuffers()[imageIndex];
		imguiPassInfo.renderArea.extent.width = api->GetSwapChain()->GetSwapChainExtent().width;
		imguiPassInfo.renderArea.extent.height = api->GetSwapChain()->GetSwapChainExtent().height;
		imguiPassInfo.clearValueCount = 2;//TODO  stock vector<Imgs>
		imguiPassInfo.pClearValues = &clear_value;
		vkCmdBeginRenderPass(api->MutImGuiCommandBuffers()[imageIndex], &imguiPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	// Record Imgui Draw Data and draw funcs into command buffer
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), api->MutImGuiCommandBuffers()[imageIndex]);

	// Submit command buffer
	vkCmdEndRenderPass(api->MutImGuiCommandBuffers()[imageIndex]);
	{
		VkFence submitFence;
		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = 0;
		if (vkCreateFence(api->GetDevice(), &fenceInfo, nullptr, &submitFence) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create fence for a frame!");
		}

		VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		VkSubmitInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		//info.waitSemaphoreCount = 1;
		//info.pWaitSemaphores = waitSemaphores;
		info.pWaitDstStageMask = &wait_stage;
		info.commandBufferCount = 1;
		info.pCommandBuffers = &api->MutImGuiCommandBuffers()[imageIndex];
		//info.signalSemaphoreCount = 1;
		//info.pSignalSemaphores = signalSemaphores;

		vkEndCommandBuffer(api->MutImGuiCommandBuffers()[imageIndex]);
		vkQueueSubmit(api->GetGraphicsQueue(), 1, &info, submitFence);
		vkWaitForFences(api->GetDevice(), 1, &submitFence, VK_TRUE, UINT64_MAX);
		vkDestroyFence(api->GetDevice(), submitFence, nullptr);
		//vkResetFences(api->GetDevice(), 1, &api->MutInFlightFences()[api->GetCurrentFrame()]);
		//vkQueueSubmit(api->GetGraphicsQueue(), 1, &info, api->MutInFlightFences()[api->GetCurrentFrame()]);
	}
	//vkDeviceWaitIdle(api->GetDevice());

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { api->GetSwapChain()->Get() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	//Sends the request to presentInfo an image by the swap chain
	result = vkQueuePresentKHR(api->GetPresentQueue(), &presentInfo);
	//In this case we recreate the swap chain if it is no longer optimal because we want the best possible results
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || api->GetIsBufferResized())
	{
		api->MutIsBufferResized() = false;
		RecreateSwapChain(window);
	}
	else if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to present swap chain image!");
	}

	api->MutCurrentFrame() = (api->GetCurrentFrame() + 1) % api->GetMaxFramesIF();
}

void RenderingMgr::SendStackedCmdBuffers()
{
	GraphicAPI* api = GraphicAPI::GetInstance();

	std::lock_guard<std::mutex> lock{ api->GetSubmitMutex() };
	if (api->GetSubmitInfos().size() > 0)
	{
		VkFence submitFence;
		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = 0;
		if (vkCreateFence(api->GetDevice(), &fenceInfo, nullptr, &submitFence) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create fence for a frame!");
		}

		VkSubmitInfo submitInfos{};
		submitInfos.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfos.commandBufferCount = static_cast<uint32_t>(api->GetSubmitInfos().size());
		submitInfos.pCommandBuffers = api->GetSubmitInfos().data();

		vkQueueSubmit(api->GetGraphicsQueue(), 1, &submitInfos, submitFence);
		vkWaitForFences(api->GetDevice(), 1, &submitFence, VK_TRUE, UINT64_MAX);
		vkDestroyFence(api->GetDevice(), submitFence, nullptr);

		vkFreeCommandBuffers(api->GetDevice(), api->GetCommandPool(), static_cast<uint32_t>(api->GetSubmitInfos().size()), api->GetSubmitInfos().data());
		api->MutSubmitInfos().clear();
	}
	if (api->GetDeletionQueue().size() > 0)
	{
		for (auto& [buffer, memory] : api->GetDeletionQueue())
		{
			vkDestroyBuffer(api->GetDevice(), buffer, nullptr);
			vkFreeMemory(api->GetDevice(), memory, nullptr);
		}

		api->MutDeletionQueue().clear();
	}
}

void libgfx_API RenderingMgr::LoadDefaultModel()
{
	Model newModel;

	newModel.CreateDefault();
	newModel.UploadData();
	newModel.m_meshID = m_meshIdMgrs.size();
	m_meshIdMgrs.emplace_back(".", 1, newModel.m_meshID);

	pipelines[0].models.push_back(std::move(newModel));
}

void libgfx_API RenderingMgr::LoadSimpleModel(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
{
	Model newModel;

	newModel.m_targetMeshes[0].vertices = vertices;
	newModel.m_targetMeshes[0].indices = indices;
	newModel.UploadData();

	pipelines[0].models.push_back(std::move(newModel));
}

void RenderingMgr::CreateImguiObjects()
{
	GraphicAPI* api = GraphicAPI::GetInstance();
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings

	ImGui_ImplGlfw_InitForVulkan(m_window->GetWindow(), true);
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = api->GetVkInstance();
	init_info.PhysicalDevice = api->GetPhysicalDevice();
	init_info.Device = api->GetDevice();
	init_info.QueueFamily = 0;
	init_info.Queue = api->GetGraphicsQueue();
	init_info.PipelineCache = VK_NULL_HANDLE;
	init_info.DescriptorPool = api->GetImGuiDescriptorPool();
	init_info.Allocator = nullptr;
	init_info.MinImageCount = 2;
	init_info.ImageCount = 2;
	//init_info.CheckVkResultFn = [](VkResult result) {
	//	VUL_EXCEPT_RESULT(result);
	//};
	ImGui_ImplVulkan_Init(&init_info, *m_imguiRenderPass.GetRenderPass());

	VkCommandBuffer command_buffer = api->BeginSingleTimeCommands();
	ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
	api->EndSingleTimeCommands(command_buffer);
}

void RenderingMgr::RenderUI(uint64_t imageIndex)
{
	GraphicAPI* api = GraphicAPI::GetInstance();
	// Record Imgui Draw Data and draw funcs into command buffer
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), api->GetImGuiCommandBuffers()[imageIndex]);
}