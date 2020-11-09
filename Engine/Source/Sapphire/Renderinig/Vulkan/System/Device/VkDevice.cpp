// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System/Device/VkDevice.hpp>

#include <Collections/Debug>
#include <Core/Algorithms/SizeOf.hpp>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>
#include <Rendering/Vulkan/System/VkRenderSurface.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	std::vector<const char*> GetRequiredExtensions(QueueFamilyType _families)
	{
		// Present requiered extensions.
		static constexpr const char* presentRequieredExtensions[] =
		{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};


		std::vector<const char*> result;

		if (static_cast<uint8>(_families) & static_cast<uint8>(QueueFamilyType::Present))
			result.insert(result.end(), presentRequieredExtensions, presentRequieredExtensions + SizeOf(presentRequieredExtensions));

		return result;
	}

	bool CheckExtensionSupport(VkPhysicalDevice _device, QueueFamilyType _families)
	{
		uint32 extensionCount;
		vkEnumerateDeviceExtensionProperties(_device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(_device, nullptr, &extensionCount, availableExtensions.data());

		std::vector<const char*> requieredExtensions = GetRequiredExtensions(_families);

		// Check each asked supported.
		for (uint32 i = 0; i < SizeOf(requieredExtensions); ++i)
		{
			bool extensionFound = false;

			for (uint32 j = 0; j < availableExtensions.size(); ++j)
			{
				// Extension found.
				if (strcmp(requieredExtensions[i], availableExtensions[j].extensionName) == 0)
				{
					extensionFound = true;
					break;
				}
			}

			// Extension not found.
			if (!extensionFound)
				return false;
		}

		return true;
	}

	bool IsPhysicalDeviceSuitable(PhysicalDeviceInfos& _infos, const RenderSurface* _surface)
	{
#if SA_DEBUG

		if(static_cast<uint8>(_infos.familyTypes) & static_cast<uint8>(QueueFamilyType::Present))
			SA_ASSERT(_surface, InvalidParam, Rendering, L"QueueFamilyType::Present requiere a RenderSurface as parameter!");

#endif

		// Check requiered extensions.
		if (!CheckExtensionSupport(_infos.device, _infos.familyTypes))
			return false;


		// Check queue family suitability.
		_infos.QueryQueueFamilies(_surface);

		if (!_infos.QueueFamiliesCompleted())
			return false;


		// Graphics SwapChain support.
		//if (_surface && !_surface->CheckSupport(_infos.device))
		//	return false;


		// TODO: SAVE?
		// Check properties suitability.
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(_infos.device, &deviceProperties);

		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(_infos.device, &deviceFeatures);

		if (!deviceFeatures.samplerAnisotropy)
			return false;


		return true;
	}


	bool Device::IsValid() const noexcept
	{
		return mLogicalDevice != VK_NULL_HANDLE && mPhysicalDevice != VK_NULL_HANDLE;
	}


	void Device::Create(const PhysicalDeviceInfos& _infos)
	{
		VkPhysicalDeviceFeatures physicalDeviceFeatures{}; // Need braces to ensure C-struct correct initialization.
		//physicalDeviceFeatures.logicOp = VK_TRUE;
		physicalDeviceFeatures.sampleRateShading = VK_TRUE;
		physicalDeviceFeatures.samplerAnisotropy = VK_TRUE;

		std::vector<const char*> extensions = GetRequiredExtensions(_infos.familyTypes);
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos = _infos.GetDeviceCreateInfos();

		// TODO: Implement.
		_infos.graphics.queueNum = 3u;
		_infos.present.queueNum = 2u;

		VkDeviceCreateInfo deviceCreateInfo;
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.pNext = nullptr;
		deviceCreateInfo.flags = 0u;
		deviceCreateInfo.queueCreateInfoCount = SizeOf(queueCreateInfos);
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
		deviceCreateInfo.enabledLayerCount = 0u;
		deviceCreateInfo.ppEnabledLayerNames = nullptr;
		deviceCreateInfo.enabledExtensionCount = SizeOf(extensions);
		deviceCreateInfo.ppEnabledExtensionNames = extensions.data();
		deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;

#if SA_VK_VALIDATION_LAYERS

		deviceCreateInfo.enabledLayerCount = ValidationLayers::GetLayerNum();
		deviceCreateInfo.ppEnabledLayerNames = ValidationLayers::GetLayerNames();

#endif

		mPhysicalDevice = _infos.device;
	
		SA_VK_ASSERT(vkCreateDevice(mPhysicalDevice, &deviceCreateInfo, nullptr, &mLogicalDevice),
			CreationFailed, Rendering, L"Failed to create logical device!");

		mQueueMgr.Create(*this, _infos);
	}

	void Device::Destroy()
	{
		SA_ASSERT(IsValid(), Nullptr, Rendering, L"Try to destroy null logical device!");

		mQueueMgr.Destroy(*this);

		vkDestroyDevice(mLogicalDevice, nullptr);

		mLogicalDevice = VK_NULL_HANDLE;
		mPhysicalDevice = VK_NULL_HANDLE;
	}

	std::vector<PhysicalDeviceInfos> Device::QuerySuitableDevices(const RenderInstance& _instance, QueueFamilyType _requiredFamilies, const RenderSurface* _surface)
	{
		// Query Physical devices.
		uint32 deviceCount = 0;
		vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);

		SA_ASSERT(deviceCount, NotSupported, Rendering, L"No GPU with Vulkan support found!");

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());

		std::vector<PhysicalDeviceInfos> results;
		results.reserve(devices.size());

		for (uint32 i = 0; i < devices.size(); ++i)
		{
			PhysicalDeviceInfos infos(devices[i], _requiredFamilies);
			
			if(IsPhysicalDeviceSuitable(infos, _surface))
				results.push_back(infos);
		}

		SA_ASSERT(results.size(), NotSupported, Rendering, L"No suitable GPU found!");

		return results;
	}

	Device::operator VkDevice() const noexcept
	{
		return mLogicalDevice;
	}

	Device::operator VkPhysicalDevice() const noexcept
	{
		return mPhysicalDevice;
	}
}

#endif
