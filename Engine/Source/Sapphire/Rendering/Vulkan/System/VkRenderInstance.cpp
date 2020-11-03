// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Collections/Debug>
#include <Core/Support/Version.hpp>

#include <Window/IWindow.hpp>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>
#include <Rendering/Vulkan/System/VkValidationLayers.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	void VkRenderInstance::Init()
	{
		SA_ASSERT(VkValidationLayers::CheckValidationSupport(), NotSupported, Rendering, L"Validation Layer not supported!")
	}
	
	void VkRenderInstance::UnInit()
	{
	}

	void VkRenderInstance::SelectDevice(const VkRenderSurface& _surface)
	{
		// Query Physical devices.
		uint32 deviceCount = 0;
		vkEnumeratePhysicalDevices(mHandle, &deviceCount, nullptr);

		SA_ASSERT(deviceCount, NotSupported, Rendering, L"No GPU with Vulkan support found!");

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(mHandle, &deviceCount, devices.data());

		VkQueueFamilyIndices _queueFamilyIndices;

		for (uint32 i = 0; i < devices.size(); ++i)
		{
			// Select first suitable device.
			if (VkDevice::IsPhysicalDeviceSuitable(devices[i], _surface, _queueFamilyIndices))
			{
				mDevice.Create(devices[i], _queueFamilyIndices);
				break;
			}
		}

		SA_ASSERT(mDevice.IsValid(), NotSupported, Rendering, L"No suitable GPU found!");

		OnDeviceCreated();
	}

	void VkRenderInstance::OnDeviceCreated()
	{
		mCameras.Create(mDevice);

		mPointLightBuffer.Create(mDevice);
		mDirectionnalLightBuffer.Create(mDevice);
		mSpotLightBuffer.Create(mDevice);
	}

	std::vector<const char*> VkRenderInstance::GetRequiredExtensions() noexcept
	{
		std::vector<const char*> extensions;

		// Query window API required extensions.
		IWindow::GetRequiredExtensions(extensions);

#if SA_VK_VALIDATION_LAYERS

		extensions.push_back("VK_EXT_debug_utils");
#endif

		return extensions;
	}

	const VkDevice& VkRenderInstance::GetDevice() const
	{
		return mDevice;
	}

	const VkRenderSurface& VkRenderInstance::GetRenderSurface(const IWindow& _window) const
	{
		for (uint32 i = 0u; i < mSurfacePairs.size(); ++i)
		{
			if (mSurfacePairs[i].first == &_window)
				return mSurfacePairs[i].second;
		}

		SA_ASSERT(false, InvalidParam, Rendering, L"Window not registered as render surface!")

		return mSurfacePairs[0].second;
	}

	const VkGPUStorageBuffer<Camera_GPU>& VkRenderInstance::GetGPUCameraBuffer() const noexcept
	{
		return mCameras.GetGPUBuffer();
	}

	const VkGPUStorageBuffer<PLightInfos>& VkRenderInstance::GetPointLightBuffer() const noexcept
	{
		return mPointLightBuffer;
	}

	const VkGPUStorageBuffer<DLightInfos>& VkRenderInstance::GetDirectionnalLightBuffer() const noexcept
	{
		return mDirectionnalLightBuffer;
	}

	const VkGPUStorageBuffer<SLightInfos>& VkRenderInstance::GetSpotLightBuffer() const noexcept
	{
		return mSpotLightBuffer;
	}


	void VkRenderInstance::Create()
	{
		mInstance = this;

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

		std::vector<const char*> extensions = GetRequiredExtensions();

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
			SizeOf(extensions),													// enabledExtensionCount
			extensions.data(),													// ppEnabledExtensionNames.
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
			SizeOf(extensions),													// enabledExtensionCount
			extensions.data(),													// ppEnabledExtensionNames.
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
		mCameras.Destroy(mDevice);

		mPointLightBuffer.Destroy(mDevice);
		mDirectionnalLightBuffer.Destroy(mDevice);
		mSpotLightBuffer.Destroy(mDevice);

		// Destroy system.
		mDevice.Destroy();

#if SA_VK_VALIDATION_LAYERS

		auto destroyDebugFunc = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(mHandle, "vkDestroyDebugUtilsMessengerEXT");
		SA_ASSERT(destroyDebugFunc, ExtensionMissing, Rendering, L"Extension PFN_vkDestroyDebugUtilsMessengerEXT missing!");
		destroyDebugFunc(mHandle, debugMessenger, nullptr);

#endif

		vkDestroyInstance(mHandle, nullptr);
	}

	const IRenderSurface& VkRenderInstance::CreateRenderSurface(const IWindow& _window)
	{
#if SA_DEBUG

		// Check already registered.
		for (auto it = mSurfacePairs.begin(); it != mSurfacePairs.end(); ++it)
			SA_ASSERT(it->first != &_window, InvalidParam, Rendering, L"Window already registered as render surface!");

#endif

		// Create and register into mRenderSurfaceInfos.
		Pair<const IWindow*, VkRenderSurface>& surfacePair = mSurfacePairs.emplace_back(Pair<const IWindow*, VkRenderSurface>{ &_window });

		VkSurfaceKHR vkSurface = _window.CreateRenderSurface(*this);

		surfacePair.second.InitHandle(vkSurface, mDevice);


		// TODO: FIX.
		//// Init resize event.
		//_window.onResizeEvent.Add(std::function<void(const IWindow&, uint32, uint32)>(
		//	[this, &renderSurfaceInfo] (const IWindow& _win, uint32 _width, uint32 _height)
		//	{
		//		(void)_win;
		//		vkDeviceWaitIdle(mDevice);
		//		renderSurfaceInfo.renderSurface.ResizeCallback(*this, _width, _height);
		//	}
		//));


		// Init RenderSurface.

		// 1st surface: Device not selected yet.
		if (!mDevice.IsValid())
		{
			SelectDevice(surfacePair.second);
			surfacePair.second.InitHandle(vkSurface, mDevice);
		}

		// Create swapchain after the creation of the first pass
		//surfacePair.second.Create(mDevice, mDevice.GetQueueFamilyIndices());

		return surfacePair.second;
	}

	void VkRenderInstance::DestroyRenderSurface(const IWindow& _window)
	{
		bool bFound = false;

		for (auto it = mSurfacePairs.begin(); it != mSurfacePairs.end(); ++it)
		{
			if (it->first == &_window)
			{
				bFound = true;

				it->second.Destroy(mDevice);

				vkDestroySurfaceKHR(mHandle, it->second, nullptr);

				it->second.UnInitHandle();

				mSurfacePairs.erase(it);

				break;
			}
		}

		SA_ASSERT(bFound, InvalidParam, Rendering, L"Window not registered as render surface!")
	}


	ICamera& VkRenderInstance::InstantiateCamera()
	{
		return mCameras.Add(mDevice);
	}

	void VkRenderInstance::DestroyCamera(const ICamera& _camera)
	{
		return mCameras.Remove(mDevice, _camera.As<VkCamera>());
	}


	uint32 VkRenderInstance::InstantiatePointLight(const PLightInfos& _infos)
	{
		return mPointLightBuffer.Add(mDevice, _infos);
	}

	void VkRenderInstance::DestroyPointLight(uint32 _id)
	{
		mPointLightBuffer.Remove(mDevice, _id);
	}

	uint32 VkRenderInstance::InstantiateDirectionnalLight(const DLightInfos& _infos)
	{
		return mDirectionnalLightBuffer.Add(mDevice, _infos);
	}

	void VkRenderInstance::DestroyDirectionnalLight(uint32 _id)
	{
		mDirectionnalLightBuffer.Remove(mDevice, _id);
	}

	uint32 VkRenderInstance::InstantiateSpotLight(const SLightInfos& _infos)
	{
		return mSpotLightBuffer.Add(mDevice, _infos);
	}

	void VkRenderInstance::DestroySpotLight(uint32 _id)
	{
		mSpotLightBuffer.Remove(mDevice, _id);
	}


	void VkRenderInstance::Update()
	{
		mCameras.Update(*this);
	}


	VkRenderInstance::operator VkInstance() const
	{
		return mHandle;
	}
}

#endif
