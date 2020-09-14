// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Core/Support/Version.hpp>

// TODO: Clean later.
#include <Window/Config.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


#include <Rendering/Vulkan/VkRenderInstance.hpp>
#include <Rendering/Vulkan/VkValidationLayers.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	uint32 VkRenderInstance::sInitCount = 0u;

	void VkRenderInstance::Init()
	{
		SA_ASSERT(VkValidationLayers::CheckValidationSupport(), FeatureNotSupported, Rendering, L"Validation Layer not supported!")
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
	}

	const std::vector<const char*>& VkRenderInstance::GetRequiredExtensions() noexcept
	{
		static std::vector<const char*> extensions;

		if (extensions.size() == 0)
		{
			// Query extensions.
			uint32_t glfwExtensionCount = 0;
			const char** glfwExtensions;

			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
			extensions.assign(glfwExtensions, glfwExtensions + glfwExtensionCount);

#if SA_VK_VALIDATION_LAYERS

			extensions.push_back("VK_EXT_debug_utils");
#endif
		}

		return extensions;
	}
}

#endif
