// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>

#include <Collections/Debug>

#include <Core/Support/Version.hpp>
#include <Core/Algorithms/SizeOf.hpp>

#include <Window/Framework/System/IWindow.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	std::vector<const char*> GetRequiredExtensions() noexcept
	{
		std::vector<const char*> extensions;

		// Query window API required extensions.
		IWindow::GetRequiredExtensions(extensions);

#if SA_VK_VALIDATION_LAYERS

		extensions.push_back("VK_EXT_debug_utils");
#endif

		return extensions;
	}


	void RenderInstance::SelectDevice(QueueType _requiredFamilies, const RenderSurface* _surface)
	{
		std::vector<PhysicalDeviceInfos> deviceInfos = Device::QuerySuitableDevices(*this, _requiredFamilies, _surface);

		// Select first suitable device.
		// TODO: Add score: https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Physical_devices_and_queue_families
		device.Create(deviceInfos[0]);

		SA_ASSERT(device.IsValid(), NotSupported, Rendering, L"No suitable GPU found!");
	}


	void RenderInstance::Create()
	{
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pNext = nullptr;
		appInfo.pApplicationName = "Main Application";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Sapphire Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(SA_VERSION_MAJOR, SA_VERSION_MINOR, SA_VERSION_PATCH);
		appInfo.apiVersion = VK_API_VERSION_1_0;


		std::vector<const char*> extensions = GetRequiredExtensions();


		VkInstanceCreateInfo instanceCreateInfo{};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pNext = nullptr;
		instanceCreateInfo.flags = 0;
		instanceCreateInfo.pApplicationInfo = &appInfo;
		instanceCreateInfo.enabledLayerCount = 0u;
		instanceCreateInfo.ppEnabledLayerNames = nullptr;
		instanceCreateInfo.enabledExtensionCount = SizeOf(extensions);
		instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

#if SA_VK_VALIDATION_LAYERS

		// Debug Messenger Info.
		const VkDebugUtilsMessengerCreateInfoEXT debugUtilscreateInfo = ValidationLayers::GetDebugUtilsMessengerCreateInfo();

		instanceCreateInfo.pNext = &debugUtilscreateInfo;

		instanceCreateInfo.enabledLayerCount = ValidationLayers::GetLayerNum();
		instanceCreateInfo.ppEnabledLayerNames = ValidationLayers::GetLayerNames();

#endif

		SA_VK_ASSERT(vkCreateInstance(&instanceCreateInfo, nullptr, &mHandle), CreationFailed, Rendering, L"Failed to create vulkan instance!");


#if SA_VK_VALIDATION_LAYERS

		auto createDebugFunc = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(mHandle, "vkCreateDebugUtilsMessengerEXT");

		SA_ASSERT(createDebugFunc, ExtensionMissing, Rendering, L"Extension PFN_vkCreateDebugUtilsMessengerEXT missing!");
		SA_VK_ASSERT(createDebugFunc(mHandle, &debugUtilscreateInfo, nullptr, &debugMessenger), CreationFailed, Rendering, L"Failed to create vulkan debug messenger!");

#endif
	}

	void RenderInstance::Destroy()
	{
		if(device.IsValid())
			device.Destroy();


#if SA_VK_VALIDATION_LAYERS

		auto destroyDebugFunc = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(mHandle, "vkDestroyDebugUtilsMessengerEXT");
		SA_ASSERT(destroyDebugFunc, ExtensionMissing, Rendering, L"Extension PFN_vkDestroyDebugUtilsMessengerEXT missing!");
		destroyDebugFunc(mHandle, debugMessenger, nullptr);

#endif

		vkDestroyInstance(mHandle, nullptr);
		mHandle = VK_NULL_HANDLE;
	}

	IRenderSurface& RenderInstance::CreateRenderSurface(const IWindow& _window)
	{
		// Create.
		VkSurfaceKHR vkSurface = _window.CreateRenderSurface(*this);

		// Register.
		RenderSurface& renderSurface = mSurfaces.emplace_back(new RenderSurface(vkSurface))->As<RenderSurface>();

		// TODO: FIX.
		//// Init resize event.
		//_window.onResizeEvent.Add(std::function<void(const IWindow&, uint32, uint32)>(
		//	[this, &renderSurfaceInfo] (const IWindow& _win, uint32 _width, uint32 _height)
		//	{
		//		(void)_win;
		//		vkDeviceWaitIdle(device);
		//		renderSurfaceInfo.renderSurface.ResizeCallback(*this, _width, _height);
		//	}
		//));


		// 1st surface: Device not selected yet.
		if (!device.IsValid())
			SelectDevice(QueueType::Default, &renderSurface); // TODO: CLEAN?

		// Create swapchain after the creation of the first pass
		renderSurface.Create(*this);

		return renderSurface;
	}

	void RenderInstance::DestroyRenderSurface(const IRenderSurface& _surface)
	{
		for (auto it = mSurfaces.begin(); it != mSurfaces.end(); ++it)
		{
			if (*it == &_surface)
			{
				(*it)->Destroy(*this);

				vkDestroySurfaceKHR(mHandle, (*it)->As<RenderSurface>(), nullptr);

				mSurfaces.erase(it);

				break;
			}
		}
	}


	void RenderInstance::Init()
	{
		SA_ASSERT(ValidationLayers::CheckValidationSupport(), NotSupported, Rendering, L"Validation Layer not supported!")
	}

	void RenderInstance::UnInit()
	{
	}


	RenderInstance::operator VkInstance() const noexcept
	{
		return mHandle;
	}
}

#endif
