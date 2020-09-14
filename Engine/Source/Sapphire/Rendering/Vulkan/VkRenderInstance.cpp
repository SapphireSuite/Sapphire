// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Core/Support/Version.hpp>

#include <Window/Config.hpp>

#if SA_WINDOW_API == SA_GLFW

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Window/GLFWWindow.hpp>

#endif

#include <Rendering/Vulkan/VkRenderInstance.hpp>
#include <Rendering/Vulkan/VkValidationLayers.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	uint32 VkRenderInstance::sInitCount = 0u;

	void VkRenderInstance::Init()
	{
		SA_ASSERT(VkValidationLayers::CheckValidationSupport(), NotSupported, Rendering, L"Validation Layer not supported!")
	}
	
	void VkRenderInstance::UnInit()
	{
	}

	void VkRenderInstance::Create()
	{
		// Init Vulkan.
		if (sInitCount++ == 0u)
			Init();


		const VkApplicationInfo appInfo
		{
			VK_STRUCTURE_TYPE_APPLICATION_INFO,											// sType.
			nullptr,																	// pNext.
			"Main Application",															// pApplicationName.
			VK_MAKE_VERSION(1, 0, 0),													// applicationVersion.
			"Sapphire Engine",															// pEngineName
			VK_MAKE_VERSION(SA_VERSION_MAJOR, SA_VERSION_MINOR, SA_VERSION_PATCH),		// engineVersion.
			VK_API_VERSION_1_0,															// apiVersion.
		};

#if SA_VK_VALIDATION_LAYERS

		// Debug Messenger Info.
		const VkDebugUtilsMessengerCreateInfoEXT debugUtilscreateInfo = VkValidationLayers::GetDebugUtilsMessengerCreateInfo();

		const VkInstanceCreateInfo instanceCreateInfo
		{
			VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,								// sType.
			&debugUtilscreateInfo,												// pNext.
			0,																	// flags,
			&appInfo,															// pApplicationInfo.
			VkValidationLayers::GetLayerNum(),									// enabledLayerCount.
			VkValidationLayers::GetLayerNames(),								// ppEnabledLayerNames.
			static_cast<uint32>(GetRequiredExtensions().size()),				// enabledExtensionCount
			GetRequiredExtensions().data(),										// ppEnabledExtensionNames.
		};

#else

		const VkInstanceCreateInfo instanceCreateInfo
		{
			VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,								// sType.
			nullptr,															// pNext.
			0,																	// flags,
			&appInfo,															// pApplicationInfo.
			0,																	// enabledLayerCount.
			nullptr,															// ppEnabledLayerNames.
			static_cast<uint32>(GetRequiredExtensions().size()),				// enabledExtensionCount
			GetRequiredExtensions().data(),										// ppEnabledExtensionNames.
		};


#endif

		SA_VK_ASSERT(vkCreateInstance(&instanceCreateInfo, nullptr, &mHandle), CreationFailed, Rendering, L"Failed to create vulkan instance!");


#if SA_VK_VALIDATION_LAYERS

		auto createDebugFunc = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(mHandle, "vkCreateDebugUtilsMessengerEXT");

		SA_ASSERT(createDebugFunc, ExtensionMissing, Rendering, L"Extension PFN_vkCreateDebugUtilsMessengerEXT missing!");
		SA_VK_ASSERT(createDebugFunc(mHandle, &debugUtilscreateInfo, nullptr, &debugMessenger), CreationFailed, Rendering, L"Failed to create vulkan debug messenger!");

#endif
	}

	void VkRenderInstance::Destroy()
	{
#if SA_VK_VALIDATION_LAYERS

		auto destroyDebugFunc = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(mHandle, "vkDestroyDebugUtilsMessengerEXT");
		SA_ASSERT(destroyDebugFunc, ExtensionMissing, Rendering, L"Extension PFN_vkDestroyDebugUtilsMessengerEXT missing!");
		destroyDebugFunc(mHandle, debugMessenger, nullptr);

#endif

		vkDestroyInstance(mHandle, nullptr);

		// UnInit Vulkan.
		if (--sInitCount == 0u)
			UnInit();
	}

	void VkRenderInstance::CreateRenderSurface(const IWindow& _window)
	{
#if SA_DEBUG

		// Check already registered.
		for (auto it = mRenderSurfaceInfos.begin(); it != mRenderSurfaceInfos.end(); ++it)
			SA_ASSERT(it->window != &_window, InvalidParam, Rendering, L"Window already registered as render surface!")

#endif

#if SA_WINDOW_API == SA_GLFW

		const GLFWWindow& glfwWindow = reinterpret_cast<const GLFWWindow&>(_window);

		RenderSurfaceInfos& renderSurfaceInfo = mRenderSurfaceInfos.emplace_back(RenderSurfaceInfos{ &_window });

		VkResult res = glfwCreateWindowSurface(
			mHandle,
			glfwWindow,
			nullptr,
			&(renderSurfaceInfo.renderSurface.operator VkSurfaceKHR &())
		);

		SA_ASSERT(res == VK_SUCCESS, CreationFailed, Window, L"Failed to create window surface!");
#else
#endif
	}

	void VkRenderInstance::DestroyRenderSurface(const IWindow& _window)
	{
		bool bFound = false;

		for (auto it = mRenderSurfaceInfos.begin(); it != mRenderSurfaceInfos.end(); ++it)
		{
			if (it->window == &_window)
			{
				bFound = true;

				vkDestroySurfaceKHR(mHandle,
					it->renderSurface.operator VkSurfaceKHR &(),
					nullptr
				);

				mRenderSurfaceInfos.erase(it);

				break;
			}
		}

		SA_ASSERT(bFound, InvalidParam, Rendering, L"Window not registered as render surface!")
	}


	const std::vector<const char*>& VkRenderInstance::GetRequiredExtensions() noexcept
	{
		static std::vector<const char*> extensions;

		if (extensions.size() == 0)
		{

#if SA_WINDOW_API == SA_GLFW

			// TODO: FIX
			glfwInit();

			// Query extensions.
			uint32_t glfwExtensionCount = 0;
			const char** glfwExtensions;

			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
			extensions.assign(glfwExtensions, glfwExtensions + glfwExtensionCount);

#endif

#if SA_VK_VALIDATION_LAYERS

			extensions.push_back("VK_EXT_debug_utils");
#endif
		}

		return extensions;
	}


	VkRenderInstance::operator VkInstance() const
	{
		return mHandle;
	}
}

#endif
