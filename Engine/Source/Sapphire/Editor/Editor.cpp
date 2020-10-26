#include <Editor/Editor.hpp>

#include <Window/GLFWWindow.hpp>
#include <Rendering/Vulkan/System/VkRenderInstance.hpp>

#include <imgui.h>
#include <imgui_internal.h>
#include "imgui_impl_glfw.h"
#include <imgui_impl_vulkan.h>

#include <string>

namespace Sa::edtr
{
	void check_vk_result(const VkResult err)
	{
		if (err == VK_SUCCESS)
			return;

		throw std::runtime_error("VkResult " + std::to_string((int)err));
	}

	Editor::Editor(const GLFWWindowT& _window, const VkRenderInstance& _renderInstance)
	{
		{
			// TODO Aurel: temporary, create better descriptor pool management
			VkDescriptorPoolSize pool_sizes[] =
			{
				{ VK_DESCRIPTOR_TYPE_SAMPLER, 100 },
				{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100 },
				{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 100 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 100 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 100 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 100 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 100 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 100 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 100 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 100 },
				{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 100 }
			};
			VkDescriptorPoolCreateInfo pool_info = {};
			pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
			pool_info.maxSets = 100 * IM_ARRAYSIZE(pool_sizes);
			pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
			pool_info.pPoolSizes = pool_sizes;
			
			SA_VK_ASSERT(vkCreateDescriptorPool(_renderInstance.GetDevice(), &pool_info, nullptr, &mDescriptorPool),
							CreationFailed, Rendering, L"Failed to create descriptor pool!");
		}

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForVulkan(_window.Get(), true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = _renderInstance;
		init_info.PhysicalDevice = _renderInstance.GetDevice();
		init_info.Device = _renderInstance.GetDevice();
		init_info.QueueFamily = _renderInstance.GetDevice().GetQueueFamilyIndices().graphicsFamily; // SURE ?
		init_info.Queue = _renderInstance.GetDevice().GetGraphicsQueue();
		init_info.PipelineCache = VK_NULL_HANDLE;
		init_info.DescriptorPool = mDescriptorPool;
		init_info.Allocator = VK_NULL_HANDLE;
		init_info.MinImageCount = 3; // Sure ?
		init_info.ImageCount = 3; // TODO Aurel: change hardcoded images number
		init_info.MSAASamples = 
			static_cast<VkSampleCountFlagBits>(_renderInstance.mSurfacePairs.begin()->second.mRenderPasses[0].GetSampleBits());
		init_info.CheckVkResultFn = check_vk_result;
		// TODO Aurel: change renderPass handling
		ImGui_ImplVulkan_Init(&init_info, _renderInstance.mSurfacePairs.begin()->second.mRenderPasses[0].Get());

		// Load Fonts
		// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
		// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
		// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
		// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
		// - Read 'docs/FONTS.txt' for more instructions and details.
		// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
		//io.Fonts->AddFontDefault();
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
		//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
		//IM_ASSERT(font != NULL);

		// Upload Fonts
		{
			// Use any command queue
			CommandBuffer commandBuffer = CommandBuffer::BeginSingleTimeCommands(_renderInstance.GetDevice(),
															_renderInstance.GetDevice().GetGraphicsQueue());
			ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
			CommandBuffer::EndSingleTimeCommands(_renderInstance.GetDevice(), commandBuffer,
				_renderInstance.GetDevice().GetGraphicsQueue());
			ImGui_ImplVulkan_DestroyFontUploadObjects();
		}

	}

	Editor::~Editor()
	{
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Editor::StartFrame()
	{
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow();
	}

	void Editor::Draw(const VkRenderFrame& _frame)
	{
		ImGui::Render();

		ImDrawData* draw_data = ImGui::GetDrawData();
		ImGui_ImplVulkan_RenderDrawData(draw_data, _frame.framebuffer->GetCommandBuffer());
	}

	void Editor::EndFrame()
	{
		ImGui::EndFrame();
		ImGui::UpdatePlatformWindows();
	}
}